#pragma once

#include <string>

namespace Game
{
    struct GameSettings
    {
        unsigned int windowWidth{};
        unsigned int windowHeight{};

        std::string windowTitle{};
    };
}