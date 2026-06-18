#pragma once
#include <string>
#include <vector>

namespace Game::API
{
    
    struct TileLayer
    {
        using Tiles = std::vector<int>;   
        std::string name;

        int width = 0;
        int height = 0;
        bool visible = true;

        Tiles data; // tile IDs
    };
}