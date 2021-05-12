#include "include/Detector.h"

Detector::Detector(MaxTree *tree)
	: m_tree(tree)
{
	long size = tree->img()->size();
	m_closestAncestors = new long[size];
	m_objectIDs = new long[size];
}

Detector::~Detector()
{
	delete [] m_closestAncestors;
	delete [] m_objectIDs;
}

void Detector::findRelevantNodes()
{
	
}

void Detector::findSignificantNodes()
{
	Image *img = m_tree->img();
	Heap *heap = new Heap(m_tree->img()->size());

	for (int y = 0; y < img->height(); y++) {
		for (int x = 0; x < img->width(); x++) {
			Pixel pixel = Pixel(-1, x, y);
			long index = pixel.index(img->width());
			long parentIndex = m_tree->nodes()[index].parent();

			if (
				m_tree->nodes() + index == m_tree->root() ||
				img->data()[index] == img->data()[parentIndex]
			) {
				continue;
			}

			heap->insert(pixel);
		}
	}

	while (!heap->isEmpty())
		m_relevantIndices.insert(m_relevantIndices.begin(), heap->remove().index(img->width()));

#ifdef DEBUG
	std::cout << "Number of relevant indices found: " << m_relevantIndices.size() << std::endl;
#endif

	delete heap;
}

void Detector::findObjects()
{

}

void Detector::markIDs()
{

}

void Detector::objectDetection()
{
	findRelevantNodes();
	findSignificantNodes();
	findObjects();
	// Movement?
	markIDs();
}
