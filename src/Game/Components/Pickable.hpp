#pragma once

namespace Game::Components
{
    struct Pickable
    {
        bool CanBePicked = true;
        float Radius = 150.f;
    };
}