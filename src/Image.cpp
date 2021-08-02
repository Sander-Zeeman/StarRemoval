#include "../include/BGFunctions.h"
#include "../include/Image.h"

// Reading image data from a file.
Image::Image(const char *filename)
{
	read(filename);
}

// Creating an Image object from data.
Image::Image(std::vector<float> data, int width, int height)
    : m_width(width)
    , m_height(height)
    , m_size(width * height)
{
    m_data = new float[m_size];
	for (long i = 0; i < m_size; i++)
        m_data[i] = data[i];
}

Image::~Image()
{
	delete [] m_data;
}

float Image::enforceBounds(float value)
{
    if (value > 1.0f)
        return 1.0f;
    if (value < 0.0f)
        return 0.0f;
    return value;
}

void Image::estimateBackground()
{
    // Retrieving the mean, variance and gain of this image.
    estimateBG(this, m_stats);

    // Subtracting the mean to reduce background noise, followed by normalization.
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            float newVal = (m_data[y * m_width + x] - m_stats.mean) / (1 - m_stats.mean);
            m_data[y * m_width + x] = enforceBounds(newVal);
        }
    }
}

void Image::stretch(float min, float max)
{
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            float newVal = (m_data[y * m_width + x] - min) / (max - min);
            m_data[y * m_width + x] = enforceBounds(newVal);
        }
    }
}

void Image::write(const char *filename)
{
    #ifdef TIMED
        TimerWrapper::TimerInstance()->startTimer();
    #endif

    // Find the extension of the given filename, so we know how to write it.
    const char *extension = strrchr(filename, '.');

	if (!extension) {
		std::cout << "ERROR: No file extension found" << std::endl;
		exit(1);
	}

	if (strcmp(extension, ".fits") == 0) {
		#ifdef CFITSIO
		    writeFitsImage(filename);
		#endif
	} else {
        std::cout << "The filetype: " << extension << " is not currently supported." << std::endl;
        exit(1);
	}

    #ifdef DEBUG
        std::cout << "Wrote image to: " << filename << std::endl;
        std::cout << "Image stats:" << std::endl;
        std::cout << "  Width: " << m_width << std::endl;
        std::cout << "  Height: " << m_height << std::endl;
        std::cout << "  Size: " << m_size << std::endl;
    #endif

    #ifdef TIMED
        TimerWrapper::TimerInstance()->stopTimer("Writing the image");
    #endif
}

Image *Image::subsection(int zoomX, int zoomY, int zoomWidth, int zoomHeight)
{
    std::vector<float> zoomData;

    for (int y = 0; y < zoomHeight; y++)
        for (int x = 0; x < zoomWidth; x++)
            zoomData.push_back(m_data[(y + zoomY) * m_width + (x + zoomX)]);

    return new Image(zoomData, zoomWidth, zoomHeight);
}

// Writing a subsection of an image.
void Image::writeSection(const char *filename)
{
    Image *zoomedImg = subsection();
    zoomedImg->write(filename);
    delete zoomedImg;
}

// Reading an image from a file.
void Image::read(const char *filename)
{
    #ifdef TIMED
        TimerWrapper::TimerInstance()->startTimer();
    #endif

	// Find the extension of the given filename, so we know how to read it.
	const char *extension = strrchr(filename, '.');

	if (!extension) {
		std::cout << "ERROR: No file extension found" << std::endl;
		exit(1);
	}

	if (strcmp(extension, ".fits") == 0) {
		// File is a FITS file.
		#ifdef CFITSIO
		    readFitsImage(filename);
		#else
            std::cout << "CFITSIO is not enabled, yet a fits file was entered." << std::endl;
            exit(1);
		#endif
	} else {
        std::cout << "The filetype: " << extension << " is not currently supported." << std::endl;
        exit(1);
    }

    #ifdef DEBUG
        std::cout << "Read image from: " << filename << std::endl;
        std::cout << "Image stats:" << std::endl;
        std::cout << "  Width: " << m_width << std::endl;
        std::cout << "  Height: " << m_height << std::endl;
        std::cout << "  Size: " << m_size << std::endl;
    #endif

    #ifdef TIMED
        TimerWrapper::TimerInstance()->stopTimer("Reading the image");
    #endif
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

void Image::writeFitsImage(const char *filename)
{
    fitsfile *fptr;
    int status = 0;
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

void Image::readFitsImage(const char *filename)
{
	fitsfile *fptr;
	int status = 0;
	int nfound, anynull;
	long naxes[2];
	float nullval = 0;

	if ( fits_open_file(&fptr, filename, READONLY, &status) )
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
}

#endif
