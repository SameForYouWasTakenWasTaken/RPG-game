#pragma once
#include "Components/Progression.hpp"
#include "Engine/Events/IEvent.hpp"
#include "vendor/entt/entt.hpp"
namespace Game::Events
{
    class Reward
    {

    public:
        entt::entity target;
        Game::Components::Reward reward;

        Reward(entt::entity target, Game::Components::Reward reward)
        : target(target), reward(reward) {}
        
        EVENT_TYPE(Reward);
    };
}