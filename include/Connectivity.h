#ifndef CONNECT_H
#define CONNECT_H

#include "Debug.h"

class Connectivity {
public:
	Connectivity() {
    	#ifdef DEBUG
        	std::cout << "Connectivity matrix used:" << std::endl;
        	for (int y = 0; y < m_height; y++) {
        		for (int x = 0; x < m_width; x++) {
        			std::cout << m_matrix[m_width * y + x] << " ";
        		}
        		std::cout << std::endl;
        	}
        	std::cout << std::endl;
        #endif
    }

	~Connectivity() {}

	bool check(int x, int y) { return m_matrix[y * m_width + x]; }
	int width() { return m_width; }
	int height() { return m_height; }

private:
	bool m_matrix[9] {
        false, true,  false,
        true,  false, true,
        false, true,  false
    };
	int m_width = 3;
	int m_height = 3;
	int m_size = 9;
};

#endif
