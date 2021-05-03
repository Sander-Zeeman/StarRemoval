#ifndef HEAP_H
#define HEAP_H

#include "Pixel.h"

class Heap
{

public:
	Heap();
	~Heap();

	bool isEmpty() { return m_currSize == 0; }
	long size() { return m_currSize; }
	void resize();
	void insert(Pixel *pixel);
	Pixel *remove();
	Pixel *top() { return m_entries + 1; }

private:
	Pixel *m_entries = nullptr;
	long m_currSize = 0;
	long m_maxSize = 1024;

};

#endif
