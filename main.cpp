#include <iostream>
#include <algorithm>
#include <cmath>
#include <regex>
#include <ctime>


const int DIM = 10;
const int NUM_OF_BOMBS = DIM * DIM / 3;
const std::string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
enum GAMESTATE {PLAYING, WIN, LOSS};

int numCovered = DIM * DIM;
GAMESTATE gameState = PLAYING;

class Tile{

public:
    bool isFlag;
    bool isCovered;
    int surrounding;
    bool isBomb;

    Tile(){
        surrounding = 0;
        isBomb = false;
        isFlag = false;
        isCovered = true;
    }

    std::string getChar(){
        if(isFlag) return "\033[1;33mF\033[0m";
        if(isCovered) return "#";
        if(isBomb) return "\033[1;31mB\033[0m";
        return surrounding == 0 ? " " : std::to_string(surrounding);
    }

    void flag() { isFlag = true; }
    void uncover();

    bool onLeftEdge();
    bool onRightEdge();
    bool onTop();
    bool onBottom();
};

Tile grid[DIM * DIM];

bool Tile::onLeftEdge(){ int index = this - &grid[0]; return (index % DIM == 0); }
bool Tile::onRightEdge(){ int index = this - &grid[0]; return (index % DIM == DIM - 1); }
bool Tile::onTop(){ int index = this - &grid[0]; return (index % DIM == index); }
bool Tile::onBottom(){ int index = this - &grid[0]; return (index + DIM) > (DIM * DIM - 1); }

void Tile::uncover(){
    if(isFlag) return;
    isCovered = false;
    numCovered--;
    if(isBomb) { gameState = LOSS; return; }
    
    int index = this - &grid[0];
    int counter = 0;

    if(grid[index - DIM - 1].isBomb && !onLeftEdge() && !onTop()) counter++;
    if(grid[index - DIM].isBomb && !onTop()) counter++;
    if(grid[index - DIM + 1].isBomb && !onRightEdge() && !onTop()) counter++;

    if(grid[index - 1].isBomb && !onLeftEdge()) counter++;
    if(grid[index + 1].isBomb && !onRightEdge()) counter++;

    if(grid[index + DIM - 1].isBomb && !onLeftEdge() && !onBottom()) counter++;
    if(grid[index + DIM].isBomb&& !onBottom()) counter++;
    if(grid[index + DIM + 1].isBomb && !onRightEdge() && !onBottom()) counter++;

    surrounding = counter;
}

void printGrid(){
    std::cout << "  ";
    for(int i = 0; i < DIM; i++) {std::cout << "  " << ALPHABET[i] << " ";}
    std::cout << std::endl;

    for(int i = 0; i < DIM; i++){ std::cout << "----"; } std::cout << "---\n";

    for(int row = 0; row < DIM; row++){
        std::cout << row + 1 << (row + 1 < 10 ? " " : "") << "|";
        for(int col = 0; col < DIM; col++){
            int index = col + (row * DIM);

            std::cout << " " << grid[index].getChar() << " |";
        }

        std::cout << std::endl;
        for(int i = 0; i < DIM; i++){ std::cout << "----"; }
        std::cout << "---" << std::endl;
    }
}

class GameController{

public:
    void getInput(){
        std::string pos;
        int row, col;
        std::regex inputRegex ("[CF][A-Z][1-9][0-9]?$");
        bool validForm = false;
        bool validPos = false;
        do{
            std::cout << "Enter a position: ";
            std::cin >> pos;
            validForm = std::regex_match(pos, inputRegex);
            
            if(validForm){

                row = (int(pos[2]) - 48);
                if(pos[3]){ row *= 10; row += int(pos[3]) - 48; }

                for(int i = 0; i < DIM; i++){
                    if(pos[1] == ALPHABET[i]) col = i + 1;
                }

                validPos = (row <= DIM && col <= DIM);
            }
        } while (!validForm || !validPos);
        
        int index = (row - 1) * DIM + col - 1;

        if(pos[0] == 'C') grid[index].uncover();
        if(pos[0] == 'F') grid[index].flag();
        
    }

    void genBombs(){
        int numPlaced = 0;
        while(numPlaced < NUM_OF_BOMBS){

            int index = rand() % (DIM * DIM - 1);
            if(!grid[index].isBomb && grid[index].isCovered) {
                grid[index].isBomb = true;
                numPlaced++;
            }

        }
    }
};

int main(){
    srand(time(NULL));
    GameController controller;
    
    printGrid();
    controller.getInput();
    controller.genBombs();
    
    while(gameState == PLAYING){
        printGrid();
        controller.getInput();
    }

    for(int i = 0; i < DIM * DIM; i++) {
        grid[i].uncover();
    }
    printGrid();
    std::cout << (gameState == WIN ? "VICTORY" : "Game Over") << std::endl;
}