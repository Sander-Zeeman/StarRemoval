#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <cstring>

namespace SR {

class Image {

public:
	Image(const char *filename);
	~Image();

private:
	const char *name = nullptr;
	double *img = nullptr;
	short width = 0;
	short height = 0;

	void readImage();
	void readFitsImage();
	void readStandardImage();
};

}

#endif
