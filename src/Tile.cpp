#include "Tile.h"

std::string Tile::GetChar()
{
    if (flagged) return "\033[1;33mF\033[0m";
    if (covered) return " ";
    if (bomb) return "\033[1;31mB\033[0m";
    switch (surrounding)
    {
        case 0:
            return "0";
        case 1:
            return "\033[1;94m1\033[0m";
        case 2:
            return "\033[1;32m2\033[0m";
        case 3:
            return "\033[1;31m3\033[0m";
        case 4:
            return "\033[1;35m4\033[0m";
        case 5:
            return "\033[1;33m5\033[0m";
        case 6:
            return "\033[1;33m6\033[0m";
        case 7:
            return "\033[1;33m7\033[0m";
        case 8:
            return "\033[1;33m8\033[0m";
        default:
            return "!";
    }
}

void Tile::Flag()
{
    if (covered) flagged = !flagged;
}

void Tile::Uncover()
{
    covered = false;
}
