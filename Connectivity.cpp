#include "include/Connectivity.h"

Connectivity::Connectivity()
{
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
}

Connectivity::~Connectivity()
{
	delete [] m_matrix;
}
