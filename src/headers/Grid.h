#pragma once
#include <vector>
#include <utility>

#include "Tile.h"

class Grid
{
private:
	unsigned char m_dim;
	unsigned int num_bombs, num_flags, num_uncovered;
	Tile** m_grid;

public:
	Grid(unsigned char dim);
	~Grid();

	Tile** GetGrid();
	unsigned char GetDim();
	unsigned int GetNumBombs();
	void SetNumBombs(unsigned int num_bombs);
	unsigned int GetNumFlags();
	unsigned int GetNumUncovered();
	inline int NumTiles() const { return m_dim * m_dim; }

	bool Uncover(int index);
	void Flag(int index);

	void GenerateBombs(std::vector<int> index_blacklist, unsigned int seed);
	std::vector<Tile*> Surrounding(int index);
private:
	void UncoverSurrounding(int index);
	inline std::pair<int, int> GetGridIndices(int index)
		{ return std::pair<int, int>(static_cast<int>(index / m_dim), index % m_dim); }

	void CalculateSurrounding();
};