#pragma once
#include <string>

namespace Game::Components
{
    struct Item
    {
        std::string Name{"Item"};
        int Count = 1; // Current amount of it. This can be infinitely big
        int StackCount = 16; // Max count before it creates a new stack. This will NOT force a new entity to be created
        int id = 0;
    };
}