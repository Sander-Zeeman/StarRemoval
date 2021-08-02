#ifndef IMAGE_H
#define IMAGE_H

#include "Debug.h"

#ifdef CFITSIO
    #include <fitsio.h>
#endif

#include <cstring>
#include <iomanip>
#include <vector>

typedef struct {
    float mean, var, gain;
} imgStats;

class Image
{
public:
	Image(const char *filename);
    Image(std::vector<float> data, int width, int height);
	~Image();

    void estimateBackground();

    Image *subsection(int zoomX = 2874, int zoomY = 1867, int zoomWidth = 800, int zoomHeight = 800);
    void stretch(float min, float max);

    void write(const char *filename);
    void writeSection(const char *filename);

	float *data() { return m_data; }
	int width() { return m_width; }
	int height() { return m_height; }
	long size() { return m_size; }
    imgStats stats() { return m_stats; }

private:
    float enforceBounds(float value);
	void read(const char *filename);

	#ifdef CFITSIO
	    void printError(int status);
	    void readFitsImage(const char *filename);
        void writeFitsImage(const char *filename);
    #endif

	float *m_data = nullptr;
	int m_width = 0;
	int m_height = 0;
	long m_size = 0;
    imgStats m_stats;
};

#endif
