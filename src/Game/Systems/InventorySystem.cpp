#include <iostream>
#include "InventorySystem.hpp"

#include "Components/InventoryItem.hpp"
#include "Components/Inventory.hpp"
#include "Components/Sprite.hpp"

namespace Game::Systems
{
    bool Inventory::AddItem(entt::registry& r, entt::entity owner, entt::entity item)
    {
        assert(owner != entt::null);
        assert(item != entt::null);
        assert(IsItem(r, item));

        auto& inventory = r.get<Game::Components::Inventory>(owner);
        auto& incoming = r.get<Game::Components::Item>(item);
        auto existing = FindItemByType(r, owner, incoming.id);

        if (existing != entt::null)
        {
            auto& existingItem = r.get<Game::Components::Item>(existing);
            existingItem.Count += incoming.Count;
            r.destroy(item); // Get rid of the duplicate

            std::cout << "Count: " << existingItem.Count << std::endl;
        } else
        {
            if (inventory.Items.size() >= inventory.Capacity)
                return false;
            
            inventory.Items.push_back(item);
        }

        std::cout << "\nPut in a new item!\n" << "Name: "
        << incoming.Name << "\n Inv Size: " << inventory.Items.size() << std::endl;
        return true;
    }

    void Inventory::RemoveItem(entt::registry& r, entt::entity owner, entt::entity item)
    {
        assert(owner != entt::null);
        assert(item != entt::null);
        assert(IsItem(r, item));

        auto& incoming = r.get<Game::Components::Item>(item);

        auto existing = FindItemByType(r, owner, incoming.id);
        if (existing == entt::null) return;
        
        auto& existingItem = r.get<Game::Components::Item>(existing);
        existingItem.Count -= incoming.Count;

        r.destroy(item);
        return;
    }

    bool Inventory::IsItem(entt::registry& r, entt::entity item)
    {
        assert(item != entt::null);
        return r.all_of<Game::Components::Item>(item);
    }

    entt::entity Inventory::FindItemByType(entt::registry& r, entt::entity owner, int itemID)
    {
        assert(owner != entt::null);

        auto& inventory = r.get<Game::Components::Inventory>(owner);

        for (auto entity : inventory.Items)
        {
            auto& item = r.get<Game::Components::Item>(entity);
            if (item.id == itemID)
                return entity;
        }

        return entt::null;
    }

    inline bool Inventory::AddItem(entt::entity owner, entt::entity item)
    {
        return AddItem(m_SceneRegistry, owner, item);
    }
    
    inline void Inventory::RemoveItem(entt::entity inventoryOwner, entt::entity item)
    {
        RemoveItem(m_SceneRegistry, inventoryOwner, item);
    }

    inline bool Inventory::IsItem(entt::entity item)
    {
        return IsItem(m_SceneRegistry, item);
    }
}