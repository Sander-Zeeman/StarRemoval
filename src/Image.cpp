#include "../include/Image.h"

Image::Image(char *filename)
	: m_name(filename)
{
    #ifdef TIME
        Timer *t = new Timer();
    #endif

	readImage();

	#ifdef TIME
        std::cout << "Reading the image took ";
        delete t;
        std::cout << std::endl << std::endl;
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

	m_data[0] = 0.0;
	m_data[1] = 0.0;
	m_data[2] = 0.0;
	m_data[3] = 0.2;
	m_data[4] = 0.2;
	m_data[5] = 0.2;

	m_data[6] = 0.0;
	m_data[7] = 0.0;
	m_data[8] = 0.2;
	m_data[9] = 0.2;
	m_data[10] = 0.4;
	m_data[11] = 0.2;

	m_data[12] = 0.0;
	m_data[13] = 0.2;
	m_data[14] = 0.2;
	m_data[15] = 0.4;
	m_data[16] = 0.6;
	m_data[17] = 0.4;

	m_data[18] = 0.0;
	m_data[19] = 0.0;
	m_data[20] = 0.2;
	m_data[21] = 0.2;
	m_data[22] = 0.4;
	m_data[23] = 0.2;

	m_data[24] = 0.0;
	m_data[25] = 0.0;
	m_data[26] = 0.0;
	m_data[27] = 0.2;
	m_data[28] = 0.2;
	m_data[29] = 0.2;

	m_data[30] = 0.0;
	m_data[31] = 0.0;
	m_data[32] = 0.0;
	m_data[33] = 0.0;
	m_data[34] = 0.2;
	m_data[35] = 0.0;

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

#ifdef OPENCV
void Image::show()
{
    cv::Mat image(m_height, m_width, CV_32F, m_data);
    cv::imshow("Image", image);
    cv::waitKey(0);
}
#endif

#ifdef CFITSIO
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
    long naxis = 2;
    long naxes[2] = {m_width, m_height};

    remove(filename);

    if (fits_create_file(&fptr, filename, &status))
        printError(status);

    if (fits_create_img(fptr, bitpix, naxis, naxes, &status))
        printError(status);

    if (fits_write_img(fptr, TFLOAT, 1, m_size, m_data, &status))
        printError(status);

    if (fits_close_file(fptr, &status))
        printError(status);
}
#endif

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


#ifdef CFITSIO
void Image::printError(int status)
{
	if (status) {
		fits_report_error(stderr, status);
		exit(status);
	}
	return;
}
#endif

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
        #ifdef OPENCV
            readCVImage();
        #else
            std::cout << "OPENCV not linked, yet the given file is not a fits or debug file." << std::endl;
            exit(1);
        #endif
	}
}

#ifdef OPENCV
void Image::readCVImage()
{
    cv::Mat image =
    cv::imread("/home/sander/Documents/Projects/StarRemoval/testImg.png");

    m_width = image.cols;
    m_height = image.rows;
    m_size = image.total();
    m_data = new float[m_size];

    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m_width; j++) {
            m_data[i * m_width + j] = image.at<float>(i * m_width + j);
        }
    }

    cv::Mat newImg(m_height, m_width, CV_32F, image.data);

	#ifdef DEBUG
    	std::cout << "Image used: " << std::endl;
    	std::cout << std::setprecision(1) << std::fixed;
    	for (int y = 0; y < m_height; y++) {
    		for (int x = 0; x < m_width; x++) {
    			std::cout << m_data[m_width * y + x] << " ";
    		}
    		std::cout << std::endl;
    	}
    	std::cout << std::endl;
	#endif

    cv::imshow("Image", newImg);
    cv::waitKey(0);
}
#endif

#ifdef CFITSIO
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

	m_width = naxes[0];
	m_height = naxes[1];

	m_size  = m_width * m_height;
	m_data = new float[m_size];

	if ( fits_read_img(fptr, TFLOAT, 1, m_size, &nullval, m_data, &anynull, &status) )
		printError( status );

	if ( fits_close_file(fptr, &status) )
		printError( status );

	return;
}
#endif