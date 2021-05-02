#include "include/Image.h"

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

void SR::Image::printError(int status)
{
	if (status) {
		fits_report_error(stderr, status);
		exit(status);
	}
	return;
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
	fitsfile *fptr;
	int status = 0;
	int nfound, anynull;
	long npixels, fpixel = 1, naxes[2];
	float nullval = 0;

	if ( fits_open_file(&fptr, name, READONLY, &status) )
		printError( status );

	if ( fits_read_keys_lng(fptr, "NAXIS", 1, 2, naxes, &nfound, &status) )
		printError( status );

	width = naxes[0];
	height = naxes[1];

	npixels  = width * height;
	img = new float[npixels];

	if ( fits_read_img(fptr, TFLOAT, fpixel, npixels, &nullval, img, &anynull, &status) )
		printError( status );

	if ( fits_close_file(fptr, &status) )
		printError( status );

	return;
}

void SR::Image::readStandardImage()
{
	std::cout << "Standard file read, to be handled." << std::endl;
}
