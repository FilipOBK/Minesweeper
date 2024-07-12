#pragma once
#include <memory>

#include "Grid.h"

enum class Difficulty { EASY, MEDIUM, HARD };

class Manager
{
private:
	std::shared_ptr<Grid> m_grid;
	Difficulty m_difficulty;

public:
	enum class GameState { PLAYING, WIN, LOSS };

	Manager(std::shared_ptr<Grid> grid, Difficulty difficulty);
private:
};