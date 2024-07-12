#include "Grid.h"

#include <iostream>
#include <algorithm>

Grid::Grid(unsigned char dim)
	: m_dim(dim)
{
	m_grid = new Tile*[dim];
	for (int i = 0; i < dim; i++)
	{
		m_grid[i] = new Tile[dim];
		for (int j = 0; j < dim; j++)
			m_grid[i][j] = Tile{ i * m_dim + j, 0, false, false, true };
	}
}

Grid::~Grid()
{
	for (int i = 0; i < m_dim; i++)
		delete[] m_grid[i];
	delete[] m_grid;
}

Tile** Grid::GetGrid()
{
	return m_grid;
}

void Grid::GenerateBombs(std::vector<int> index_blacklist, unsigned int seed)
{
	srand(seed);
}
