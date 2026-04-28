#pragma once

#include "Engine/Systems/ISystem.hpp"
#include "vendor/entt/entt.hpp"
namespace Game::Systems
{
    class Pickup : public Core::Systems::ISystem
    {
        entt::registry& m_SceneRegistry;
    public:
        Pickup(entt::registry& registry) : m_SceneRegistry(registry) {}

        [[nodiscard]] bool PickUp(entt::entity item, entt::entity owner);
        bool IsPickable(entt::entity item);

        void OnUpdate(float dt) override;
    };
}