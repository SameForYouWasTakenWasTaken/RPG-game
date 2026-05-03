#pragma once

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
        int Level = 1;
        int XP = 0;

        int Gold = 0;

        bool CanEarnXP = true;
    };

    // Reward component for any task
    struct Reward
    {
        int Gold = 0;
        int XP = 0;
    };
}