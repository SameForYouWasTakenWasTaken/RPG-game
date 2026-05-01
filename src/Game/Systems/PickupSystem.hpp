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

        [[nodiscard]] static bool PickUp(entt::registry& r, entt::entity item, entt::entity owner);
        [[nodiscard]] static bool IsPickable(entt::registry& r, entt::entity item);

        [[nodiscard]] inline bool PickUp(entt::entity item, entt::entity owner);
        [[nodiscard]] inline bool IsPickable(entt::entity item);

        void OnUpdate(float dt) override;
    };
}