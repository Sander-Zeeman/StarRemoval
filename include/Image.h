#ifndef IMAGE_H
#define IMAGE_H

#ifdef CFITSIO
    #include <fitsio.h>
#endif

#include <cstring>
#include <iomanip>

#include "Debug.h"

class Image
{

public:
	Image(const char *filename);
	~Image();

	float *data() { return m_data; }
	int width() { return m_width; }
	int height() { return m_height; }
	long size() { return m_size; }

private:
    void debugImage();
	void readImage();
	#ifdef CFITSIO
	    void printError(int status);
	    void readFitsImage();
    #endif

	const char *m_name = nullptr;
	float *m_data = nullptr;
	int m_width = 0;
	int m_height = 0;
	long m_size = 0;

};

#endif
