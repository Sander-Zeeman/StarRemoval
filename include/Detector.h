#ifndef DETECT_H
#define DETECT_H

#include "Debug.h"
#include "Heap.h"
#include "MaxTree.h"

#include <vector>

class Detector {

public:
	Detector(MaxTree *tree);
	~Detector();

	void objectDetection();

private:
	void findRelevantNodes();
	void findSignificantNodes();
	void findObjects();
	void markIDs();

	MaxTree *m_tree = nullptr;
	std::vector<long> m_relevantIndices;
	long *m_closestSigAncestors = nullptr;
	long *m_mainBranches = nullptr;
	long *m_mainPowerBranches = nullptr;
	long *m_objectIDs = nullptr;
	long m_significantNodeCount = 0;
	long m_objectCount = 0;

};

#endif