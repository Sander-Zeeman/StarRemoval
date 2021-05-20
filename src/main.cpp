#include "../include/Clusterer.h"
#include "../include/Debug.h"
#include "../include/Image.h"
#include "../include/MaxTree.h"
#include "../include/Detector.h"
#include "../include/Point.h"

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

    // Building the MaxTree
	MaxTree *tree = new MaxTree(img);
	tree->flood();

    // Object detection.
	Detector *detect = new Detector(tree);
	detect->objectDetection();

    TimerWrapper::TimerInstance()->startTimer();
    // Creating points for clustering.
    Node *nodes = tree->nodes();
    std::vector<Point> points;

    for (int y = 0; y < img->height(); y++) {
        for (int x = 0; x < img->width(); x++) {
            long idx = y * img->width() + x;
            if (!nodes[idx].isSignificant())
                continue;

            Point p(static_cast<float>(nodes[idx].area()), img->data()[idx]);
            points.push_back(p);
        }
    }
    TimerWrapper::TimerInstance()->stopTimer("Creating points for clustering");

    Clusterer *clusterer = new Clusterer(points);
    clusterer->cluster();

    // Writing the modified image.
    img->writeImage();

    delete clusterer;
	delete detect;
	delete tree;
	delete img;

    TimerWrapper::TimerInstance()->stopTimer("The full program");
	return 0;
}
