#include "../include/Detector.h"

Detector::Detector(MaxTree *tree)
	: m_tree(tree)
{
	long size = tree->img()->size();
	m_closestSigAncestors = new long[size];
    for (long i = 0; i < size; i++) {
        m_closestSigAncestors[i] = NO_PARENT;
    }
	m_objectIDs = new long[size];
}

Detector::~Detector()
{
	delete [] m_closestSigAncestors;
	delete [] m_objectIDs;
}

void Detector::findRelevantNodes()
{
	Image *img = m_tree->img();
	Heap *heap = new Heap(m_tree->img()->size());

	for (int y = 0; y < img->height(); y++) {
		for (int x = 0; x < img->width(); x++) {
			Pixel pixel = Pixel(img->data()[x + img->width() * y], x, y);
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

	while (!heap->isEmpty()){
		m_relevantIndices.insert(m_relevantIndices.begin(),
		heap->remove().index(img->width()));
}
#ifdef DEBUG
	std::cout << m_relevantIndices.size() << " relevant indices found: ";

    for (auto i : m_relevantIndices) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;
#endif

	delete heap;
}

void Detector::findSignificantNodes()
{
    Node *nodes = m_tree->nodes();

    for (unsigned long i = 0; i < m_relevantIndices.size(); i++) {
        long idx = m_relevantIndices[i];
        long parentIdx = nodes[idx].parent();

        if (nodes[parentIdx].isSignificant()) {
            m_closestSigAncestors[idx] = parentIdx;
        } else if (m_closestSigAncestors[parentIdx] != NO_PARENT) {
            m_closestSigAncestors[idx] = m_closestSigAncestors[parentIdx];
        }

        if (true) {
            nodes[idx].setSignificant(true);
            m_significantNodeCount++;
            //updateMainBranch(parentIdx);
        }
    }

    #ifdef DEBUG
        std::cout << m_significantNodeCount << " significant nodes at: ";

        for (unsigned long i = 0; i < m_relevantIndices.size(); i++) {
            unsigned long idx = m_relevantIndices[i];
            if (m_tree->nodes()[idx].isSignificant()) {
                std::cout << idx << ", ";
            }
        }

        std::cout << std::endl;
    #endif
}

void Detector::findObjects()
{
    Node *nodes = m_tree->nodes();

    for (unsigned long i = 0; i < m_relevantIndices.size(); i++) {
        unsigned long idx = m_relevantIndices[i];
        if (!nodes[idx].isSignificant())
            continue;

        if (m_closestSigAncestors[idx] == NO_PARENT) {
            nodes[idx].setObject(true);
            m_objectCount++;
            continue;
        }

        // Does not yet account for nested objects.
    }

    #ifdef DEBUG
        std::cout << m_objectCount << " objects at nodes: ";

        for (unsigned long i = 0; i < m_relevantIndices.size(); i++) {
            unsigned long idx = m_relevantIndices[i];
            if (m_tree->nodes()[idx].isObject()) {
                std::cout << idx << ", ";
            }
        }

        std::cout << std::endl << std::endl;
    #endif
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
