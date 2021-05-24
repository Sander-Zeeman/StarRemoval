//#include "matplotlibcpp.h"

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
    img->estimateBG();
/*
    // Building the MaxTree
	MaxTree *tree = new MaxTree(img);
	tree->flood();

    // Object detection.
	Detector *detect = new Detector(img, tree);
	detect->objectDetection();
*/
    // Writing the modified image.
    img->writeImage();

	//delete detect;
	//delete tree;
	delete img;

    TimerWrapper::TimerInstance()->stopTimer("The full program");
	TimerWrapper::TimerInstance()->cleanTimer();
	return 0;
}
