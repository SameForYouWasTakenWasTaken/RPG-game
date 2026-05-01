#pragma once

#include "vendor/entt/entt.hpp"

namespace Game::Components
{
    struct Weapon
    {
        float Damage = 5.f;
        float AttackRange = 300.f;

        // Hit window
        float HitWindowBegin = 0.2f;
        float HitWindowEnd = 0.4f;

        float AttackCooldown = 3.f; // How long between each attack

        float cooldownTimer = 0.f; // Timer, don't change

        bool hitRegistered = false;
    };
}