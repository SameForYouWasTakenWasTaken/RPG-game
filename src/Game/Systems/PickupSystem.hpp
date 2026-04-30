#pragma once

#include "Engine/Systems/ISystem.hpp"
#include "vendor/entt/entt.hpp"
namespace Game::Systems
{
    class Pickup : public Core::Systems::ISystem
    {
        entt::registry& m_SceneRegistry;
    public:
        /**
 * @brief Constructs the Pickup system with a scene entity registry reference.
 *
 * @param registry Reference to the entt::registry used to query and modify scene entities.
 */
Pickup(entt::registry& registry) : m_SceneRegistry(registry) {}

        [[nodiscard]] bool PickUp(entt::entity item, entt::entity owner);
        bool IsPickable(entt::entity item);

        void OnUpdate(float dt) override;
    };
}