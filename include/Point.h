#ifndef POINT_H
#define POINT_H

#include <stdlib.h>

class Point
{
public:
    Point(long index, float area, float value) : m_index(index), m_x(area), m_y(value) {};
    Point(float area, float value) : m_x(area), m_y(value) {};
    ~Point() {};

    long id() { return m_id; }
    long index() { return m_index; }
    float x() { return m_x; }
    float y() { return m_y; }

    void setID(long id) { m_id = id; }
    void setLocation(float x, float y) {
        m_x = x;
        m_y = y;
    }

    float euclidSquaredDist(Point p) {
        float xDist = abs(p.x() - m_x);
        float yDist = abs(p.y() - m_y);
        return xDist * xDist + yDist * yDist;
    };

private:
    long m_id { -1 };
    long m_index;
    float m_x;
    float m_y;
};

#endif
