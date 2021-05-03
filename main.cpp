#include <iostream>
#include "include/Image.h"
#include "include/MaxTree.h"

int main(int argc, char *argv[])
{
	Image *img = new Image(argv[1]);
	std::cout << "Width: " << img->width() << ", Height: " << img->height() << ", Size: " << img->size() << std::endl;
	MaxTree *tree = new MaxTree(img);
	tree->flood();
	delete tree;
	return 0;
}
