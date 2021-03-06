#ifndef PIXEL_H
#define PIXEL_H

#include "Debug.h"

class Pixel
{
public:
	Pixel() {};
    Pixel(int x, int y) : m_x(x), m_y(y) {};
    Pixel(float val, int x, int y) : m_val(val), m_x(x), m_y(y) {};
	~Pixel() {};

	float val() { return m_val; }
	int x() { return m_x; }
	int y() { return m_y; }
	long index(int width) { return static_cast<long>(m_y * width + m_x); }

	void setVal(float val) { m_val = val; }
	void setLocation(long index, int width) {
		m_x = static_cast<int>(index % width);
		m_y = static_cast<int>(index / width);
	}

private:
	float m_val = -1.0f;
	int m_x = 0;
	int m_y = 0;
};

#endif
