#include "../include/PostProcessing.h"

int findLimit(float *data, int width, int height, int x, int y, bool vertical, bool increasing)
{
    int temp = vertical ? y : x;
    long idx;

    do {
        temp += increasing ? 1 : -1;

        if (increasing) {
            if (vertical) {
                if (temp >= height) {
                    break;
                }
            } else {
                if (temp >= width) {
                    break;
                }
            }
        } else {
            if (temp < 0) {
                break;
            }
        }

        idx = vertical ? (temp * width + x) : (y * width + temp);
    } while (data[idx] < 0);

    return temp;
}

void findLimits(float *data, int width, int height, int x, int y, int & lowX, int & highX, int & lowY, int & highY)
{
    lowX = findLimit(data, width, height, x, y, false, false);
    highX = findLimit(data, width, height, x, y, false, true);
    lowY = findLimit(data, width, height, x, y, true, false);
    highY = findLimit(data, width, height, x, y, true, true);
}

float interpolate2D(int idx, int low, float dataLow, int high, float dataHigh)
{
    int lowWidth = idx - low;
    int totalWidth = high - low;
    float t = static_cast<float>(lowWidth) / static_cast<float>(totalWidth);

    return (1 - t) * dataLow + t * dataHigh;
}

float interpolate(float *data, int width, int height, int x, int y, int lowX, int highX, int lowY, int highY)
{
    float dataLowX = data[y * width + lowX];
    float dataHighX = data[y * width + highX];
    float dataLowY = data[lowY * width + x];
    float dataHighY = data[highY * width + x];

    if (lowX < 0) {
        lowX = 0;
        dataLowX = 0.0f;
    }
    if (highX >= width) {
        highX = width - 1;
        dataHighX = 0.0f;
    }
    if (lowY < 0) {
        lowY = 0;
        dataLowY = 0.0f;
    }
    if (highY >= height) {
        highY = height - 1;
        dataHighY = 0.0f;
    }

    float interpolationX = interpolate2D(x, lowX, dataLowX, highX, dataHighX);
    float interpolationY = interpolate2D(y, lowY, dataLowY, highY, dataHighY);

    return (interpolationX + interpolationY) / 2;
}
