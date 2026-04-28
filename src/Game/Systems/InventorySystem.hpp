#pragma once

#include "Engine/Systems/ISystem.hpp"
#include "vendor/entt/entt.hpp"

namespace Game::Systems
{
    class Inventory : public Core::Systems::ISystem
    {
        entt::registry& m_SceneRegistry;
        entt::entity FindItemByType(entt::entity owner, int itemID);
    public:
        Inventory(entt::registry& registry) : m_SceneRegistry(registry) {}
        
        void AddItem(entt::entity inventoryOwner, entt::entity item);
        void RemoveItem(entt::entity inventoryOwner, entt::entity item);
        bool IsItem(entt::entity item);

    };
}