#pragma once
#include <memory>
#include <utility>

#include "Grid.h"

enum class Difficulty { EASY = 1, MEDIUM, HARD };
enum class Move { CLICK, FLAG };

class Manager
{
private:
	enum class GameState { PLAYING, WIN, LOSS };

	std::shared_ptr<Grid> m_grid;
	Difficulty m_difficulty;
	GameState game_state;

public:

	Manager(std::shared_ptr<Grid> grid, Difficulty difficulty);
	void Start();

private:
	void PrintGrid();
	std::pair<Move, int> GetInput();
	void Initialize();
	void Play();
};