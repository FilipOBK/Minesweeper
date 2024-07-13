#include <iostream>
#include "Manager.h"

int main()
{
    Manager manager = Manager(std::make_shared<Grid>(5), Difficulty::EASY);
    manager.Start();
    
    std::cin.get();
    return 0;
}