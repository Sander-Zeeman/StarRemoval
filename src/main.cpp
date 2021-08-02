//#include "matplotlibcpp.h"

#include "../include/Clusterer.h"
#include "../include/Debug.h"
#include "../include/Detector.h"
#include "../include/Image.h"
#include "../include/MaxTree.h"
#include "../include/Point.h"
#include "../include/PostProcessing.h"

#include <opencv2/photo.hpp>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <time.h>


void interpolate(Image *img, long *ids)
{
    TimerWrapper::TimerInstance()->startTimer();

    bool *maskArray = new bool[img->size()];
    for (long i = 0; i < img->size(); i++) {
        maskArray[i] = (ids[i] >= 0);
    }

    float *data = img->data();
    int lowX, highX, lowY, highY;
    float interpolatedValue;
    for (int y = 0; y < img->height(); y++) {
        for (int x = 0; x < img->width(); x++) {
            if (maskArray[y * img->width() + x]) {
                findLimits(maskArray, img->width(), img->height(), x, y, lowX, highX, lowY, highY);
                interpolatedValue = interpolate(data, img->width(), img->height(), x, y, lowX, highX, lowY, highY);
                data[y * img->width() + x] = interpolatedValue;
            }
        }
    }

    TimerWrapper::TimerInstance()->stopTimer("Interpolation");
}

void NSInpaint(Image *img, long *ids, bool ns)
{
    TimerWrapper::TimerInstance()->startTimer();

    std::cout << img->width() << " " << img->height() << " " << img->size() << std::endl;

    cv::Mat input = cv::Mat(
        img->height(),
        img->width(),
        CV_32FC1,
        img->data()
    );

    bool *maskArray = new bool[img->size()];
    for (long i= 0; i < img->size(); i++) {
        maskArray[i] = (ids[i] >= 0);
    }

    cv::Mat mask = cv::Mat(
        img->height(),
        img->width(),
        CV_8UC1,
        maskArray
    );

    cv::Mat output;


    cv::inpaint(input, mask, output, 5, ns ? cv::INPAINT_NS : cv::INPAINT_TELEA);
    delete [] maskArray;

    float *oldImg = img->data();
    float *newImg = output.ptr<float>();
    for (long i = 0; i < img->size(); i++) {
        oldImg[i] = newImg[i];
    }

    TimerWrapper::TimerInstance()->stopTimer("Inpainting");
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Expecting a file as argument!" << std::endl;
        exit(1);
    }

    TimerWrapper::TimerInstance()->startTimer();

    Image *tempImg;
    std::vector<float> zoomData;
    int zoomX = 2874;
    int zoomY = 1867;
    int zoomWidth = 800;
    int zoomHeight = 800;


    // Reading the image
    Image *img = new Image(static_cast<char*>(argv[1]));
    img->writeImage("final_noise.fits");
    float *data = img->data();
    for (int y = 0; y < zoomHeight; y++) {
        for (int x = 0; x < zoomWidth; x++) {
            zoomData.push_back(data[(y + zoomY) * img->width() + (x + zoomX)]);
        }
    }
    tempImg = new Image("final_zoom_noise.fits", zoomData, zoomWidth, zoomHeight);
    tempImg->writeImage("final_zoom_noise.fits");
    delete tempImg;
    return 0;

    img->estimateBackground();
    float *data = img->data();
    for (int y = 0; y < zoomHeight; y++) {
        for (int x = 0; x < zoomWidth; x++) {
            zoomData.push_back(data[(y + zoomY) * img->width() + (x + zoomX)]);
        }
    }
    img->writeImage("final_BG.fits");
    tempImg = new Image("final_BG_zoom.fits", zoomData, zoomWidth, zoomHeight);
    tempImg->writeImage("final_BG_zoom.fits");
    delete tempImg;

    // Building the MaxTree
	MaxTree *tree = new MaxTree(img);
	tree->flood();

    // Object detection.
	Detector *detect = new Detector(img, tree);
	detect->starDetection();

    Node *nodes = tree->nodes();
    long *ids = detect->ids();

    delete img;
    img = new Image(static_cast<char*>(argv[1]));
    data = img->data();

    for (int y = 0; y < img->height(); y++) {
        if (ids[y * img->width()] >= 0) {
            ids[y * img->width()] = -1;
            data[y * img->width()] = 0.0f;
        }

        if (ids[(y + 1) * img->width() - 1] >= 0) {
            ids[(y + 1) * img->width() - 1] = -1;
            data[(y + 1) * img->width() - 1] = 0.0f;
        }
    }

    for (int x = 0; x < img->width(); x++) {
        if (ids[x] >= 0) {
            ids[x] = -1;
            data[x] = 0.0f;
        }

        if (ids[(img->height() - 1) * img->width() + x] >= 0) {
            ids[(img->height() - 1) * img->width() + x] = -1;
            data[(img->height() - 1) * img->width() + x] = 0.0f;
        }
    }


    TimerWrapper::TimerInstance()->startTimer();
    for (long i = 0; i < img->size(); i++) {
        if (ids[i] >= 0) {
            data[i] = 0.0f;
        }
    }
    TimerWrapper::TimerInstance()->stopTimer("Removal");
    zoomData.clear();
    for (int y = 0; y < zoomHeight; y++) {
        for (int x = 0; x < zoomWidth; x++) {
            zoomData.push_back(data[(y + zoomY) * img->width() + (x + zoomX)]);
        }
    }
    img->writeImage("final_removal_noise.fits");
    tempImg = new Image("final_removed_zoom_noise.fits", zoomData, zoomWidth, zoomHeight);
    tempImg->writeImage("final_removed_zoom_noise.fits");
    delete tempImg;


    TimerWrapper::TimerInstance()->startTimer();
    for (long i = 0; i < img->size(); i++) {
        if (ids[i] >= 0) {
            data[i] = data[ids[i]];
        }
    }
    TimerWrapper::TimerInstance()->stopTimer("Hierarchy");
    zoomData.clear();
    for (int y = 0; y < zoomHeight; y++) {
        for (int x = 0; x < zoomWidth; x++) {
            zoomData.push_back(data[(y + zoomY) * img->width() + (x + zoomX)]);
        }
    }
    img->writeImage("final_hierarchy_noise.fits");
    tempImg = new Image("final_hierarchy_zoom_noise.fits", zoomData, zoomWidth, zoomHeight);
    tempImg->writeImage("final_hierarchy_zoom_noise.fits");
    delete tempImg;

    TimerWrapper::TimerInstance()->startTimer();
    interpolate(img, ids);
    TimerWrapper::TimerInstance()->stopTimer("Interpolation");
    zoomData.clear();
    for (int y = 0; y < zoomHeight; y++) {
        for (int x = 0; x < zoomWidth; x++) {
            zoomData.push_back(data[(y + zoomY) * img->width() + (x + zoomX)]);
        }
    }
    img->writeImage("final_interpolate_noise.fits");
    tempImg = new Image("final_interpolate_zoom_noise.fits", zoomData, zoomWidth, zoomHeight);
    tempImg->writeImage("final_interpolate_zoom_noise.fits");
    delete tempImg;

    TimerWrapper::TimerInstance()->startTimer();
    long *zoomIds = new long[zoomWidth * zoomHeight];
    zoomData.clear();
    for (int y = 0; y < zoomHeight; y++) {
        for (int x = 0; x < zoomWidth; x++) {
            zoomData.push_back(data[(y + zoomY) * img->width() + (x + zoomX)]);
            zoomIds[y * zoomWidth + x] = ids[(y + zoomY) * img->width() + (x + zoomX)];
        }
    }
    tempImg = new Image("final_NS_zoom_noise.fits", zoomData, zoomWidth, zoomHeight);
    NSInpaint(tempImg, zoomIds, true);
    TimerWrapper::TimerInstance()->stopTimer("Navier");
    tempImg->writeImage("final_NS_zoom_noise.fits");
    delete tempImg;
    delete [] zoomIds;

	delete img;
    delete tree;
    delete detect;

    TimerWrapper::TimerInstance()->stopTimer("The full program");
	TimerWrapper::TimerInstance()->cleanTimer();

	return 0;
}
