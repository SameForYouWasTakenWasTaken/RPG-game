#include "InventorySystem.hpp"

#include "Components/InventoryItem.hpp"
#include "Components/Inventory.hpp"

namespace Game::Systems
{
    void Inventory::AddItem(entt::entity owner, entt::entity item)
    {
        assert(owner != entt::null);
        assert(item != entt::null);
        assert(IsItem(item));

        auto& incoming = m_SceneRegistry.get<Game::Components::Item>(item);
        auto existing = FindItemByType(owner, incoming.id);

        if (existing != entt::null)
        {
            auto& existingItem = m_SceneRegistry.get<Game::Components::Item>(existing);
            existingItem.Count += incoming.Count;
            m_SceneRegistry.destroy(item);
        } else 
        {
            auto& inventory = m_SceneRegistry.get<Game::Components::Inventory>(owner);
            
            if (inventory.Items.size() >= inventory.Capacity)
                return;
            
            inventory.Items.push_back(item);
        }
    }

    void Inventory::RemoveItem(entt::entity owner, entt::entity item)
    {
        assert(owner != entt::null);
        assert(item != entt::null);
        assert(IsItem(item));

        auto& incoming = m_SceneRegistry.get<Game::Components::Item>(item);

        auto existing = FindItemByType(owner, incoming.id);
        if (existing == entt::null) return;
        
        auto& existingItem = m_SceneRegistry.get<Game::Components::Item>(existing);
        existingItem.Count -= incoming.Count;

        m_SceneRegistry.destroy(item);
        return;
    }

    bool Inventory::IsItem(entt::entity item)
    {
        assert(item != entt::null);
        return m_SceneRegistry.all_of<Game::Components::Item>(item);
    }

    entt::entity Inventory::FindItemByType(entt::entity owner, int itemID)
    {
        assert(owner != entt::null);

        auto& inventory = m_SceneRegistry.get<Game::Components::Inventory>(owner);

        for (auto entity : inventory.Items)
        {
            auto& item = m_SceneRegistry.get<Game::Components::Item>(entity);
            if (item.id == itemID)
                return entity;
        }

        return entt::null;
    }
}