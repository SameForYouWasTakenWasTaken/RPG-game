#pragma once

#include "Engine/Events/EventBus.hpp"
#include "Engine/Events/IEvent.hpp"

namespace Game::Events
{
    class Died : public Core::Events::IEvent
    {
    public:
        entt::entity deadEntity;

        Died(entt::entity diedEntity) : deadEntity(diedEntity) {}
    };
}