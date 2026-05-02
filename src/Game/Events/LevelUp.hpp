#pragma once

#include "vendor/entt/entt.hpp"
#include "Engine/Events/IEvent.hpp"

namespace Game::Events
{
    class LevelUp : public Core::Events::IEvent
    {

    public:
        entt::entity entity{entt::null};
        int newLevel;
        
        LevelUp(entt::entity entity, int level)
        : entity(entity), newLevel(level) {}

        EVENT_TYPE(LevelUp);
    };
}