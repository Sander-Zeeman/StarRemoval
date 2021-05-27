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
    double s, k, k_2, p;

    s = skewTest(data);
    k = kurtosisTest(data);
    k_2 = s*s + k*k;

    boost::math::chi_squared dist(2);
    p = boost::math::cdf(boost::math::complement(dist, k_2));

    return p;
}

double TTest(std::vector<float> data1, std::vector<float> data2)
{
    // Using example by the boost documentation.
    // https://www.boost.org/doc/libs/1_46_1/libs/math/doc/sf_and_dist/html/math_toolkit/dist/stat_tut/weg/st_eg/two_sample_students_t.html
    // Slightly off from the scipy implementation, but acceptable.
    double p,t;

    double size1 = static_cast<double>(data1.size());
    double size2 = static_cast<double>(data2.size());
    double dof = size1 + size2 - 2.0;

    double mean1 = boost::math::statistics::mean(data1);
    double mean2 = boost::math::statistics::mean(data2);

    double var1 = boost::math::statistics::variance(data1);
    double var2 = boost::math::statistics::variance(data2);

    t = (mean1 - mean2) / sqrt((var1 / size1) + (var2 / size2));

    boost::math::students_t dist(dof);
    p = boost::math::cdf(boost::math::complement(dist, fabs(t)));

    // two sided test
    return p * 2;
}


bool testNormality(Image *img, int xStart, int yStart, int size, double rejectRate)
{
    float *data = img->data();

    std::vector<float> tile;
    for (int yOffset = 0; yOffset < size; yOffset++) {
        for (int xOffset = 0; xOffset < size; xOffset++) {
            int x = xStart + xOffset;
            int y = yStart + yOffset;
            tile.push_back(data[y * img->width() + x]);
        }
    }

    double p = KSquared(tile);
    return p > rejectRate;
}

bool checkMeanEquality(std::vector<float> first, std::vector<float> second, double rejectRate)
{
    double p = TTest(first, second);
    return p > rejectRate;
}

bool checkMeans(Image *img, int xStart, int yStart, int size, double rejectRate)
{
    float *data = img->data();

    std::vector<float> leftHalf;
    std::vector<float> rightHalf;
    std::vector<float> topHalf;
    std::vector<float> bottomHalf;

    int half = size >> 1;

    for (int yOffset = 0; yOffset < size; yOffset++) {
        for (int xOffset = 0; xOffset < size; xOffset++) {
            int x = xStart + xOffset;
            int y = yStart + yOffset;
            float val = data[y * img->width() + x];

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


bool isFlatTile(Image *img, int xStart, int yStart, int size, double rejectRate)
{
    if (xStart + size > img->width() || yStart + size > img->height()) {
        return false;
    }

    float *data = img->data();
    float total = 0.0f;
    for (int yOffset = 0; yOffset < size; yOffset++) {
        for (int xOffset = 0; xOffset < size; xOffset++) {
            int x = xStart + xOffset;
            int y = yStart + yOffset;
            total += data[y * img->width() + x];
        }
    }

    if (total < 1e-6) {
        return false;
    }

    rejectRate = 1 - sqrt(1 - rejectRate);
    if (!testNormality(img, xStart, yStart, size, rejectRate)) {
        return false;
    }

    rejectRate = 1 - sqrt(sqrt(1 - rejectRate));
    if (!checkMeans(img, xStart, yStart, size, rejectRate)) {
        return false;
    }

    return true;
}

bool tilesAvailable(Image *img, int size, double rejectRate)
{
    for (int y = 0; y < img->height(); y += size) {
        for (int x = 0; x < img->width(); x += size) {
            if (isFlatTile(img, x, y, size, rejectRate)) {
                return true;
            }
        }
    }

    return false;
}

int largestTileSize(Image *img, double rejectRate)
{
    int size = 1 << 6;
    int min = 1 << 4;
    int max = 1 << 7;

    if (tilesAvailable(img, size, rejectRate)) {
        while (size < max) {
            size <<= 1;
            if (!tilesAvailable(img, size, rejectRate)) {
                return size >> 1;
            }
        }
        return max;
    } else {
        while (size > min) {
            size >>= 1;
            if (tilesAvailable(img, size, rejectRate)) {
                return size;
            }
        }
    }

    return 0;
}

std::vector<float> extractData(Image *img, std::vector<Pixel> tiles, int size)
{
    float *data = img->data();
    std::vector<float> tileData;

    for (Pixel tile : tiles) {
        int xStart = tile.x();
        int yStart = tile.y();
        for (int yOffset = 0; yOffset < size; yOffset++) {
            for (int xOffset = 0; xOffset < size; xOffset++) {
                int y = yStart + yOffset;
                int x = xStart + xOffset;
                tileData.push_back(data[y * img->width() + x]);
            }
        }
    }

    return tileData;
}

void estimateStats(Image *img, std::vector<Pixel> tiles, imgStats & stats, int size)
{
    std::vector<float> tileData = extractData(img, tiles, size);

    stats.mean = boost::math::statistics::mean(tileData);
    stats.var = boost::math::statistics::variance(tileData);
    stats.gain = stats.mean / stats.var;
}

void setStats(Image *img, int size, imgStats & stats, double rejectRate)
{
    std::vector<Pixel> tiles;

    for (int y = 0; y < img->height(); y += size) {
        for (int x = 0; x < img->width(); x += size) {
            if (isFlatTile(img, x, y, size, rejectRate)) {
                Pixel p(-1.0f, x, y);
                tiles.push_back(p);
            }
        }
    }

    #ifdef DEBUG
        std::cout << "Number of usable tiles found: " << tiles.size() << std::endl;
    #endif

    estimateStats(img, tiles, stats, size);
}

void estimateBG(Image *img, imgStats & stats)
{
    TimerWrapper::TimerInstance()->startTimer();

    double rejectRate = 0.05;
    int tileSize = largestTileSize(img, rejectRate);

    if (tileSize == 0) {
        std::cout << "No usable background tiles, aborting." << std::endl;
        exit(1);
    }

    #ifdef DEBUG
        std::cout << "Using a background tile size of: " << tileSize <<std::endl;
    #endif

    setStats(img, tileSize, stats, rejectRate);
    TimerWrapper::TimerInstance()->stopTimer("Background estimation");
}
