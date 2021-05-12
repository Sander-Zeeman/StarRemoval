#ifndef HEAP_H
#define HEAP_H

#include "Debug.h"
#include "Pixel.h"

class Heap
{

public:
	Heap(long size);
	~Heap();

	bool isEmpty() { return m_currSize == 0; }
	long size() { return m_currSize; }
	Pixel top() { return m_entries[1]; }

	void insert(Pixel pixel);
	Pixel remove();

private:
	void maxHeapify(long pos);
	void swapEntries(long a, long b);

	Pixel *m_entries = nullptr;
	long m_currSize = 0;
	long m_maxSize = 0;

};

#endif
