#include "../include/PostProcessing.h"

void fixEdges(Image *img, long *ids)
{
    float *data = img->data();

    for (int y = 0; y < img->height(); y++) {
        if (ids[y * img->width()] >= 0) {
            ids[y * img->width()] = -1;
            data[y * img->width()] = 0.0f;
        }

        if (ids[(y + 1) * img->width() - 1] >= 0) {
            ids[(y + 1) * img->width() - 1] = -1;
            data[(y + 1) * img->width() - 1] = 0.0f;
        }
    }

    for (int x = 0; x < img->width(); x++) {
        if (ids[x] >= 0) {
            ids[x] = -1;
            data[x] = 0.0f;
        }

        if (ids[(img->height() - 1) * img->width() + x] >= 0) {
            ids[(img->height() - 1) * img->width() + x] = -1;
            data[(img->height() - 1) * img->width() + x] = 0.0f;
        }
    }
}

bool *createMask(long *ids, long size)
{
    bool *mask = new bool[size];
    for (long i = 0; i < size; i++)
        mask[i] = (ids[i] >= 0);

    return mask;
}

void remove(Image *img, long *ids)
{
    #ifdef TIMED
        TimerWrapper::TimerInstance()->startTimer();
    #endif

    float *data = img->data();
    bool *mask = createMask(ids, img->size());

    for (long i = 0; i < img->size(); i++)
        if (mask[i])
            data[i] = 0.0f;

    delete [] mask;

    #ifdef TIMED
        TimerWrapper::TimerInstance()->stopTimer("Removal");
    #endif
}

void hierarchicalInpaint(Image *img, long *ids, long *closestSigAncestors, Node *nodes)
{
    #ifdef TIMED
        TimerWrapper::TimerInstance()->startTimer();
    #endif

    float *data = img->data();
    bool *mask = createMask(ids, img->size());
    long parentIndex;

    for (long i = 0; i < img->size(); i++) {
        if (mask[i]) {
            if (closestSigAncestors[ids[i]] != NO_PARENT) {
                parentIndex = nodes[closestSigAncestors[ids[i]]].parent();
            } else {
                parentIndex = nodes[ids[i]].parent();
            }
            data[i] = data[parentIndex];
        }
    }

    delete [] mask;

    #ifdef TIMED
        TimerWrapper::TimerInstance()->stopTimer("Hierarchical inpainting");
    #endif
}

void findLimits(bool *mask, int width, int x, int y, int & lowX, int & highX, int & lowY, int & highY)
{
    lowX = x;
    do {
        lowX--;
    } while (mask[y * width + lowX]);

    highX = x;
    do {
        highX++;
    } while (mask[y * width + highX]);

    lowY = y;
    do {
        lowY--;
    } while (mask[lowY * width + x]);

    highY = y;
    do {
        highY++;
    } while (mask[highY * width + x]);
}

float interpolate2D(int idx, int low, float dataLow, int high, float dataHigh)
{
    int lowWidth = idx - low;
    int totalWidth = high - low;
    float t = static_cast<float>(lowWidth) / totalWidth;

    return (1 - t) * dataLow + t * dataHigh;
}

float interpolate(float *data, int width, int x, int y, int lowX, int highX, int lowY, int highY)
{
    float dataLowX = data[y * width + lowX];
    float dataHighX = data[y * width + highX];
    float interpolationX = interpolate2D(x, lowX, dataLowX, highX, dataHighX);

    float dataLowY = data[lowY * width + x];
    float dataHighY = data[highY * width + x];
    float interpolationY = interpolate2D(y, lowY, dataLowY, highY, dataHighY);

    return (interpolationX + interpolationY) / 2;
}

void interpolateInpaint(Image *img, long *ids)
{
    #ifdef TIMED
        TimerWrapper::TimerInstance()->startTimer();
    #endif

    float *data = img->data();
    bool *mask = createMask(ids, img->size());

    int lowX, highX, lowY, highY;
    float interpolatedValue;
    for (int y = 0; y < img->height(); y++) {
        for (int x = 0; x < img->width(); x++) {
            if (mask[y * img->width() + x]) {
                findLimits(mask, img->width(), x, y, lowX, highX, lowY, highY);
                interpolatedValue = interpolate(data, img->width(), x, y, lowX, highX, lowY, highY);
                data[y * img->width() + x] = interpolatedValue;
            }
        }
    }

    delete [] mask;

    #ifdef TIMED
        TimerWrapper::TimerInstance()->stopTimer("Interpolation inpainting");
    #endif
}

void NSInpaint(Image *img, long *ids, int radius)
{
    #ifdef TIMED
        TimerWrapper::TimerInstance()->startTimer();
    #endif

    cv::Mat input = cv::Mat(
        img->height(),
        img->width(),
        CV_32FC1,
        img->data()
    );

    float *data = img->data();
    bool *maskArray = createMask(ids, img->size());

    cv::Mat mask = cv::Mat(
        img->height(),
        img->width(),
        CV_8UC1,
        maskArray
    );

    cv::Mat output;

    cv::inpaint(input, mask, output, radius, cv::INPAINT_NS);

    float *newImg = output.ptr<float>();
    for (long i = 0; i < img->size(); i++) {
        data[i] = newImg[i];
    }

    delete [] maskArray;

    #ifdef TIMED
        TimerWrapper::TimerInstance()->stopTimer("Navier Stokes inpainting");
    #endif
}
