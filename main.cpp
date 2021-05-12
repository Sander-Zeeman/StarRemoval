#include "include/Debug.h"
#include "include/Image.h"
#include "include/MaxTree.h"
#include "include/Detector.h"

//Descend ruins this

int main(int argc, char *argv[])
{
	Image *img = new Image(argv[1]);

	#ifdef DEBUG
	std::cout << "Width: " << img->width() << ", Height: " << img->height() << ", Size: " << img->size() << std::endl;
	#endif

	MaxTree *tree = new MaxTree(img);
	tree->flood();

	#ifdef DEBUG
	std::cout << "Node's parents after setting up tree." << std::endl;
	for (int y = 0; y < img->height(); y++) {
		for (int x = 0; x < img->width(); x++) {
			std::cout << tree->nodes()[y * img->width() + x].parent() << " ";
		}
		std::cout << std::endl;
	}
	#endif

	Detector *detect = new Detector(tree);
	detect->objectDetection();

	delete detect;
	delete tree;
	delete img;
	return 0;
}
