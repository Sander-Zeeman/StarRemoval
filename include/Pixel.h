#ifndef PIXEL_H
#define PIXEL_H

class Pixel
{
public:
	Pixel();
	Pixel(float val, int x, int y);
	~Pixel();

	float val() { return m_val; }
	int x() { return m_x; }
	int y() { return m_y; }
	long index(int width) { return m_y * width + m_x; }

	void setVal(float val) { m_val = val; }
	void setLocation(long index, int width) {
		m_x = index % width;
		m_y = index / width;
	}

private:
	float m_val = -1.0;
	int m_x = 0;
	int m_y = 0;
};

#endif
