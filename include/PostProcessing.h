#ifndef POST_H
#define POST_H

#include "Debug.h"
#include "Node.h"

int findLimit(float *data, int width, int height, int x, int y, bool vertical, bool increasing);
void findLimits(float *data, int width, int height, int x, int y, int & lowX, int & highX, int & lowY, int & highY);
float interpolate2D(int idx, int low, float dataLow, int high, float dataHigh);
float interpolate(float *data, int width, int height, int x, int y, int lowX, int highX, int lowY, int highY);

#endif
