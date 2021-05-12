#include "../include/Connectivity.h"

Connectivity::Connectivity()
{
/*
    Using the following connectivity matrix:
                0 1 0
                1 0 1
                0 1 0
*/

	m_size = 9;
	m_height = 3;
	m_width = 3;
	m_matrix = new bool[m_size];

	m_matrix[0] = 0;
	m_matrix[1] = 1;
	m_matrix[2] = 0;
	m_matrix[3] = 1;
	m_matrix[4] = 0;
	m_matrix[5] = 1;
	m_matrix[6] = 0;
	m_matrix[7] = 1;
	m_matrix[8] = 0;

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

Connectivity::~Connectivity()
{
	delete [] m_matrix;
}
