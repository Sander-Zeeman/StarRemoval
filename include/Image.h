#ifndef IMAGE_H
#define IMAGE_H

//#include <fitsio.h>
#include <cstring>

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
	void printError(int status);
	void readImage();
	void readFitsImage();

	const char *m_name = nullptr;
	float *m_data = nullptr;
	int m_width = 0;
	int m_height = 0;
	long m_size = 0;

};

#endif
