#ifndef IMAGE_H
#define IMAGE_H

#include <fitsio.h>
#include <stdio.h>
#include <iostream>
#include <cstring>

namespace SR {

class Image {

public:
	Image(const char *filename);
	~Image();

	float *get_img() { return img; }
	int get_width() { return width; }
	int get_height() { return height; }

private:
	const char *name = nullptr;
	float *img = nullptr;
	int width = 0;
	int height = 0;

	void printError(int status);
	void readImage();
	void readFitsImage();
	void readStandardImage();

};

}

#endif
