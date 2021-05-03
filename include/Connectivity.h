#ifndef CONNECT_H
#define CONNECT_H

class Connectivity {

public:
	Connectivity();
	~Connectivity();

	bool check(int x, int y) { return m_matrix[y * m_width + x]; }
	int width() { return m_width; }
	int height() { return m_height; }

private:
	bool *m_matrix = nullptr;
	int m_width = 0;
	int m_height = 0;
	int m_size = 0;
};

#endif
