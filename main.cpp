#include <iostream>
#include <algorithm>
#include <cmath>
#include <regex>
#include <ctime>


const int DIM = 10;
const int NUM_OF_BOMBS = DIM * DIM / 3;
const std::string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const enum GAMESTATE {PLAYING, WIN, LOSS};

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
        if(isFlag) return "F";
        if(isCovered) return "#";
        if(isBomb) return "B";
        return surrounding == 0 ? " " : std::to_string(surrounding);
    }

    void flag() { isFlag = true; }
    void uncover();
};

Tile grid[DIM * DIM];

void updateSurrounding(int index){
    std::cout << "calculating surrounding\n";
    int surr = 0;
    for(int i = -1; i < 2; i++){
        if(grid[index - DIM - i].isBomb) surr++;
    }
    if(grid[index-1].isBomb) surr++;
    if(grid[index+1].isBomb) surr++;
    for(int i = -1; i < 2; i++){
        if(grid[index + DIM - i].isBomb) surr++;
    }
    grid[index].surrounding = surr;
}

void Tile::uncover(){
    isCovered = false;
    numCovered--;
    if(isBomb) { gameState = LOSS; return; }
    updateSurrounding(this - &grid[0]);
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
        grid[i].isCovered = false;
    }
    printGrid();
    std::cout << (gameState == WIN ? "VICTORY" : "Game Over") << std::endl;
}