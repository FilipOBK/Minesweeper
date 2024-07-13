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

unsigned char Grid::GetDim()
{
	return m_dim;
}

bool Grid::Uncover(int index)
{
	auto [row, col] = GetGridIndices(index);
	if (!m_grid[row][col].covered)
		return;

	m_grid[row][col].Uncover();
	if (m_grid[row][col].bomb)
		return false;


	return true;
}

void Grid::GenerateBombs(unsigned int quantity, std::vector<int> index_blacklist, unsigned int seed)
{
	int available_tiles = (m_dim * m_dim) - index_blacklist.size();
	if (quantity > available_tiles)
		throw std::domain_error("Not enough tiles to place bombs");

	srand(seed);

	for (int i = 0; i < quantity; i++)
	{
		int index = static_cast<int>(rand() % NumTiles());
		bool invalid = std::find(index_blacklist.begin(), index_blacklist.end(), index) != index_blacklist.end();
		if (invalid)
		{
			i--;
			continue;
		}
		
		auto [row, col] = GetGridIndices(index);
		m_grid[row][col].bomb = true;
		index_blacklist.push_back(index); // Disallow same index to 'bombed' again
	}

	CalculateSurrounding();
}

void Grid::UncoverSurrounding(int index)
{
	// TODO
}

void Grid::CalculateSurrounding()
{
	for (int row = 0; row < m_dim; row++)
	{
		for (int col = 0; col < m_dim; col++)
		{
			if (!m_grid[row][col].bomb)
				continue;

			if (row > 0) // Not on top of board
			{
				m_grid[row - 1][col].surrounding++;
				if (col > 0)
					m_grid[row - 1][col - 1].surrounding++;
				if (col < m_dim - 1)
					m_grid[row - 1][col + 1].surrounding++;
			}
			if (row < m_dim - 1) // Not on bottom of board
			{
				m_grid[row + 1][col].surrounding++;
				if (col > 0)
					m_grid[row + 1][col - 1].surrounding++;
				if (col < m_dim - 1)
					m_grid[row + 1][col + 1].surrounding++;
			}
			if (col > 0)
				m_grid[row][col - 1].surrounding++;
			if (col < m_dim - 1)
				m_grid[row][col + 1].surrounding++;
		}
	}
}
