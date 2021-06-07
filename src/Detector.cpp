#include "../include/Detector.h"

Detector::Detector(Image *img, MaxTree *tree)
	: m_tree(tree)
    , m_img(img)
{
	long size = m_img->size();
	m_closestSigAncestors = new long[size];
  	m_mainBranches = new long[size];

    for (long i = 0; i < size; i++) {
        m_closestSigAncestors[i] = NO_PARENT;
    	m_mainBranches[i] = NO_PARENT;
    }

	m_objectIDs = new long[size];
}

Detector::~Detector()
{
	delete [] m_closestSigAncestors;
    delete [] m_mainBranches;
	delete [] m_objectIDs;
}

void Detector::findRelevantNodes()
{
    TimerWrapper::TimerInstance()->startTimer();

	Heap *heap = new Heap(m_img->size());
    float *data = m_img->data();
    Node *nodes = m_tree->nodes();

	for (int y = 0; y < m_img->height(); y++) {
		for (int x = 0; x < m_img->width(); x++) {
			Pixel pixel = Pixel(data[x + m_img->width() * y], x, y);
			long index = pixel.index(m_img->width());
			long parentIndex = nodes[index].parent();

			if ( nodes + index == m_tree->root() || data[index] == data[parentIndex] )
				continue;

			heap->insert(pixel);
		}
	}

	while (!heap->isEmpty())
		m_relevantIndices.push_back(heap->remove().index(m_img->width()));

    std::reverse(m_relevantIndices.begin(), m_relevantIndices.end());

    #ifdef DEBUG
    	std::cout << m_relevantIndices.size() << " relevant indices found." << std::endl;
    #endif

	delete heap;

    TimerWrapper::TimerInstance()->stopTimer("Finding relevant indices");
}

void Detector::updateMainBranch(long idx)
{
    if (m_closestSigAncestors[idx] == NO_PARENT)
        return;

    Node *nodes = m_tree->nodes();

    long ancestorIdx = m_closestSigAncestors[idx];

    if (nodes[idx].hasSigDescendent()) {
        long currSigDescendentIdx = m_mainBranches[ancestorIdx];
        if (nodes[currSigDescendentIdx].area() < nodes[idx].area()) {
            m_mainBranches[ancestorIdx] = idx;
        }
    } else {
        nodes[ancestorIdx].setHasSigDescendent(true);
        m_mainBranches[ancestorIdx] = idx;
    }
}

float Detector::noiseVariance(long idx)
{
    imgStats statistics = m_img->stats();
    float variance = statistics.var;
    float gain = statistics.gain;

    if (m_closestSigAncestors[idx] != NO_PARENT) {
        variance += (m_img->data()[m_closestSigAncestors[idx]] / gain);
    }

    return variance;
}

float Detector::powerDef(long idx)
{
    float *data = m_img->data();
    Node *nodes = m_tree->nodes();

    long parentIdx = nodes[idx].parent();

    float delta = data[parentIdx];

    if (m_closestSigAncestors[idx] != NO_PARENT) {
        delta -= data[m_closestSigAncestors[idx]];
    }

    return
        nodes[idx].power() +
        delta * (
            2 * nodes[idx].volume() +
            delta * static_cast<float>(nodes[idx].area())
        );
}

bool Detector::sigTest(long idx)
{
    float params1[4] = {
        1.683355084690155e-01f,
        3.770229379757511e+02f,
        1.176722049258011e+05f,
        6.239836661965291e+06f
    };

    float params2[3] = {
        1.354265276841128e+03f,
        2.091126298053044e+05f,
        1.424803575269314e+06f
    };

    float variance = noiseVariance(idx);
    float power = powerDef(idx);
    float area = static_cast<float>(m_tree->nodes()[idx].area());
    float normalizedPower = power / variance / area;

    // Constant from repo.
    if (area > 4087.0f) {
        area = 4087.0f;
    }

    float areaSquared = area * area;
    float areaCubed = areaSquared * area;

    float num =
        params1[0] * areaCubed +
        params1[1] * areaSquared +
        params1[2] * area +
        params1[3];

    float denom =
        areaCubed +
        params2[0] * areaSquared +
        params2[1] * area +
        params2[2];

    float x = num / denom;

    return normalizedPower > x;
}

void Detector::findSignificantNodes()
{
    TimerWrapper::TimerInstance()->startTimer();

    Node *nodes = m_tree->nodes();

    for (unsigned long i = 0; i < m_relevantIndices.size(); i++) {
        long idx = m_relevantIndices[i];
        long parentIdx = nodes[idx].parent();

        if (nodes[parentIdx].isSignificant()) {
            m_closestSigAncestors[idx] = parentIdx;
        } else if (m_closestSigAncestors[parentIdx] != NO_PARENT) {
            m_closestSigAncestors[idx] = m_closestSigAncestors[parentIdx];
        }

        if (sigTest(idx)) {
            nodes[idx].setSignificant(true);
            m_significantNodeCount++;
            updateMainBranch(idx);
        }
    }

    #ifdef DEBUG
        std::cout << m_significantNodeCount << " significant nodes found." << std::endl;
    #endif

    TimerWrapper::TimerInstance()->stopTimer("Finding significant nodes");
}

void Detector::findObjects()
{
    TimerWrapper::TimerInstance()->startTimer();

    Node *nodes = m_tree->nodes();

    long nestedCount = 0;

    for (unsigned long i = 0; i < m_relevantIndices.size(); i++) {
        long idx = m_relevantIndices[i];
        long parentIdx = m_closestSigAncestors[idx];

        if (!nodes[idx].isSignificant())
            continue;

        if (parentIdx == NO_PARENT) {
            nodes[idx].setObject(true);
            m_objectCount++;
            continue;
        }

        if (m_mainBranches[parentIdx] != idx) {
            nodes[idx].setObject(true);
            nestedCount++;
            continue;
        }
    }

    m_objectCount += nestedCount;

    #ifdef DEBUG
        std::cout << m_objectCount << " objects detected. " <<
        nestedCount << " were nested." << std::endl;
    #endif

    TimerWrapper::TimerInstance()->stopTimer("Finding objects");
}

float Detector::findTop(Node *nodes, long idx)
{
    if (nodes[idx].top() != UNASSIGNED)
        return nodes[idx].top();

    float top = nodes[idx].hasSigDescendent() ? findTop(nodes, m_mainBranches[idx]) : nodes[idx].height();

    nodes[idx].setTop(top);
    return top;
}

void Detector::findStars()
{
    TimerWrapper::TimerInstance()->startTimer();

    Node *nodes = m_tree->nodes();

    for (unsigned long i = 0; i < m_relevantIndices.size(); i++) {
        long idx = m_relevantIndices[i];

        if (!nodes[idx].isObject())
            continue;

        float toss = findTop(m_tree->nodes(), idx);
    }

    for (unsigned long i = 0; i < m_relevantIndices.size(); i++) {
        long idx = m_relevantIndices[i];

        if (!nodes[idx].isObject()) {
            continue;
        }

        // > 1e6 area probably background
        if (nodes[idx].area() < 10000 && nodes[idx].top() > 0.3) {
            nodes[idx].setStar(true);
            m_starCount++;
            continue;
        }
    }

    #ifdef DEBUG
        std::cout << m_starCount << " stars detected." << std::endl;
    #endif

    TimerWrapper::TimerInstance()->stopTimer("Finding stars");
}

void Detector::markIDs()
{
    TimerWrapper::TimerInstance()->startTimer();

    Node *nodes = m_tree->nodes();

    for (long i = 0; i < m_img->size(); i++) {
        if (nodes[i].starChecked())
            continue;

        long next_idx = i;

        while (
            next_idx != NO_PARENT &&
            !nodes[next_idx].isStar() &&
            !nodes[next_idx].starChecked()
        )
        {
            nodes[next_idx].setCheckedForStar(true);
            next_idx = nodes[next_idx].parent();
        }

        long object_id = NO_OBJECT;
        long end_idx = next_idx;

        if (next_idx == NO_PARENT) {
            object_id = NO_OBJECT;
            end_idx = next_idx;
        } else if (nodes[next_idx].starChecked()) {
            object_id = m_objectIDs[next_idx];
            end_idx = next_idx;
        } else if (nodes[next_idx].isStar()) {
            object_id = next_idx;
            end_idx = nodes[next_idx].parent();
            nodes[object_id].setCheckedForStar(true);
        }

        next_idx = i;
        do {
            m_objectIDs[next_idx] = object_id;
            next_idx = nodes[next_idx].parent();
        } while (next_idx != end_idx);
    }

    TimerWrapper::TimerInstance()->stopTimer("Marking IDs");
}

void Detector::starDetection()
{
    TimerWrapper::TimerInstance()->startTimer();

	findRelevantNodes();
	findSignificantNodes();
	findObjects();
    findStars();
	markIDs();

    TimerWrapper::TimerInstance()->stopTimer("Star detection");

    return;
}
