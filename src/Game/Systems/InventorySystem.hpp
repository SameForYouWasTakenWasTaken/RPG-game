#pragma once

#include "Engine/Systems/ISystem.hpp"
#include "vendor/entt/entt.hpp"

namespace Game::Systems
{
    class Inventory : public Core::Systems::ISystem
    {
        entt::registry& m_SceneRegistry;
        
        static entt::entity FindItemByType(entt::registry& r, entt::entity owner, int itemID);
    public:
        /**
        * @brief Constructs an Inventory system tied to the given entity registry.
        *
        * @param registry Reference to the scene's entt::registry used by this system.
        */
        Inventory(entt::registry& registry) : m_SceneRegistry(registry) {}
        
        
        [[nodiscard]] static bool AddItem(entt::registry& r, entt::entity inventoryOwner, entt::entity item);
        static void RemoveItem(entt::registry& r, entt::entity inventoryOwner, entt::entity item);
        
        static bool IsItem(entt::registry& r, entt::entity item);
        
        static bool IsOwnedBy(entt::registry& r, entt::entity owner, entt::entity item);

        [[nodiscard]] bool AddItem(entt::entity inventoryOwner, entt::entity item);
        
        void RemoveItem(entt::entity inventoryOwner, entt::entity item);
        bool IsItem(entt::entity item);

        bool IsOwnedBy(entt::entity owner, entt::entity item);
    };
}