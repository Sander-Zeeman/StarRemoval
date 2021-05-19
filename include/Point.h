#ifndef POINT_H
#define POINT_H

#include <stdlib.h>

class Point
{
public:
    Point(float x, float y) : m_x(x), m_y(y) {};
    ~Point() {};

    float x() { return m_x; }
    float y() { return m_y; }
    float euclidSquaredDist(Point p) {
        float xDist = abs(p.x() - m_x);
        float yDist = abs(p.y() - m_y);
        return xDist * xDist + yDist * yDist;
    };

private:
    float m_x { 0.0f };
    float m_y { 0.0f };
};

#endif
