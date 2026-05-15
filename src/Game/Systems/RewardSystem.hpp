#pragma once

#include "Engine/Events/EventBus.hpp"
#include "Engine/Systems/ISystem.hpp"
#include "Systems/PickupSystem.hpp"
#include <Components/Progression.hpp>

namespace Game::Systems
{
    class Reward : public Core::Systems::ISystem
    {
        entt::registry& m_SceneRegistry;
        Core::Events::EventBus& m_EventBus;    
    public:
        Reward(entt::registry& registry, Core::Events::EventBus& bus) :
            m_SceneRegistry(registry), m_EventBus(bus)  {}

        void Award(entt::entity, Components::Reward reward);
    };
}