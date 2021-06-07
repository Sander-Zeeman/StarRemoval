#ifndef NODE_H
#define NODE_H

#define UNASSIGNED -1
#define IN_QUEUE -2
#define NO_PARENT -3
#define NO_OBJECT -4

#include "Debug.h"

class Node
{
public:
	Node() {};
	~Node() {};

	bool isSignificant() { return m_flags & 1; }
	void setSignificant(bool b) { setFlag(b, 1); }

	bool isObject() { return m_flags & 2; }
	void setObject(bool b) { setFlag(b, 2); }

	bool starChecked() { return m_flags & 4; }
	void setCheckedForStar(bool b) { setFlag(b, 4); }

	bool sigAncestorChecked() { return m_flags & 8; }
	void setCheckedForSigAncestor(bool b) { setFlag(b, 8); }

	bool hasSigDescendent() { return m_flags & 16; }
	void setHasSigDescendent(bool b) { setFlag(b, 16); }

	bool hasDescendent() { return m_flags & 32; }
	void setHasDescendent(bool b) { setFlag(b, 32); }

	bool isStar() { return m_flags & 64; }
	void setStar(bool b) { setFlag(b, 64); }

	long parent() { return m_parent; }
	float height() { return m_height; }
	float top() { return m_top; }
	long area() { return m_area; }
	float volume() { return static_cast<float>(m_area) * m_height; }
	float power() { return m_power; }

	void setParent(long index) { m_parent = index; }
    void setHeight(float height) { m_height = height; }
    void setTop(float top) { m_top = top; }
	void setArea(long area) { m_area = area; }
    void setVolume(float volume) { m_volume = volume; }
	void setPower(float power) { m_power = power; }

private:
	void setFlag(bool b, uint8_t pos) { b ? setFlagTrue(pos) : setFlagFalse(pos); }
	void setFlagFalse(uint8_t pos) { m_flags = static_cast<uint8_t>(~(~m_flags | pos)); }
	void setFlagTrue(uint8_t pos) { m_flags |= pos; }

	uint8_t m_flags = 0;
	long m_parent = UNASSIGNED;
	long m_area = 1;
	float m_height = 0.0f;
    float m_top = UNASSIGNED;
    float m_volume = 0.0f;
	float m_power = 0.0f;
};

#endif
