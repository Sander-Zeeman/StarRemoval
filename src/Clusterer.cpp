#include "../include/Clusterer.h"

void Clusterer::initCentroids() {
    // KMeans++
    m_centroids.push_back(Point(0, 10, 0.8f));
    m_centroids.push_back(Point(1, 500, 0.8f));
    m_centroids.push_back(Point(2, 10, 0.2f));
    m_centroids.push_back(Point(3, 500, 0.2f));
}

long Clusterer::closestCentroid(Point p)
{
    long closestCentroidIndex = m_centroids[0].index();
    float closestCentroidDist = p.euclidSquaredDist(m_centroids[0]);
    for (Point c : m_centroids) {
        float PCDist = p.euclidSquaredDist(c);
        if (PCDist < closestCentroidDist) {
            closestCentroidDist = PCDist;
            closestCentroidIndex = c.id();
        }
    }
    return closestCentroidIndex;
}

void Clusterer::calculateMeanLocation(long centroidIndex, float & x, float & y)
{
    x = 0.0f;
    y = 0.0f;
    int total = 0;

    for (Point p : m_points) {
        if (p.id() == centroidIndex) {
            x += p.x();
            y += p.y();
            total++;
        }
    }

    x /= static_cast<float>(total);
    y /= static_cast<float>(total);
}

bool Clusterer::step() {
    float distTraveled = 0.0f;

    // Assign points to closest centroid
    for (Point p : m_points)
        p.setID(closestCentroid(p));

    // Calculate centroid movement
    for (Point c : m_centroids) {
        float x, y;
        calculateMeanLocation(c.index(), x, y);
        distTraveled += Point(x, y).euclidSquaredDist(c);
        c.setLocation(x, y);
    }

    // Keep track of anything changing
    return distTraveled > m_epsilon;
}

void Clusterer::cluster()
{
    TimerWrapper::TimerInstance()->startTimer();
    int i = 0;

    TimerWrapper::TimerInstance()->startTimer();
    initCentroids();
    TimerWrapper::TimerInstance()->stopTimer("Initializing the centroids");

    TimerWrapper::TimerInstance()->startTimer();
    while (step()) {
        if (++i >= m_maxIterations) {
            #ifdef DEBUG
                std::cout << "Max Iterations reached while clustering!" << std::endl;
            #endif
            break;
        }
    }
    TimerWrapper::TimerInstance()->stopTimer("Adjusting the centroids");

    TimerWrapper::TimerInstance()->stopTimer("Clustering");
}
