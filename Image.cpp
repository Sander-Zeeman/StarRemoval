#include "include/Image.h"

Image::Image(const char *filename)
	: m_name(filename)
{
	readImage();
}

Image::~Image()
{
	if (m_data)
		delete [] m_data;
}

void Image::printError(int status)
{
	if (status) {
		fits_report_error(stderr, status);
		exit(status);
	}
	return;
}

void Image::readImage()
{
	// Find the extension of the given filename, so we know how to read it.
	const char *extension;
	extension = strrchr(m_name, '.');

	// No extension was found.
	if (!extension) {
		std::cout << "ERROR: No file extension found" << std::endl;
		exit(1);
	}

	if (strcmp(extension, ".fits") == 0) {
		// File is a FITS file.
		readFitsImage();
	} else {
		// File has another extension, to be handled by OpenCV later.
		std::cout << "Standard file read, to be handled." << std::endl;
	}
}

void Image::readFitsImage()
{
	fitsfile *fptr;
	int status = 0;
	int nfound, anynull;
	long npixels, fpixel = 1, naxes[2];
	float nullval = 0;

	if ( fits_open_file(&fptr, m_name, READONLY, &status) )
		printError( status );

	if ( fits_read_keys_lng(fptr, "NAXIS", 1, 2, naxes, &nfound, &status) )
		printError( status );

	m_width = naxes[0];
	m_height = naxes[1];

	m_size  = m_width * m_height;
	m_data = new float[m_size];

	if ( fits_read_img(fptr, TFLOAT, fpixel, m_size, &nullval, m_data, &anynull, &status) )
		printError( status );

	if ( fits_close_file(fptr, &status) )
		printError( status );

	return;
}
