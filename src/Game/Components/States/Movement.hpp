#pragma once

#include "glm/glm.hpp"
namespace Game::Components
{
    struct Walking
    {
        glm::vec2 direction;

        Walking(glm::vec2 dir)
        {
            direction = glm::normalize(dir);
        }
    };

    struct Running : public Walking 
    {
        Running(glm::vec2 dir) : Walking(dir) {}
    };
    
    struct Idle{};

    // AI
    struct Chasing{};
}