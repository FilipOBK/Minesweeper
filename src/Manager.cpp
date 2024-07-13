#include "Manager.h"

#include <iostream>
#include <regex>
#include <ctime>
#include <vector>

Manager::Manager(std::shared_ptr<Grid> grid, Difficulty difficulty)
	: m_grid(grid), m_difficulty(difficulty)
{

}

void Manager::Start()
{
    Initialize();
    Play();
    End();
}

void Manager::Initialize()
{
    float bomb_density = 0.09f * (int)m_difficulty; // 18% of tiles should be bombs on medium difficulty
    unsigned int bomb_quantity = m_grid->GetDim() * m_grid->GetDim() * bomb_density;
    m_grid->SetNumBombs(bomb_quantity);

    unsigned int seed = time(NULL);

    PrintGrid();
    auto [move, index] = GetInput();
    game_state = move == Move::QUIT ? GameState::LOSS : GameState::PLAYING;

    std::vector<int> index_blacklist;
    index_blacklist.push_back(index);
    for (auto tile : m_grid->Surrounding(index))
        index_blacklist.push_back(tile->index);

    m_grid->GenerateBombs(index_blacklist, seed);
    m_grid->Uncover(index);
}

void Manager::Play()
{
    while (game_state == GameState::PLAYING)
    {
        PrintGrid();
        auto [move, index] = GetInput();

        if (move == Move::FLAG)
        {
            m_grid->Flag(index);
            continue;
        }

        if (m_grid->Uncover(index) || move == Move::QUIT)
        {
            game_state = GameState::LOSS;
            continue;
        }
        
        if (m_grid->NumTiles() - m_grid->GetNumUncovered() <= m_grid->GetNumBombs())
            game_state = GameState::WIN;
    }
}

void Manager::End()
{
    std::cout << (game_state == GameState::WIN ? "VICTORY" : "GAME OVER") << "\n";
}

void Manager::PrintGrid()
{
    const int DIM = m_grid->GetDim();

    std::cout << "  ";
    for (int i = 0; i < DIM; i++) { std::cout << "  " << (char)('A' + i) << " "; }
    std::cout << std::endl;

    for (int i = 0; i < DIM; i++) { std::cout << "----"; } std::cout << "---\n";

    for (int row = 0; row < DIM; row++)
    {
        std::cout << row + 1 << (row + 1 < 10 ? " " : "") << "|";
        for (int col = 0; col < DIM; col++)
        {
            std::string chr = m_grid->GetGrid()[row][col].GetChar();
            std::cout << " " << chr << " |";
        }

        std::cout << std::endl;
        for (int i = 0; i < DIM; i++) { std::cout << "----"; }
        std::cout << "---" << std::endl;
    }
    std::cout << "Flags: " << m_grid->GetNumBombs() - m_grid->GetNumFlags() << "\n";
}

std::pair<Move, int> Manager::GetInput()
{
    auto input_regex = std::regex("[CF][A-Z][1-9][0-9]?$");
    bool valid_form = false, valid_pos = false;
    int row{ 0 }, col{ 0 };
    std::string input;

    while (!valid_form || !valid_pos)
    {
        std::cout << "Enter move: ";
        std::cin >> input;

        if (input[0] == 'Q') // If you wanna quit, no need for validity
            break;
        valid_form = std::regex_match(input, input_regex);
        if (!valid_form) 
            continue;


        row = (int)(input[2]) - '0';
        if (input[3])
        {
            row *= 10;
            row += (int)(input[3] - '0');
        }
        row--; // Now 0-indexed

        char max_col = std::min('A' + m_grid->GetDim() - 1, (int)'Z');
        if (input[1] > max_col)
            continue;
        col = input[1] - 'A';
        valid_pos = row < m_grid->GetDim() && col < m_grid->GetDim();
    }

    Move move = Move(input[0]);
    int index = row * m_grid->GetDim() + col;
    return std::pair<Move, int>(move, index);
}

