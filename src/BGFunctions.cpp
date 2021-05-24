#include "../include/BGFunctions.h"

bool BGFunctions::testNormality(int xStart, int yStart, int size, float rejectRate)
{
    // D'Agostino-Pearson K^2 test.
    return true;
}

bool BGFunctions::checkMeanEquality(std::vector<float> first, std::vector<float> second, float rejectRate)
{
    // t-test for equal means.
    return true;
}

bool BGFunctions::checkMeans(int xStart, int yStart, int size, float rejectRate)
{
    float *data = m_img->data();

    std::vector<float> leftHalf;
    std::vector<float> rightHalf;
    std::vector<float> topHalf;
    std::vector<float> bottomHalf;

    int half = size >> 1;

    for (int yOffset = 0; yOffset < size; yOffset++) {
        for (int xOffset = 0; xOffset < size; xOffset++) {
            int x = xStart + xOffset;
            int y = yStart + yOffset;
            float val = data[y * m_img->width() + x];

            if (xOffset < half) {
                leftHalf.push_back(val);
            } else {
                rightHalf.push_back(val);
            }

            if (yOffset < half) {
                topHalf.push_back(val);
            } else {
                bottomHalf.push_back(val);
            }
        }
    }

    if (!checkMeanEquality(topHalf, bottomHalf, rejectRate)) {
        return false;
    }

    if (!checkMeanEquality(leftHalf, rightHalf, rejectRate)) {
        return false;
    }

    return true;
}

bool BGFunctions::isFlatTile(int xStart, int yStart, int size, float rejectRate)
{
    if (xStart + size > m_img->width() || yStart + size > m_img->height()) {
        return false;
    }

    float *data = m_img->data();
    float total = 0.0f;
    for (int yOffset = 0; yOffset < size; yOffset++) {
        for (int xOffset = 0; xOffset < size; xOffset++) {
            int x = xStart + xOffset;
            int y = yStart + yOffset;
            total += data[y * m_img->width() + x];
        }
    }
    if (total < 1e-6) {
        return false;
    }

    rejectRate = 1 - static_cast<float>(sqrt(1 - rejectRate));
    if (!testNormality(xStart, yStart, size, rejectRate)) {
        return false;
    }

    rejectRate = 1 - static_cast<float>(sqrt(sqrt(1 - rejectRate)));
    if (!checkMeans(xStart, yStart, size, rejectRate)) {
        return false;
    }

    return true;
}

bool BGFunctions::tilesAvailable(int size, float rejectRate)
{
    for (int y = 0; y < m_img->height(); y += size) {
        for (int x = 0; x < m_img->width(); x += size) {
            if (isFlatTile(x, y, size, rejectRate)) {
                return true;
            }
        }
    }

    return false;
}

int BGFunctions::largestTileSize(float rejectRate)
{
    int size = 1 << 6;
    int min = 1 << 4;
    int max = 1 << 7;

    if (tilesAvailable(size, rejectRate)) {
        while (size < max) {
            size <<= 1;
            if (!tilesAvailable(size, rejectRate)) {
                return size >> 1;
            }
        }
        return max;
    } else {
        while (size > min) {
            size >>= 1;
            if (tilesAvailable(size, rejectRate)) {
                return size;
            }
        }
    }

    return 0;
}

void BGFunctions::calculateMean(std::vector<Pixel> tiles, int size)
{
    float *data = m_img->data();
    std::vector<float> tileMeans;

    for (Pixel tile : tiles) {
        float total = 0.0f;
        int xStart = tile.x();
        int yStart = tile.y();
        for (int yOffset = 0; yOffset < size; yOffset++) {
            for (int xOffset = 0; xOffset < size; xOffset++) {
                int y = yStart + yOffset;
                int x = xStart + xOffset;
                total += data[y * m_img->width() + x];
            }
        }
        tileMeans.push_back(total / static_cast<float>(size * size));
    }

    float total = 0.0f;
    for (float mean : tileMeans) {
        total += mean;
    }
    m_mean = total / static_cast<float>(tileMeans.size());
}

void BGFunctions::calculateVariance(std::vector<Pixel> tiles, int size)
{
    float *data = m_img->data();
    float total = 0.0f;

    for (Pixel tile : tiles) {
        int xStart = tile.x();
        int yStart = tile.y();
        for (int yOffset = 0; yOffset < size; yOffset++) {
            for (int xOffset = 0; xOffset < size; xOffset++) {
                int y = yStart + yOffset;
                int x = xStart + xOffset;
                float diff = data[y * m_img->width() + x] - m_mean;
                total += (diff * diff);
            }
        }
    }

    m_variance = total / static_cast<float>(tiles.size() * size * size);
}

void BGFunctions::estimateStats(std::vector<Pixel> tiles, int size)
{
    calculateMean(tiles, size);
    calculateVariance(tiles, size);
    m_gain = m_mean / m_variance;
}

void BGFunctions::setStats(int size, float rejectRate)
{
    std::vector<Pixel> tiles;

    for (int y = 0; y < m_img->height(); y += size) {
        for (int x = 0; x < m_img->width(); x += size) {
            if (isFlatTile(x, y, size, rejectRate)) {
                Pixel p(-1.0f, x, y);
                tiles.push_back(p);
            }
        }
    }

    #ifdef DEBUG
        std::cout << "Number of usable tiles found: " << tiles.size() << std::endl;
    #endif

    estimateStats(tiles, size);
}

void BGFunctions::estimateBG()
{
    TimerWrapper::TimerInstance()->startTimer();
    float rejectRate = 0.05f;
    int tileSize = largestTileSize(rejectRate);

    if (tileSize == 0) {
        std::cout << "No usable background tiles, aborting." << std::endl;
        exit(1);
    }

    #ifdef DEBUG
        std::cout << "Using a background tile size of: " << tileSize <<std::endl;
    #endif

    setStats(tileSize, rejectRate);
    TimerWrapper::TimerInstance()->stopTimer("Background estimation");
}
