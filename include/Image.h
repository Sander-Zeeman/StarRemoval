#ifndef IMAGE_H
#define IMAGE_H


#include "Debug.h"


#ifdef CFITSIO
    #include <fitsio.h>
#endif

#ifdef OPENCV
    #include <opencv2/core.hpp>
    #include <opencv2/highgui.hpp>
    #include <opencv2/imgcodecs.hpp>
    #include <opencv2/imgproc.hpp>
#endif

#include <cstring>
#include <iomanip>

class Image
{

public:
	Image(char *filename);
	~Image();

    #ifdef OPENCV
        void show();
    #endif

    void writeImage();

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
        void writeFitsImage();
    #endif

    #ifdef OPENCV
        void readCVImage();
    #endif

	char *m_name = nullptr;
	char* m_extension = nullptr;
	float *m_data = nullptr;
	int m_width = 0;
	int m_height = 0;
	long m_size = 0;

};

#endif
