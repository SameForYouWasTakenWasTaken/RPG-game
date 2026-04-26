#pragma once

namespace Game::Components
{
    struct Humanoid
    {
        float Health = 100;
        float BaseDamage = 10.f;
        float Speed = 350.f;
        float RunSpeed = Speed * 1.2;
    };
}