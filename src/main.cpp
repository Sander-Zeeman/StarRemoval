#include "../include/Debug.h"
#include "../include/Image.h"
#include "../include/MaxTree.h"
#include "../include/Detector.h"
#include <iomanip>

int main(int argc, char *argv[])
{
	Image *img = new Image(argv[1]);

	#ifdef DEBUG
	std::cout << "Width: " << img->width() << ", Height: " << img->height() << ", Size: " << img->size() << std::endl << std::endl;
	#endif

	MaxTree *tree = new MaxTree(img);
	tree->flood();

	#ifdef DEBUG
	std::cout << "Node's (parent, area, volume, power) after setting up tree." << std::endl;
	for (int y = 0; y < img->height(); y++) {
		for (int x = 0; x < img->width(); x++) {
			std::cout << " (" << std::setw(4) << tree->nodes()[y * img->width() + x].parent() \
			          << ", " << std::setw(4) << tree->nodes()[y * img->width() + x].area()   \
			          << ", " << std::setw(4) << tree->nodes()[y * img->width() + x].volume() \
			          << ", " << std::setw(4) << tree->nodes()[y * img->width() + x].power()  \
			          << ") ";
		}
		std::cout << std::endl;
	}
    std::cout << std::endl;
	#endif


	Detector *detect = new Detector(tree);
	detect->objectDetection();

	delete detect;
	delete tree;
	delete img;
	return 0;
}
