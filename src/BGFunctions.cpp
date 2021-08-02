#include "../include/BGFunctions.h"

double skewTest(std::vector<float> data)
{
    double n = static_cast<double>(data.size());

    double b2 = boost::math::statistics::skewness(data);
    double y = b2 * sqrt(
        ((n + 1.0) * (n + 3.0))
            /
        (6.0 * (n - 2.0))
    );

    double beta2 = (
        (3.0 * (n*n + 27.0*n - 70.0) * (n + 1.0) * (n + 3.0))
            /
        ((n - 2.0) * (n + 5.0) * (n + 7.0) * (n + 9.0))
    );

    double w2 = sqrt(2.0 * (beta2 - 1.0)) - 1.0;
    double delta = 1.0 / sqrt(0.5 * log(w2));
    double alpha = sqrt(2.0 / (w2 - 1.0));
    y = (y == 0.0 ? 1.0 : y);

    double coeff = y / alpha;
    double Z = delta * log(coeff + sqrt(coeff*coeff + 1.0));
    return Z;
}

double kurtosisTest(std::vector<float> data)
{
    double n = static_cast<double>(data.size());

    double b2 = boost::math::statistics::kurtosis(data);
    double E = 3.0 * (n - 1.0) / (n + 1.0);
    double varb2 =
        (24.0 * n * (n - 2.0) * (n - 3.0))
            /
        ((n + 1.0) * (n + 1.0) * (n + 3.0) * (n + 5.0));
    double x = (b2 - E) / sqrt(varb2);

    double sqrtbeta1 =
        6.0 * (n*n - 5.0*n + 2.0)
            /
        ((n + 7.0) * (n + 9.0))
            *
        sqrt(
            (6 * (n + 3.0) * (n + 5.0))
                /
            (n * (n - 2.0) * (n - 3.0))
        );

    double A = 6.0 + (8.0 / sqrtbeta1) * (
        (2.0 / sqrtbeta1) + sqrt(
            1.0 + (4.0 / (sqrtbeta1 * sqrtbeta1))
        )
    );

    double term1 = 1.0 - 2.0 / (9.0 * A);
    double denom = 1.0 + x * sqrt(
        2.0 / (A - 4.0)
    );
    double term2 = cbrt((1.0 - (2.0 / A)) / denom);

    double Z = (term1 - term2) / sqrt(2.0 / (9.0 * A));
    return Z;
}

double KSquared(std::vector<float> data)
{
    // Follows the scipy implementation of this test
    // https://github.com/scipy/scipy/blob/v1.6.3/scipy/stats/stats.py#L1635-L1706
    double s = skewTest(data);
    double k = kurtosisTest(data);
    double k_2 = s*s + k*k;

    boost::math::chi_squared dist(2);
    double pValue = boost::math::cdf(boost::math::complement(dist, k_2));

    return pValue;
}

bool testNormality(Image *img, int xStart, int yStart, int tileSize, double rejectRate)
{
    float *data = img->data();

    std::vector<float> tile;
    for (int yOffset = 0; yOffset < tileSize; yOffset++) {
        for (int xOffset = 0; xOffset < tileSize; xOffset++) {
            int x = xStart + xOffset;
            int y = yStart + yOffset;
            tile.push_back(data[y * img->width() + x]);
        }
    }

    double pValue = KSquared(tile);
    return pValue > rejectRate;
}

double TTest(std::vector<float> firstHalf, std::vector<float> secondHalf)
{
    // Using example by the boost documentation.
    // https://www.boost.org/doc/libs/1_46_1/libs/math/doc/sf_and_dist/html/math_toolkit/dist/stat_tut/weg/st_eg/two_sample_students_t.html
    // Slightly off from the scipy implementation, but acceptable.
    double pValue, t;

    int size1 = firstHalf.size();
    int size2 = secondHalf.size();
    int dof = size1 + size2 - 2;

    double mean1 = boost::math::statistics::mean(firstHalf);
    double mean2 = boost::math::statistics::mean(secondHalf);

    double var1 = boost::math::statistics::variance(firstHalf);
    double var2 = boost::math::statistics::variance(secondHalf);

    t = (mean1 - mean2) / sqrt((var1 / size1) + (var2 / size2));

    boost::math::students_t dist(dof);
    pValue = boost::math::cdf(boost::math::complement(dist, fabs(t)));

    // two sided test
    return pValue * 2;
}

bool checkMeanEquality(std::vector<float> firstHalf, std::vector<float> secondHalf, double rejectRate)
{
    // Check for a statistically significant difference between the means of two halves.
    double pValue = TTest(firstHalf, secondHalf);
    return pValue > rejectRate;
}

bool checkMeans(Image *img, int xStart, int yStart, int tileSize, double rejectRate)
{
    float *data = img->data();

    std::vector<float> leftHalf;
    std::vector<float> rightHalf;
    std::vector<float> topHalf;
    std::vector<float> bottomHalf;

    int halfSize = tileSize >> 1;

    // Split the tile in two halves in two ways, vertically and horizontally.
    for (int yOffset = 0; yOffset < tileSize; yOffset++) {
        for (int xOffset = 0; xOffset < tileSize; xOffset++) {
            int x = xStart + xOffset;
            int y = yStart + yOffset;
            float val = data[y * img->width() + x];

            if (xOffset < halfSize) {
                leftHalf.push_back(val);
            } else {
                rightHalf.push_back(val);
            }

            if (yOffset < halfSize) {
                topHalf.push_back(val);
            } else {
                bottomHalf.push_back(val);
            }
        }
    }

    if (!checkMeanEquality(topHalf, bottomHalf, rejectRate))
        return false;

    if (!checkMeanEquality(leftHalf, rightHalf, rejectRate))
        return false;

    return true;
}

// Determines whether a tile can be considered flat, based on 4 criteria.
bool isFlatTile(Image *img, int xStart, int yStart, int tileSize)
{
    if (xStart + tileSize > img->width() || yStart + tileSize > img->height())
        return false;

    float *data = img->data();
    float total = 0.0f;
    for (int yOffset = 0; yOffset < tileSize; yOffset++) {
        for (int xOffset = 0; xOffset < tileSize; xOffset++) {
            int x = xStart + xOffset;
            int y = yStart + yOffset;
            total += data[y * img->width() + x];
        }
    }

    if (total < 1e-6)
        return false;

    const double normalityRejectRate = 1 - sqrt(1 - baseRejectRate);
    if (!testNormality(img, xStart, yStart, tileSize, normalityRejectRate))
        return false;

    const double meanRejectRate = 1 - sqrt(sqrt(1 - baseRejectRate));
    if (!checkMeans(img, xStart, yStart, tileSize, meanRejectRate))
        return false;

    return true;
}

// Finds whether any flat tiles exist at this tile size.
bool tilesAvailable(Image *img, int tileSize)
{
    for (int y = 0; y < img->height(); y += tileSize)
        for (int x = 0; x < img->width(); x += tileSize)
            if (isFlatTile(img, x, y, tileSize))
                return true;

    return false;
}

// Find the largest possible tile size for which tiles exist.
int largestTileSize(Image *img)
{
    int tileSize = 1 << 6;
    int minSize = 1 << 4;
    int maxSize = 1 << 7;

    if (tilesAvailable(img, tileSize)) {
        while (tileSize < maxSize) {
            tileSize <<= 1;
            if (!tilesAvailable(img, tileSize)) {
                return tileSize >> 1;
            }
        }
        return maxSize;
    } else {
        while (tileSize > minSize) {
            tileSize >>= 1;
            if (tilesAvailable(img, tileSize)) {
                return tileSize;
            }
        }
        return 0;
    }
}

// Extract the data from a list of tiles to a single array.
std::vector<float> extractData(Image *img, std::vector<Pixel> tiles, int tileSize)
{
    float *data = img->data();
    std::vector<float> tileData;

    for (Pixel tile : tiles) {
        int xStart = tile.x();
        int yStart = tile.y();
        for (int yOffset = 0; yOffset < tileSize; yOffset++) {
            for (int xOffset = 0; xOffset < tileSize; xOffset++) {
                int y = yStart + yOffset;
                int x = xStart + xOffset;
                tileData.push_back(data[y * img->width() + x]);
            }
        }
    }

    return tileData;
}

// Determines the mean, variance and gain of a list of tiles.
void estimateStats(Image *img, std::vector<Pixel> tiles, int tileSize, imgStats & stats)
{
    std::vector<float> tileData = extractData(img, tiles, tileSize);

    stats.mean = boost::math::statistics::mean(tileData);
    stats.var = boost::math::statistics::variance(tileData);
    stats.gain = stats.mean / stats.var;

    #ifdef DEBUG
        std::cout << "Image statistics:" << std::endl;
        std::cout << "  Mean: " << stats.mean << std::endl;
        std::cout << "  Variance: " << stats.var << std::endl;
        std::cout << "  Gain: " << stats.gain << std::endl;
    #endif
}

void setStats(Image *img, int tileSize, imgStats & stats)
{
    std::vector<Pixel> tiles;

    for (int y = 0; y < img->height(); y += tileSize) {
        for (int x = 0; x < img->width(); x += tileSize) {
            if (isFlatTile(img, x, y, tileSize)) {
                Pixel pixel(x, y);
                tiles.push_back(pixel);
            }
        }
    }

    #ifdef DEBUG
        std::cout << "Number of usable tiles found: " << tiles.size() << std::endl;
    #endif

    estimateStats(img, tiles, tileSize, stats);
}

void estimateBG(Image *img, imgStats & stats)
{
    #ifdef TIMED
        TimerWrapper::TimerInstance()->startTimer();
    #endif

    int tileSize = largestTileSize(img);

    if (tileSize == 0) {
        std::cout << "No usable background tiles, aborting." << std::endl;
        exit(1);
    }

    #ifdef DEBUG
        std::cout << "Using a background tile size of: " << tileSize << std::endl;
    #endif

    setStats(img, tileSize, stats);

    #ifdef TIMED
        TimerWrapper::TimerInstance()->stopTimer("Background estimation");
    #endif
}
