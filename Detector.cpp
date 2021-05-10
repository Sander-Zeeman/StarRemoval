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

}

void Detector::findObjects()
{

}

void Detector::markIDs()
{

}

void Detector::objectDetection()
{

}
