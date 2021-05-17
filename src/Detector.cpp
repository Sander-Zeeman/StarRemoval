#include "../include/Detector.h"

Detector::Detector(MaxTree *tree)
	: m_tree(tree)
{
	long size = tree->img()->size();
	m_closestSigAncestors = new long[size];
    for (long i = 0; i < size; i++) {
        m_closestSigAncestors[i] = NO_PARENT;
    }

	m_mainBranches = new long[size];
	m_objectIDs = new long[size];
}

Detector::~Detector()
{
	delete [] m_closestSigAncestors;
	delete [] m_objectIDs;
}

void Detector::findRelevantNodes()
{
    #ifdef TIME
        Timer *t = new Timer();
    #endif

	Image *img = m_tree->img();
	Heap *heap = new Heap(m_tree->img()->size());

	for (int y = 0; y < img->height(); y++) {
		for (int x = 0; x < img->width(); x++) {
			Pixel pixel = Pixel(img->data()[x + img->width() * y], x, y);
			long index = pixel.index(img->width());
			long parentIndex = m_tree->nodes()[index].parent();

			if ( m_tree->nodes() + index == m_tree->root() || img->data()[index] == img->data()[parentIndex] )
				continue;

			heap->insert(pixel);
		}
	}

	while (!heap->isEmpty())
		m_relevantIndices.push_back(heap->remove().index(img->width()));

    std::reverse(m_relevantIndices.begin(), m_relevantIndices.end());

    #ifdef DEBUG
    	std::cout << m_relevantIndices.size() << " relevant indices found: " << std::endl;
    #endif

    #ifdef TIME
        std::cout << "Detecting relevant indices took: ";
        delete t;
        std::cout << std::endl << std::endl;
    #endif

	delete heap;
}

void Detector::updateMainBranch(long idx)
{
    if (m_closestSigAncestors[idx] == NO_PARENT)
        return;

    Node *nodes = m_tree->nodes();

    long ancestorIdx = m_closestSigAncestors[idx];

    if (nodes[idx].hasSigDescendent()) {
        long currSigDescendentIdx = m_mainBranches[ancestorIdx];
        if (nodes[currSigDescendentIdx].area() < nodes[idx].area()) {
            m_mainBranches[ancestorIdx] = idx;
        }
    } else {
        nodes[ancestorIdx].setHasSigDescendent(true);
        m_mainBranches[ancestorIdx] = idx;
    }
}

void Detector::findSignificantNodes()
{
    #ifdef TIME
        Timer *t = new Timer();
    #endif

    Node *nodes = m_tree->nodes();

    for (unsigned long i = 0; i < m_relevantIndices.size(); i++) {
        long idx = m_relevantIndices[i];
        long parentIdx = nodes[idx].parent();

        if (nodes[parentIdx].isSignificant()) {
            m_closestSigAncestors[idx] = parentIdx;
        } else if (m_closestSigAncestors[parentIdx] != NO_PARENT) {
            m_closestSigAncestors[idx] = m_closestSigAncestors[parentIdx];
        }

        nodes[idx].setSignificant(true);
        m_significantNodeCount++;
        updateMainBranch(parentIdx);
    }

    #ifdef DEBUG
        std::cout << m_significantNodeCount << " significant nodes at: " << std::endl;
    #endif

    #ifdef TIME
        std::cout << "Detecting significant nodes took: ";
        delete t;
        std::cout << std::endl << std::endl;
    #endif
}

void Detector::findObjects()
{
    #ifdef TIME
        Timer *t = new Timer();
    #endif

    Node *nodes = m_tree->nodes();

    for (unsigned long i = 0; i < m_relevantIndices.size(); i++) {
        long idx = m_relevantIndices[i];
        long parentIdx = m_closestSigAncestors[idx];

        if (!nodes[idx].isSignificant())
            continue;

        if (parentIdx == NO_PARENT) {
            nodes[idx].setObject(true);
            m_objectCount++;
            continue;
        }

        if (m_mainBranches[parentIdx] != idx) {
            nodes[idx].setObject(true);
            m_objectCount++;
            continue;
        }
    }

    #ifdef DEBUG
        std::cout << m_objectCount << " objects at nodes: " << std::endl;
    #endif

    #ifdef TIME
        std::cout << "Detecting objects took: ";
        delete t;
        std::cout << std::endl << std::endl;
    #endif
}

void Detector::markIDs()
{
    #ifdef TIME
        Timer *t = new Timer();
    #endif



    #ifdef TIME
        std::cout << "Marking IDs took: ";
        delete t;
        std::cout << std::endl << std::endl;
    #endif
}

void Detector::objectDetection()
{
	findRelevantNodes();
	findSignificantNodes();
	findObjects();
	// Movement?
	markIDs();
}