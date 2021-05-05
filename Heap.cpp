#include "include/Heap.h"

Heap::Heap(long size)
	: m_maxSize(size + 1)
{
	m_entries = new Pixel[size + 1];
}

Heap::~Heap()
{
	delete [] m_entries;
}

void Heap::swapEntries(long a, long b) {
	Pixel tmp = m_entries[a];
	m_entries[a] = m_entries[b];
	m_entries[b] = tmp;
}

void Heap::insert(Pixel pixel) {
	m_entries[++m_currSize] = pixel;
	long index = m_currSize;
	while (m_entries[index].val() > m_entries[index / 2].val()) {
		swapEntries(index, index / 2);
		index /= 2;
	}
}

void Heap::maxHeapify(long pos) {
	if (pos > (m_currSize / 2) && pos <= m_currSize)
		return;

	if (m_entries[pos].val() < m_entries[2 * pos].val()
		|| m_entries[pos].val() < m_entries[2 * pos + 1].val()) {
		if (m_entries[2 * pos].val() > m_entries[2 * pos + 1].val()) {
			swapEntries(pos, 2 * pos);
			maxHeapify(2 * pos);
		} else {
			swapEntries(pos, 2 * pos + 1);
			maxHeapify(2 * pos + 1);
		}
	}
}

Pixel Heap::remove()
{

	Pixel root = top();
	m_entries[1] = m_entries[m_currSize--];
	if (m_currSize > 0)
		maxHeapify(1);
	return root;
}
