#ifndef DETECT_H
#define DETECT_H

#include "Debug.h"
#include "Heap.h"
#include "Image.h"
#include "MaxTree.h"

#include <vector>
#include <algorithm>

class Detector {
public:
	Detector(Image *img, MaxTree *tree);
	~Detector();

    std::vector<long> relevantIndices() { return m_relevantIndices; }
    long *mainBranch() { return m_mainBranches; }

	void objectDetection();

private:
	void findRelevantNodes();
    void updateMainBranch(long idx);
    float noiseVariance(long idx);
    float powerDef(long idx);
    bool sigTest(long idx);
	void findSignificantNodes();
	void findObjects();
	void markIDs();

	MaxTree *m_tree = nullptr;
    Image *m_img = nullptr;
	std::vector<long> m_relevantIndices;
	long *m_closestSigAncestors = nullptr;
	long *m_mainBranches = nullptr;
	long *m_objectIDs = nullptr;
	long m_significantNodeCount = 0;
	long m_objectCount = 0;
};

#endif
