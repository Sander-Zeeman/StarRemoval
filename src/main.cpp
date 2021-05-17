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

long findFirstNonSig(Node *nodes, long *cache, long idx)
{
    if (nodes[idx].parent() == NO_PARENT || !nodes[idx].isSignificant())
        return idx;

    if (cache[idx] != -1)
        return cache[idx];

    long parentIdx = nodes[idx].parent();
    cache[idx] = findFirstNonSig(nodes, cache, parentIdx);

    return cache[idx];
}

int main(int argc, char *argv[])
{
    // Reading the image
    Image *img = new Image((char*)argv[1]);

	#ifdef DEBUG
    	std::cout << "Width: " << img->width() << ", Height: " << img->height() << ", Size: " << img->size() << std::endl << std::endl;
    #endif

    // Building the MaxTree
	MaxTree *tree = new MaxTree(img);
	tree->flood();

    // Object detection.
	Detector *detect = new Detector(tree);
	detect->objectDetection();

    #ifdef TIME
        Timer *t = new Timer();
    #endif
    long *cache = new long[img->size()];
    for (long i = 0; i < img->size(); i++) {
        cache[i] = -1;
    }

    Node *nodes = tree->nodes();
    for (int y = 0; y < img->height(); y++) {
        for (int x = 0; x < img->width(); x++) {
            if (nodes[y * img->width() + x].isSignificant() && img->data()[y * img->width() + x] > 0.5) {
                long idx = y * img->width() + x;
                std::cout << idx << std::endl;
                img->data()[idx] = img->data()[findFirstNonSig(nodes, cache, idx)];
            }
        }
    }
    delete [] cache;

    img->writeImage();
    #ifdef TIME
        std::cout << "Modifying output image took ";
        delete t;
        std::cout << std::endl << std::endl;
    #endif

	delete detect;
	delete tree;
	delete img;
	return 0;
}
