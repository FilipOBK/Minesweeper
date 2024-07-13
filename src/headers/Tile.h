#pragma once

#include <string>

struct Tile
{
	int index;
	int surrounding;

	bool bomb;
	bool flagged;
	bool covered;

	std::string GetChar();
	void Flag();
	void Uncover();
};