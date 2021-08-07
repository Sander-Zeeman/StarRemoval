#include "../include/MaxTree.h"

MaxTree::MaxTree(Image *img)
    : m_img(img)
{
	m_nodes = new Node[m_img->size()];
    for (long i = 0; i < img->size(); i++) {
        m_nodes[i].setHeight(img->data()[i]);
    }
}

MaxTree::~MaxTree()
{
	delete [] m_nodes;
}

Pixel MaxTree::findStart()
{
	float *data = m_img->data();

	// Start search for minimum pixel at (0, 0).
	Pixel minPixel = Pixel(data[0], 0, 0);

	// Iterate over all pixels to find the minimal val.
	for (long i = 0; i < m_img->size(); i++) {
		if (data[i] < minPixel.val()) {
			minPixel.setVal(data[i]);
			minPixel.setLocation(i, m_img->width());
		}
	}

	return minPixel;
}

bool MaxTree::queueNeighbour(float val, int x, int y)
{
	Pixel neighbour = Pixel(-1, x, y);
	long neighbourIndex = neighbour.index(m_img->width());
	Node *neighbourNode = m_nodes + neighbourIndex;

	if (neighbourNode->parent() == UNASSIGNED) {
		neighbour.setVal(m_img->data()[neighbourIndex]);
		neighbourNode->setParent(IN_QUEUE);
		m_heap->insert(neighbour);

		if (neighbour.val() > val)
			return 1;
	}
	return 0;
}

void MaxTree::getBounds(
    Pixel pixel,
    int & radiusX,
    int & radiusY,
    int & xMin,
    int & xMax,
    int & yMin,
    int & yMax
)
{
    radiusX = m_connectivity->width() / 2;
	radiusY = m_connectivity->height() / 2;

    xMin = pixel.x() < radiusX ? radiusX - pixel.x() : 0;
	yMin = pixel.y() < radiusY ? radiusY - pixel.y() : 0;

	xMax =
        pixel.x() + radiusX >= m_img->width()
		    ? radiusX + m_img->width() - pixel.x() - 1
		    : 2 * radiusX;

	yMax =
        pixel.y() + radiusY >= m_img->height()
		    ? radiusY + m_img->height() - pixel.y() - 1
		    : 2 * radiusY;
}

void MaxTree::queueNeighbours(Pixel pixel)
{
    int radiusX, radiusY, xMin, xMax, yMin, yMax;
	getBounds(pixel, radiusX, radiusY, xMin, xMax, yMin, yMax);

	for (int offsetY = yMin; offsetY <= yMax; offsetY++) {
		for (int offsetX = xMin; offsetX <= xMax; offsetX++) {
			if (!m_connectivity->check(offsetX, offsetY))
				continue;

			if (queueNeighbour(
				pixel.val(),
				pixel.x() - radiusX + offsetX,
				pixel.y() - radiusY + offsetY
			))
				return;
		}
	}
}

void MaxTree::mergeNodes(long toIndex, long fromIndex)
{
	Node *toNode = m_nodes + toIndex;
	Node *fromNode = m_nodes + fromIndex;

	toNode->setArea(toNode->area() + fromNode->area());

	float delta = m_img->data()[fromIndex] - m_img->data()[toIndex];

    float volumeChange = delta * static_cast<float>(fromNode->area());
    toNode->setVolume(toNode->volume() + fromNode->volume() + volumeChange);

	float powerChange = delta * (2 * fromNode->volume() + volumeChange);
	toNode->setPower(toNode->power() + fromNode->power() + powerChange);
}

void MaxTree::descend(Pixel pixel)
{
	if (m_stack.size() < 2)
		return;

	Pixel oldTop = m_stack.top();
	m_stack.pop();
	Pixel newTop = m_stack.top();

	if (newTop.val() < pixel.val())
		m_stack.push(pixel);

	newTop = m_stack.top();
	long oldIndex = oldTop.index(m_img->width());
	long newIndex = newTop.index(m_img->width());

	m_nodes[oldIndex].setParent(newIndex);
	mergeNodes(newIndex, oldIndex);
}

void MaxTree::finishStack()
{
	// When we finish, the stack can still contain some pixels, we string them together.
	while (m_stack.size() > 1) {
		Pixel oldTop = m_stack.top();
		m_stack.pop();
		Pixel newTop = m_stack.top();

		long oldIndex = oldTop.index(m_img->width());
		long newIndex = newTop.index(m_img->width());

		m_nodes[oldIndex].setParent(newIndex);
		mergeNodes(newIndex, oldIndex);
	}
}

void MaxTree::flood()
{
    TimerWrapper::TimerInstance()->startTimer();

	// Find the least bright pixel (and its index in our array) as our starting point.
	Pixel nextPixel = findStart();
	long nextIndex = nextPixel.index(m_img->width());
	#ifdef DEBUG
	   std::cout << "Minimum pixel at: " << nextIndex << " with value: " << nextPixel.val() << std::endl << std::endl;
	#endif

	// Set this pixel to be the root of our tree.
	m_root = m_nodes + nextIndex;
	m_root->setParent(NO_PARENT);

	m_connectivity = new Connectivity();
	m_heap = new Heap(m_img->size());

	// Also insert this pixel in both the stack and heap.
	m_stack.push(nextPixel);
	m_heap->insert(nextPixel);

	while (!m_heap->isEmpty()) {
		Pixel currPixel = nextPixel;
		queueNeighbours(currPixel);

		nextPixel = m_heap->top();
		if (nextPixel.val() > currPixel.val()) {
			m_stack.push(nextPixel);
			continue;
		}

        currPixel = m_heap->remove();
		Pixel stackTop = m_stack.top();
		long currIndex = currPixel.index(m_img->width());
		long stackIndex = stackTop.index(m_img->width());
		if (currIndex != stackIndex) {
			m_nodes[currIndex].setParent(stackIndex);
			m_nodes[stackIndex].setArea(m_nodes[stackIndex].area() + 1);
		}

        if (m_heap->isEmpty())
			break;

        nextPixel = m_heap->top();
		if (nextPixel.val() < currPixel.val())
			descend(nextPixel);
	}

	finishStack();

	delete  m_heap;
	delete  m_connectivity;

    TimerWrapper::TimerInstance()->stopTimer("Flooding the MaxTree");
}
