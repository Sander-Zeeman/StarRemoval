#ifndef BGFUNCTIONS_H
#define BGFUNCTIONS_H

#include "Debug.h"
#include "Image.h"
#include "Pixel.h"

#include <boost/math/distributions/chi_squared.hpp>
#include <boost/math/distributions/students_t.hpp>
#include <boost/math/statistics/univariate_statistics.hpp>
#include <cmath>
#include <vector>

double skewTest(std::vector<float> data);
double kurtosisTest(std::vector<float> data);
double KSquared(std::vector<float> data);
double TTest(std::vector<float> data1, std::vector<float> data2);

bool testNormality(Image *img, int xStart, int yStart, int size, double rejectRate);
bool checkMeanEquality(std::vector<float> first, std::vector<float> second, double rejectRate);
bool checkMeans(Image *img, int xStart, int yStart, int size, double rejectRate);

bool isFlatTile(Image *img, int xStart, int yStart, int size, double rejectRate);
bool tilesAvailable(Image *img, int size, double rejectRate);
int largestTileSize(Image *img, double rejectRate);

std::vector<float> extractData(Image *img, std::vector<Pixel> tiles, int size);
void estimateStats(Image *img, std::vector<Pixel> tiles, imgStats & stats, int size);
void setStats(Image *img, int size, imgStats & stats, double rejectRate);

void estimateBG(Image *img, imgStats & stats);

#endif
