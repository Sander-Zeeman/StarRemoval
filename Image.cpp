#include "Image.h"

SR::Image::Image(const char *filename)
	: name(filename)
{
	readImage();
}

SR::Image::~Image()
{
	if (img)
		delete [] img;
}

void SR::Image::readImage()
{
	// Find the extension of the given filename, so we know how to read it.
	const char *extension;
	extension = strrchr(name, '.');

	// No extension was found.
	if (!extension) {
		std::cout << "ERROR: No file extension found" << std::endl;
		exit(1);
	}

	if (strcmp(extension, ".fits") == 0) {
		// File is a FITS file.
		readFitsImage();
	} else {
		// File has another extension, to be handled by OpenCV
		readStandardImage();
	}
}

void SR::Image::readFitsImage()
{
	std::cout << "FITS file read, to be handled." << std::endl;
}

void SR::Image::readStandardImage()
{
	std::cout << "Standard file read, to be handled." << std::endl;
}
