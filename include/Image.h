#ifndef IMAGE_H
#define IMAGE_H

#include "Debug.h"

#ifdef CFITSIO
    #include <fitsio.h>
#endif

#include <cstring>
#include <iomanip>

typedef struct {
    float mean, var, gain;
} imgStats;

class Image
{
public:
	Image(char *filename);
	~Image();

    void estimateBackground();
    void subImage(int lowX, int highX, int lowY, int highY);
    void writeImage();

	float *data() { return m_data; }
	int width() { return m_width; }
	int height() { return m_height; }
	long size() { return m_size; }
    imgStats stats() { return m_stats; }

private:
    void debugImage();
	void readImage();

    void findPixelBounds(float & min, float & max);
    void stretchContrast();

	#ifdef CFITSIO
	    void printError(int status);
	    void readFitsImage();
        void writeFitsImage();
    #endif

	char *m_name = nullptr;
	char* m_extension = nullptr;
	float *m_data = nullptr;
	int m_width = 0;
	int m_height = 0;
	long m_size = 0;
    imgStats m_stats;
};

#endif
