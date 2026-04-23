#pragma once

namespace Game::Components
{
    struct Attacking{
        float timer = 0.0f;
        float hitStart = 0.2f;
        float hitEnd = 0.4f;
        bool hitRegistered = false;
        float elapsedCooldownCount = 0.f;
    };
    
    struct Attacked{};
}