#ifndef POST_H
#define POST_H

#include "Debug.h"
#include "Image.h"
#include "Node.h"

#include <opencv2/photo.hpp>

void fixEdges(Image *img, long *ids);
bool *createMask(long *ids, long size);

void remove(Image *img, long *ids);

void hierarchicalInpaint(Image *img, long *ids, long *closestSigAncestors, Node *nodes);

void findLimits(bool *mask, int width, int x, int y, int & lowX, int & highX, int & lowY, int & highY);
float interpolate2D(int idx, int low, float dataLow, int high, float dataHigh);
float interpolate(float *data, int width, int x, int y, int lowX, int highX, int lowY, int highY);
void interpolateInpaint(Image *img, long *ids);

void NSInpaint(Image *img, long *ids, int radius = 5);

#endif
