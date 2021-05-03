#ifndef NODE_H
#define NODE_H

#define UNASSIGNED -1
#define IN_QUEUE -2
#define NO_PARENT -3

class Node
{

public:
	Node();
	~Node();

	long parent() { return m_parent; }
	long area() { return m_area; }
	float volume() { return m_volume; }
	float power() { return m_power; }

	void setParent(long index) { m_parent = index; }
	void setArea(long area) { m_area = area; }
	void setVolume(float volume) { m_volume = volume; }
	void setPower(float power) { m_power = power; }

private:
	long m_parent = UNASSIGNED;
	long m_area = 1;
	float m_volume = 0;
	float m_power = 0;
};

#endif
