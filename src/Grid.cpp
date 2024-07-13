#include "Grid.h"

#include <iostream>
#include <algorithm>

Grid::Grid(unsigned char dim)
	: m_dim(dim), num_flags(0), num_uncovered(0)
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

unsigned int Grid::GetNumBombs()
{
	return num_bombs;
}

void Grid::SetNumBombs(unsigned int num_bombs)
{
	this->num_bombs = num_bombs;
}

unsigned int Grid::GetNumFlags()
{
	return num_flags;
}

unsigned int Grid::GetNumUncovered()
{
	return num_uncovered;
}

bool Grid::Uncover(int index)
{
	auto [row, col] = GetGridIndices(index);
	if (!m_grid[row][col].covered || m_grid[row][col].flagged)
		return false;

	m_grid[row][col].Uncover();
	num_uncovered++;
	if (m_grid[row][col].bomb)
		return true;		

	if (m_grid[row][col].surrounding == 0)
		UncoverSurrounding(index);

	return false;
}

void Grid::Flag(int index)
{
	auto [row, col] = GetGridIndices(index);
	if (!m_grid[row][col].covered)
		return;

	m_grid[row][col].Flag();
	num_flags += m_grid[row][col].flagged ? 1 : -1;
}

void Grid::GenerateBombs(std::vector<int> index_blacklist, unsigned int seed)
{
	int available_tiles = (m_dim * m_dim) - index_blacklist.size();
	if (num_bombs > available_tiles)
		throw std::domain_error("Not enough tiles to place bombs");

	srand(seed);

	for (int i = 0; i < num_bombs; i++)
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
	for (auto tile : Surrounding(index))
		Uncover(tile->index);
}

std::vector<Tile*> Grid::Surrounding(int index)
{
	std::vector<Tile*> res;
	auto [row, col] = GetGridIndices(index);
	if (row > 0) // Not on top of board
	{
		res.push_back(&m_grid[row - 1][col]);
		if (col > 0)
			res.push_back(&m_grid[row - 1][col - 1]);
		if (col < m_dim - 1)
			res.push_back(&m_grid[row - 1][col + 1]);
	}
	if (row < m_dim - 1) // Not on bottom of board
	{
		res.push_back(&m_grid[row + 1][col]);
		if (col > 0)
			res.push_back(&m_grid[row + 1][col - 1]);
		if (col < m_dim - 1)
			res.push_back(&m_grid[row + 1][col + 1]);
	}
	if (col > 0)
		res.push_back(&m_grid[row][col - 1]);
	if (col < m_dim - 1)
		res.push_back(&m_grid[row][col + 1]);

	return res;
}

void Grid::CalculateSurrounding()
{
	for (int i = 0; i < NumTiles(); i++)
	{
		auto [row, col] = GetGridIndices(i);
		if (!m_grid[row][col].bomb)
			continue;

		for (auto tile : Surrounding(i))
			tile->surrounding++;
	}
}
