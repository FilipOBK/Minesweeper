#pragma once
#include <vector>
#include <utility>

#include "Tile.h"

class Grid
{
private:
	unsigned char m_dim;
	Tile** m_grid;

public:
	Grid(unsigned char dim);
	~Grid();

	Tile** GetGrid();
	unsigned char GetDim();
	inline int NumTiles() const { return m_dim * m_dim; }

	inline bool OnTop(const Tile& tile) const { return tile.index % m_dim == 0; }
	inline bool OnLeft(const Tile& tile) const { return tile.index < m_dim; }
	inline bool OnRight(const Tile& tile) const { return tile.index >= (m_dim - 1) * m_dim; }
	inline bool OnBottom(const Tile& tile) const { return tile.index % m_dim == m_dim - 1; }

	bool Uncover(int index);
	inline void Flag(int index)
	{
		auto [row, col] = GetGridIndices(index);
		m_grid[row][col].Flag();
	}

	void GenerateBombs(unsigned int quantity, std::vector<int> index_blacklist, unsigned int seed);
private:
	void UncoverSurrounding(int index);
	inline std::pair<int, int> GetGridIndices(int index)
		{ return std::pair<int, int>(static_cast<int>(index / m_dim), index % m_dim); }

	void CalculateSurrounding();
};