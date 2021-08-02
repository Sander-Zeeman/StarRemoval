#include "../include/Heap.h"

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
    long parent = index >> 1;
	while (index != 1 && m_entries[index].val() > m_entries[parent].val()) {
		swapEntries(index, parent);
		index = parent;
        parent = index >> 1;
	}
}

void Heap::maxHeapify(long pos) {
	if (pos > (m_currSize / 2) && pos <= m_currSize)
		return;

    long lChild = pos << 1;
    long rChild = lChild + 1;
    if (
        m_entries[pos].val() < m_entries[lChild].val() ||
        m_entries[pos].val() < m_entries[rChild].val()
    ) {
        if (m_entries[lChild].val() > m_entries[rChild].val()) {
		    swapEntries(pos, lChild);
			maxHeapify(lChild);
		} else {
			swapEntries(pos, rChild);
			maxHeapify(rChild);
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
