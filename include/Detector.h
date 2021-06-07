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
    long *ids() { return m_objectIDs; }
    long *mainBranch() { return m_mainBranches; }

	void starDetection();

private:
	void findRelevantNodes();
    void updateMainBranch(long idx);
    float noiseVariance(long idx);
    float powerDef(long idx);
    bool sigTest(long idx);
	void findSignificantNodes();
	void findObjects();
    float findTop(Node *nodes, long idx);
    void findStars();
	void markIDs();

	MaxTree *m_tree = nullptr;
    Image *m_img = nullptr;

	std::vector<long> m_relevantIndices;
	long *m_closestSigAncestors = nullptr;
	long *m_mainBranches = nullptr;
	long *m_objectIDs = nullptr;

	long m_significantNodeCount = 0;
	long m_objectCount = 0;
    long m_starCount = 0;
};

#endif
