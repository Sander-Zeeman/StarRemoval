#include "../include/Debug.h"
#include "../include/Image.h"
#include "../include/MaxTree.h"
#include "../include/Detector.h"

#include <iomanip>

void printNode(MaxTree *tree, long idx)
{
    std::cout << std::setw(6) << idx \
              << " | " << std::setw(6) << tree->nodes()[idx].parent() \
              << " | " << std::setw(6) << tree->nodes()[idx].area()   \
	          << " | " << std::setw(5) << tree->nodes()[idx].height() \
	          << " | " << std::setw(5) << tree->nodes()[idx].volume() \
	          << " | " << std::setw(5) << tree->nodes()[idx].power()  \
	          << std::endl;
}

int main(int argc, char *argv[])
{
    #ifdef TIME
        Timer *t;
    #endif


    // Reading the image
	#ifdef TIME
        t = new Timer();
    #endif

    Image *img = new Image((char*)argv[1]);

	#ifdef DEBUG
    	std::cout << "Width: " << img->width() << ", Height: " << img->height() << ", Size: " << img->size() << std::endl << std::endl;
    #endif

	#ifdef TIME
    	std::cout << "Reading the image took: ";
        delete t;
        std::cout << std::endl << std::endl;
	#endif

    // Building the MaxTree
	MaxTree *tree = new MaxTree(img);
	tree->flood();

    // Object detection.
    #ifdef TIME
        t = new Timer();
    #endif

	Detector *detect = new Detector(tree);
	detect->objectDetection();

    #ifdef TIME
	    std::cout << "Object detection took: ";
        delete t;
        std::cout << std::endl << std::endl;
	#endif

    for (int y = 0; y < img->height(); y++) {
        for (int x = 0; x < img->width(); x++) {
            if (tree->nodes()[y * img->width() + x].isSignificant()) {
                img->data()[y * img->width() + x] = 0;
            }
        }
    }
    img->writeImage();

	delete detect;
	delete tree;
	delete img;
	return 0;
}
