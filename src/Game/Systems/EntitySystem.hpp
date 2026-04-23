#pragma once

#include "vendor/entt/entt.hpp"

#include "Engine/Systems/ISystem.hpp"
namespace Game::Systems
{
    class EntitySystem : public Core::Systems::ISystem
    {
        
    public:
        EntitySystem() = default;
    };
}