#pragma once

#include <cstdint>

namespace Game::Components
{
    struct CombatStats
    {
        float Strength = 1.f;
        float Dexterity = 1.f;
        float Vitality = 1.f;
    };

    struct Progression
    {
        uint32_t Level = 1;
        uint32_t XP = 0;

        uint32_t Gold = 0;

        bool CanEarnXP = true;
    };

    // Reward component for any task
    struct Reward
    {
        uint32_t Gold = 0;
        uint32_t XP = 0;
    };
}