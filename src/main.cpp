#include <iostream>
#include "Manager.h"

int main()
{
    Manager manager = Manager(std::make_shared<Grid>(5), Difficulty::EASY);

    std::cin.get();
    return 0;
}