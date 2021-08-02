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

const double baseRejectRate = 0.05f;

double skewTest(std::vector<float> data);
double kurtosisTest(std::vector<float> data);
double KSquared(std::vector<float> data);
bool testNormality(Image *img, int xStart, int yStart, int tileSize, double rejectRate);

double TTest(std::vector<float> firstHalf, std::vector<float> secondHalf);
bool checkMeanEquality(std::vector<float> firstHalf, std::vector<float> secondHalf, double rejectRate);
bool checkMeans(Image *img, int xStart, int yStart, int tileSize, double rejectRate);

bool isFlatTile(Image *img, int xStart, int yStart, int tileSize);
bool tilesAvailable(Image *img, int tileSize);
int largestTileSize(Image *img);

std::vector<float> extractData(Image *img, std::vector<Pixel> tiles, int tileSize);
void estimateStats(Image *img, std::vector<Pixel> tiles, int tileSize, imgStats & stats);
void setStats(Image *img, int size, imgStats & stats);

void estimateBG(Image *img, imgStats & stats);

#endif
