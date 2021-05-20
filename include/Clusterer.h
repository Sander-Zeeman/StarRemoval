#include "Debug.h"
#include "Point.h"

#include <vector>

class Clusterer
{
public:
    Clusterer(std::vector<Point> points) : m_points(points) {}
    ~Clusterer() {}

    void cluster();
private:
    void initCentroids();
    long closestCentroid(Point p);
    void calculateMeanLocation(long index, float & x, float & y);
    bool step();

    std::vector<Point> m_points;
    std::vector<Point> m_centroids;
    int m_maxIterations { 100 };
    float m_epsilon = 1e-6f;
};
