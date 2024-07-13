#include <iostream>
#include "Manager.h"

int main()
{
    Manager manager = Manager(std::make_shared<Grid>(10), Difficulty::MEDIUM);
    manager.Start();
    
    return 0;
}