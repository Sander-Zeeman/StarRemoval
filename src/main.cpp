#include "../include/Detector.h"
#include "../include/Image.h"
#include "../include/MaxTree.h"
#include "../include/PostProcessing.h"
#include "../include/Timer.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Expecting a file as argument!" << std::endl;
        exit(1);
    }

    #ifdef TIMED
        TimerWrapper::TimerInstance()->startTimer();
    #endif

    // Reading the image
    Image *img = new Image(static_cast<char*>(argv[1]));

    #ifdef NOISY
    img->write("_regular.fits");
    img->writeSection("_zoomed_regular.fits");
    #endif

    // Remove background noise.
    img->estimateBackground();

    #ifdef NOISELESS
    img->write("_noiseless.fits");
    img->writeSection("_zoomed_noiseless.fits");
    #endif

    // Building the MaxTree
    MaxTree *tree = new MaxTree(img);
    tree->flood();

    // Object detection.
    Detector *detect = new Detector(img, tree);
    detect->starDetection();

    Node *nodes = tree->nodes();
    long *ids = detect->ids();

    fixEdges(img, ids);

    Image *subImg;
    long *subIds;

    #ifdef NOISELESS

    remove(img, ids);
    img->write("_removed.fits");
    img->writeSection("_zoomed_removed.fits");

    hierarchicalInpaint(img, ids, nodes);
    img->write("_hierarchy.fits");
    img->writeSection("_zoomed_hierarchy.fits");

    interpolateInpaint(img, ids);
    img->write("_interpolate.fits");
    img->writeSection("_zoomed_interpolate.fits");

    subImg = img->subsection();
    subIds = new long[800 * 800];
    for (int yOffset = 0; yOffset < 800; yOffset++) {
        for (int xOffset = 0; xOffset < 800; xOffset++) {
            subIds[yOffset * 800 + xOffset] = ids[(1867 + yOffset) * img->width() + (2874 + xOffset)];
        }
    }
    NSInpaint(subImg, subIds);
    subImg->write("_zoomed_navier.fits");
    delete subImg;
    delete [] subIds;

    #endif

    #ifdef NOISY

    delete img;
    img = new Image(static_cast<char*>(argv[1]));

    remove(img, ids);
    img->write("_removed_noisy.fits");
    img->writeSection("_zoomed_removed_noisy.fits");

    hierarchicalInpaint(img, ids, nodes);
    img->write("_hierarchy_noisy.fits");
    img->writeSection("_zoomed_hierarchy_noisy.fits");

    interpolateInpaint(img, ids);
    img->write("_interpolate_noisy.fits");
    img->writeSection("_zoomed_interpolate_noisy.fits");

    subImg = img->subsection();
    subIds = new long[800 * 800];
    for (int yOffset = 0; yOffset < 800; yOffset++) {
        for (int xOffset = 0; xOffset < 800; xOffset++) {
            subIds[yOffset * 800 + xOffset] = ids[(1867 + yOffset) * img->width() + (2874 + xOffset)];
        }
    }
    NSInpaint(subImg, subIds);
    subImg->write("_zoomed_navier_noisy.fits");
    delete subImg;
    delete [] subIds;

    #endif



    delete detect;
    delete tree;
    delete img;

    #ifdef TIMED
        TimerWrapper::TimerInstance()->stopTimer("The full program");
        TimerWrapper::TimerInstance()->cleanTimer();
    #endif

	return 0;
}
