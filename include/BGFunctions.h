#ifndef BGFUNCTIONS_H
#define BGFUNCTIONS_H

#include "Debug.h"
#include "Image.h"
#include "Pixel.h"

#include <cmath>
#include <vector>

class BGFunctions
{
public:
    BGFunctions(Image *img) : m_img(img) {}
    ~BGFunctions() {}

    float mean() { return m_mean; }
    float variance() { return m_variance; }

    void estimateBG();

private:
    bool testNormality(int xStart, int yStart, int size, float rejectRate);
    bool checkMeanEquality(std::vector<float> first, std::vector<float> second, float rejectRate);
    bool checkMeans(int xStart, int yStart, int size, float rejectRate);
    bool isFlatTile(int xStart, int yStart, int size, float rejectRate);
    bool tilesAvailable(int size, float rejectRate);
    int largestTileSize(float rejectRate);
    void calculateMean(std::vector<Pixel> tiles, int size);
    void calculateVariance(std::vector<Pixel> tiles, int size);
    void estimateStats(std::vector<Pixel> tiles, int size);
    void setStats(int size, float rejectRate);


    Image *m_img = nullptr;

    float m_mean = 0.0f;
    float m_variance = 0.0f;
    float m_gain = 0.0f;
};

#endif
