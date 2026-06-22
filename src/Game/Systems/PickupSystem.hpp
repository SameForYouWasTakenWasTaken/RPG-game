#pragma once

#include "Engine/Systems/ISystem.hpp"
#include "SpatialGrid.hpp"
#include "vendor/entt/entt.hpp"
namespace Game::Systems
{
    class Pickup : public Core::Systems::ISystem
    {
        entt::registry& m_SceneRegistry;
        SpatialGrid& m_SpatialGrid;
    public:
        /**
        * @brief Constructs the Pickup system with a scene entity registry reference.
        *
        * @param registry Reference to the entt::registry used to query and modify scene entities.
        * @param spatialGrid Reference to the SpatialGrid used for spatial queries.
        */
        Pickup(entt::registry& registry, SpatialGrid& spatialGrid) 
        : m_SceneRegistry(registry), m_SpatialGrid(spatialGrid) {}

        [[nodiscard]] static bool PickUp(entt::registry& r, entt::entity item, entt::entity owner);
        [[nodiscard]] static bool IsPickable(entt::registry& r, entt::entity item);

        [[nodiscard]] bool PickUp(entt::entity item, entt::entity owner);
        [[nodiscard]] bool IsPickable(entt::entity item);

        void OnUpdate(float dt) override;
    };
}