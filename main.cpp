#include <fitsio.h>
#include <iostream>
#include "include/Image.h"


int main(int argc, char *argv[])
{
	fitsfile *fptr;
	SR::Image *img = new SR::Image(argv[1]);
	float *data = img->get_img();
	std::cout << "Width: " << img->get_width() << ", Height: " << img->get_height() << std::endl;
	for (int i = 0; i < img->get_width() * img->get_height(); i++) {
		if (data[i] > 0.6)
			std::cout << data[i] << std::endl;
	}
	delete img;
	return 0;
}
