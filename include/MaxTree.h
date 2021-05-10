#ifndef MAXTREE_H
#define MAXTREE_H

#include <stack>

#include "Connectivity.h"
#include "Debug.h"
#include "Heap.h"
#include "Image.h"
#include "Node.h"
#include "Pixel.h"

class MaxTree
{

public:
	MaxTree(Image *img);
	~MaxTree();

	Image *img() { return m_img; }
	Node *nodes() { return m_nodes; }
	void flood();

private:
	void initialize();
	Pixel findStart();
	bool queueNeighbour(float val, int x, int y);
	void queueNeighbours(Pixel pixel);
	void mergeNodes(long toIndex, long fromIndex);
	void descend(Pixel pixel);
	void finishStack();

	Image *m_img = nullptr;
	Node *m_root = nullptr;
	Node *m_nodes = nullptr;

	std::stack<Pixel> m_stack;
	Heap *m_heap = nullptr;
	Connectivity *m_connectivity = nullptr;
};

#endif
