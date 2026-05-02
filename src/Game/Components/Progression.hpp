#pragma once

namespace Game::Components
{
    struct Stats
    {
        float Strength = 0.f;
        float Dexterity = 0.f;
        float Vitality = 0.f;
    };

    struct Progression
    {
        int Level = 1;
        int XP = 0;

        bool CanEarnXP = true;
    };

    // Reward component for any task
    struct Reward
    {
        int Gold = 0;
        int XP = 0;
    };
}