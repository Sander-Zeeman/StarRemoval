#include "include/Heap.h"

Heap::Heap()
{
	m_entries = new Pixel[1024];
}

Heap::~Heap()
{
	delete [] m_entries;
}

void Heap::resize()
{
	m_maxSize *= 2;
	Pixel *temp = new Pixel[m_maxSize];

	for (long i = 0; i < m_currSize; i++) {
		temp[i] = m_entries[i];
	}

	delete [] m_entries;
	m_entries = temp;
}

void Heap::insert(Pixel *pixel)
{
	if (m_maxSize == m_currSize)
		resize();

	long index = m_currSize + 1;
	Pixel *entry = m_entries + index;
	float val = pixel->val();

	while (index != 1) {
		long upIndex = index /= 2;
		Pixel *upEntry = m_entries + upIndex;

		if (upEntry->val() >= val)
			break;

		*entry = *upEntry;
		index = upIndex;
		entry = upEntry;
	}

	*entry = *pixel;
	m_currSize += 1;
}

Pixel *Heap::remove()
{
	long index = 1;
	Pixel *entry = top();
	Pixel *last = m_entries + m_currSize;
	Pixel root = *entry;

	while (true) {
		index *= 2;
		if (index > m_currSize)
			break;

		Pixel *downLeft = m_entries + index;

		if (index < m_currSize) {
			Pixel *downRight = m_entries + index + 1;

			if (downRight->val() > downLeft->val()) {
				downLeft += 1;
				index += 1;
			}
		}

		if (downLeft->val() <= last->val())
			break;

		*entry = *downLeft;
		entry = downLeft;
	}

	*entry = *last;
	*last = root;

	m_currSize -= 1;

	return last;
}
