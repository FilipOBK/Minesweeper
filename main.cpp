#include <iostream>
#include <algorithm>
#include <cmath>
#include <regex>
#include <ctime>
#include <utility>


const int DIM = 4;
const int NUM_OF_BOMBS = DIM * DIM / 4;
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
        if(isCovered) return " ";
        if(isBomb) return "\033[1;31mB\033[0m";
        switch(surrounding){
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

    void flag() {
        if(isCovered) isFlag = isFlag ? false : true;
    }
    void uncover();
    void uncoverSurrounding();

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
    if(isCovered) numCovered--;
    isCovered = false;
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

void Tile::uncoverSurrounding(){

    int index = this - &grid[0];

    if(!onTop() && !grid[index - DIM].isBomb) { grid[index - DIM].uncover(); }
    if(!onLeftEdge() && !grid[index - 1].isBomb) { grid[index - 1].uncover(); }
    if(!onRightEdge() && !grid[index + 1].isBomb) { grid[index + 1].uncover(); }
    if(!onBottom() && !grid[index + DIM].isBomb) { grid[index + DIM].uncover(); }

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
    std::pair<char, int> getPos(){
        std::string pos;
        int row, col = 0;
        std::regex inputRegex ("[CF][A-Z][1-9][0-9]?$");
        bool validForm = false;
        bool validPos = false;
        do{
            std::cout << "Enter a position: ";
            std::cin >> pos;
            validForm = std::regex_match(pos, inputRegex);
            
            if(validForm){

                // get the row #
                row = (int(pos[2]) - 48);
                if(pos[3]){ row *= 10; row += int(pos[3]) - 48; }

                for(int i = 0; i < DIM; i++){
                    if(pos[1] == ALPHABET[i]) col = i + 1;
                }

                // if the column is still 0 after checking the alphabet, it is an invalid col
                if(col == 0) col = 100;

                validPos = (row <= DIM && col <= DIM);
            }
        } while (!(validForm && validPos));

        int index = (row - 1) * DIM + col - 1;
        char action = pos[0];
        return std::pair<char,int>(action,index);
    }

    void firstClick(){

        std::pair<char, int> input = getPos();
        int index = input.second;
        Tile* tile = &grid[index];

        if(input.first == 'F') {
            tile->flag();
            return;
        }

        tile->uncover();
        tile->uncoverSurrounding();

        genBombs();

        int extraIndex = rand() % 4;
        switch(extraIndex){
            case 0:
            extraIndex = DIM * -1;
            break;
            case 1:
            extraIndex = -1;
            break;
            case 2:
            extraIndex = 1;
            break;
            case 3:
            extraIndex = DIM;
        }

        tile += extraIndex;
        tile->uncoverSurrounding();


    }

    void getInput(){
        std::pair<char, int> input = getPos();

        if(input.first == 'C') grid[input.second].uncover();
        if(input.first == 'F') grid[input.second].flag();
        
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
        for(int i = 0; i < DIM * DIM; i++){
            if(!grid[i].isCovered) grid[i].uncover();
        }
    }

    void checkWin(){ if(numCovered == NUM_OF_BOMBS) gameState = WIN; }
};

int main(){
    srand(time(NULL));
    GameController controller;
    
    printGrid();
    controller.firstClick();

    while(gameState == PLAYING){
        printGrid();
        controller.getInput();
        controller.checkWin();
    }
    GAMESTATE store = gameState;
    for(int i = 0; i < DIM * DIM; i++) {
        grid[i].uncover();
    }
    printGrid();
    std::cout << (store == WIN ? "VICTORY" : "Game Over") << std::endl;
}