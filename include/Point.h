#ifndef POINT_H
#define POINT_H

#include <stdlib.h>

class Point
{
public:
    Point(long index, float area, float value)
        : m_index(index), m_x(area), m_y(value) {};
    ~Point() {};

    float x() { return m_x; }
    float y() { return m_y; }
    long index() { return m_index; }
    short id() { return m_id; }

    void setLocation(float x, float y) {
        m_x = x;
        m_y = y;
    }
    void setID(short id) { m_id = id; }

    float euclidSquaredDist(Point p) {
        float xDist = abs(p.x() - m_x);
        float yDist = abs(p.y() - m_y);
        return xDist * xDist + yDist * yDist;
    };

private:
    float m_x { 0.0f };
    float m_y { 0.0f };
    long index { -1 };
    short m_id { -1 };
};

#endif
