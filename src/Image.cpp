#include "../include/Image.h"

Image::Image(char *filename)
	: m_name(filename)
{
    TimerWrapper::TimerInstance()->startTimer();
	readImage();
	TimerWrapper::TimerInstance()->stopTimer("Reading the image");
    #ifdef DEBUG
        std::cout << "Width: " << m_width << ", Height: " << m_height << ", Size: " << m_size << std::endl << std::endl;
    #endif

}

Image::~Image()
{
	delete [] m_data;
}

void Image::debugImage()
{
/*
    The following debug image is used:
        0.0 0.0 0.0 0.2 0.2 0.2
        0.0 0.0 0.2 0.2 0.4 0.2
        0.0 0.2 0.2 0.4 0.6 0.4
        0.0 0.0 0.2 0.2 0.4 0.2
        0.0 0.0 0.0 0.2 0.2 0.2
        0.0 0.0 0.0 0.0 0.2 0.0
*/

	m_width = 6;
	m_height = 6;
	m_size = 36;

	m_data = new float[36];

	m_data[0] = 0.0f;
	m_data[1] = 0.0f;
	m_data[2] = 0.0f;
	m_data[3] = 0.2f;
	m_data[4] = 0.2f;
    m_data[5] = 0.2f;

	m_data[6] = 0.0f;
	m_data[7] = 0.0f;
	m_data[8] = 0.2f;
	m_data[9] = 0.2f;
	m_data[10] = 0.4f;
	m_data[11] = 0.2f;

	m_data[12] = 0.0f;
	m_data[13] = 0.2f;
	m_data[14] = 0.2f;
	m_data[15] = 0.4f;
	m_data[16] = 0.6f;
	m_data[17] = 0.4f;

	m_data[18] = 0.0f;
	m_data[19] = 0.0f;
	m_data[20] = 0.2f;
	m_data[21] = 0.2f;
	m_data[22] = 0.4f;
	m_data[23] = 0.2f;

	m_data[24] = 0.0f;
	m_data[25] = 0.0f;
	m_data[26] = 0.0f;
	m_data[27] = 0.2f;
	m_data[28] = 0.2f;
	m_data[29] = 0.2f;

	m_data[30] = 0.0f;
	m_data[31] = 0.0f;
	m_data[32] = 0.0f;
	m_data[33] = 0.0f;
	m_data[34] = 0.2f;
	m_data[35] = 0.0f;

	#ifdef DEBUG
    	std::cout << "Image used: " << std::endl;
    	std::cout << std::setprecision(1) << std::fixed;
    	for (int y = 0; y < 6; y++) {
    		for (int x = 0; x < 6; x++) {
    			std::cout << m_data[6 * y + x] << " ";
    		}
    		std::cout << std::endl;
    	}
    	std::cout << std::endl;
	#endif
}

void Image::writeImage()
{
	if (strcmp(m_extension, ".fits") == 0) {
		#ifdef CFITSIO
		    writeFitsImage();
		#endif
	} else {
        exit(1);
	}
}

void Image::readImage()
{
	// Find the extension of the given filename, so we know how to read it.
	m_extension = strrchr(m_name, '.');

	// No extension was found.
	if (!m_extension) {
		std::cout << "ERROR: No file extension found" << std::endl;
		exit(1);
	}

	if (strcmp(m_extension, ".fits") == 0) {
		// File is a FITS file.
		#ifdef CFITSIO
		    readFitsImage();
		#else
            std::cout << "CFITSIO not linked, yet a fits file was entered." << std::endl;
            exit(1);
		#endif
	} else {
        // File has another format.
        exit(1);
    }
}

#ifdef CFITSIO
void Image::printError(int status)
{
	if (status) {
		fits_report_error(stderr, status);
		exit(status);
	}
	return;
}

void Image::writeFitsImage()
{
    fitsfile *fptr;
    int status = 0;

    char *name = m_name;
    while (*name != '.')
        name++;
    *name = '\0';
    char *filename = strcat(m_name, "_modified.fits");

    int bitpix = FLOAT_IMG;
    long naxes[2] = {m_width, m_height};

    remove(filename);

    if (fits_create_file(&fptr, filename, &status))
        printError(status);

    if (fits_create_img(fptr, bitpix, 2, naxes, &status))
        printError(status);

    if (fits_write_img(fptr, TFLOAT, 1, m_size, m_data, &status))
        printError(status);

    if (fits_close_file(fptr, &status))
        printError(status);
}

void Image::readFitsImage()
{
	fitsfile *fptr;
	int status = 0;
	int nfound, anynull;
	long naxes[2];
	float nullval = 0;

	if ( fits_open_file(&fptr, m_name, READONLY, &status) )
		printError( status );

	if ( fits_read_keys_lng(fptr, "NAXIS", 1, 2, naxes, &nfound, &status) )
		printError( status );

	m_width = static_cast<int>(naxes[0]);
	m_height = static_cast<int>(naxes[1]);

	m_size  = m_width * m_height;
	m_data = new float[m_size];

	if ( fits_read_img(fptr, TFLOAT, 1, m_size, &nullval, m_data, &anynull, &status) )
		printError( status );

	if ( fits_close_file(fptr, &status) )
		printError( status );

	return;
}
#endif
