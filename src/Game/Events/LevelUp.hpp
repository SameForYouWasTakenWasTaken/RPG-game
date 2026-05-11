#pragma once

#include "vendor/entt/entt.hpp"
#include "Engine/Events/IEvent.hpp"

namespace Game::Events
{
    class LevelUp : public Core::Events::IEvent
    {

    public:
        entt::entity entity{entt::null};
        uint32_t newLevel;
        
        LevelUp(entt::entity entity, uint32_t level)
        : entity(entity), newLevel(level) {}

        EVENT_TYPE(LevelUp);
    };
}