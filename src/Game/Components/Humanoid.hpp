#pragma once

namespace Game::Components
{
    struct Humanoid
    {
        float MaxHealth = 100.f;
        float Health = 100.f;
        float BaseDamage = 10.f;
        
        float Speed = 350.f;
        float RunSpeed = Speed * 1.2f;

        bool CanRegenerateHP = true;
        float RegenerateSpeed = 10.f; // In seconds
        float RegenerateHP = 10.f;

        float timer_HealthRegen = 0.f; // Don't touch this.
    };
}