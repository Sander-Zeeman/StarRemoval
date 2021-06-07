#include "../include/Clusterer.h"
#include "../include/Debug.h"
#include "../include/Detector.h"
#include "../include/Image.h"
#include "../include/MaxTree.h"
#include "../include/Point.h"
#include "../include/PostProcessing.h"

#include <vector>
#include <iomanip>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Expecting a file as argument!" << std::endl;
        exit(1);
    }

    TimerWrapper::TimerInstance()->startTimer();

    // Reading the image
    Image *img = new Image(static_cast<char*>(argv[1]));
    img->estimateBackground();

    // Building the MaxTree
	MaxTree *tree = new MaxTree(img);
	tree->flood();

    // Object detection.
	Detector *detect = new Detector(img, tree);
	detect->starDetection();

    delete img;

    img = new Image(static_cast<char*>(argv[1]));
    float *data = img->data();
    Node *nodes = tree->nodes();
    long *ids = detect->ids();

    // Few non-objects that are bright

    for (long i = 0; i < img->size(); i++)
        if (ids[i] != NO_OBJECT)
            data[i] = UNASSIGNED;

    TimerWrapper::TimerInstance()->startTimer();

    int lowX, highX, lowY, highY;
    float interpolatedValue;
    for (int i = 0; i < 1; i++) {
        for (int y = 0; y < img->height(); y++) {
            for (int x = 0; x < img->width(); x++) {
                if (ids[y * img->width() + x] != NO_OBJECT) {
                    findLimits(data, img->width(), img->height(), x, y, lowX, highX, lowY, highY);
                    interpolatedValue = interpolate(data, img->width(), img->height(), x, y, lowX, highX, lowY, highY);
                    data[y * img->width() + x] = interpolatedValue;
                }
            }
        }
    }

    TimerWrapper::TimerInstance()->stopTimer("Interpolation");

    // Writing the modified image.
    img->writeImage();

	delete img;
    delete tree;
    delete detect;

    TimerWrapper::TimerInstance()->stopTimer("The full program");
	TimerWrapper::TimerInstance()->cleanTimer();

	return 0;
}
