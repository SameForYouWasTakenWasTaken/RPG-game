#include <algorithm>
#include "InventorySystem.hpp"

#include "Components/InventoryItem.hpp"
#include "Components/Inventory.hpp"
#include "Components/Sprite.hpp"
#include "Systems/PickupSystem.hpp"

namespace Game::Systems
{
    /**
     * @brief Adds an item entity to an owner's inventory, merging with an existing stack when possible.
     *
     * If an item with the same ID already exists in the owner's inventory, the existing stack's count
     * is increased and the incoming entity is destroyed. If no matching item exists, the incoming
     * entity is appended to the inventory if there is capacity.
     *
     * @param r Registry containing the owner, item, and inventory components.
     * @param owner Entity that owns the inventory (must not be `entt::null`).
     * @param item  Entity representing the item to add (must not be `entt::null` and must have an `Item` component).
     * @return `true` if the item was merged or successfully added; `false` if the item could not be added because the inventory is at capacity.
     */
    bool Inventory::AddItem(entt::registry& r, entt::entity owner, entt::entity item)
    {
        assert(owner != entt::null);
        assert(item != entt::null);
        assert(IsItem(r, item));

        auto& inventory = r.get<Game::Components::Inventory>(owner);
        
        auto& incoming = r.get<Game::Components::Item>(item);
        incoming.owner = owner;

        auto existing = FindItemByType(r, owner, incoming.id);

        if (existing != entt::null)
        {
            auto& existingItem = r.get<Game::Components::Item>(existing);
            existingItem.Count += incoming.Count;
            r.destroy(item); // Get rid of the duplicate
        } else
        {
            if (inventory.Items.size() >= inventory.Capacity)
                return false;
            
            inventory.Items.push_back(item);
        }

        return true;
    }

    /**
     * @brief Removes a quantity of an item from an owner's inventory and destroys the provided item entity.
     *
     * Subtracts the provided item's Count from the matching item in the owner's inventory (matched by item id). If no matching item exists, the function returns without modifying the inventory. After applying the subtraction, the provided item entity is destroyed.
     *
     * @param r Registry containing entities and components.
     * @param owner Entity that owns the inventory; must not be entt::null.
     * @param item Entity representing the item and quantity to remove; must not be entt::null and must have Game::Components::Item.
     */
    void Inventory::RemoveItem(entt::registry& r, entt::entity owner, entt::entity item)
    {
        assert(owner != entt::null);
        assert(item != entt::null);
        assert(IsItem(r, item));

        auto& inventory = r.get<Game::Components::Inventory>(owner);
        auto& incoming = r.get<Game::Components::Item>(item);

        auto existing = FindItemByType(r, owner, incoming.id);
        if (existing == entt::null) return;
        
        auto& existingItem = r.get<Game::Components::Item>(existing);
        existingItem.Count -= incoming.Count;

        if (existingItem.Count <= 0)
        {
            inventory.Items.erase(
                std::remove(inventory.Items.begin(), inventory.Items.end(), existing),
                inventory.Items.end());
            r.destroy(existing);
        }

        r.destroy(item);
    }

    /**
     * @brief Checks whether an entity has an Item component.
     *
     * @return `true` if the entity has a Game::Components::Item component, `false` otherwise.
     */
    bool Inventory::IsItem(entt::registry& r, entt::entity item)
    {
        assert(item != entt::null);
        return r.all_of<Game::Components::Item>(item);
    }

    /**
     * @brief Locates an item entity in an owner's inventory by item ID.
     *
     * Searches the Inventory component of the specified owner for an entity whose
     * Game::Components::Item::id equals the provided itemID.
     *
     * @param r Registry containing the owner and item entities.
     * @param owner Owner entity whose Inventory will be searched. Must not be entt::null and must have a Inventory component.
     * @param itemID Identifier of the item type to find.
     * @return entt::entity The matching entity if found, or entt::null if no match exists.
     */
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

    /**
     * @brief Adds an item entity to the specified owner's inventory using the instance's scene registry.
     *
     * @param owner Entity owning the inventory.
     * @param item Entity representing the item to add.
     * @return bool `true` if the item was added (or merged into an existing stack), `false` if adding a new item failed due to capacity limits.
     */
    bool Inventory::AddItem(entt::entity owner, entt::entity item)
    {
        return AddItem(m_SceneRegistry, owner, item);
    }
    
    /**
     * @brief Removes an item entity from the specified owner's inventory using the instance's scene registry.
     *
     * @param inventoryOwner The owner entity whose inventory will be modified.
     * @param item The item entity to remove.
     */
    void Inventory::RemoveItem(entt::entity inventoryOwner, entt::entity item)
    {
        RemoveItem(m_SceneRegistry, inventoryOwner, item);
    }

    /**
     * @brief Checks whether an entity represents an Item in the scene registry.
     *
     * @param item The entity to check.
     * @return `true` if the entity has a `Game::Components::Item` component, `false` otherwise.
     */
    bool Inventory::IsItem(entt::entity item)
    {
        return IsItem(m_SceneRegistry, item);
    }

    bool Inventory::IsOwnedBy(entt::registry& r, entt::entity owner, entt::entity item)
    {
        if (item == entt::null) return false;

        if (!IsItem(r, item)) return false;
        auto& itemComponent = r.get<Components::Item>(item);

        return itemComponent.owner == owner;
    }



    bool Inventory::IsOwnedBy(entt::entity owner, entt::entity item)
    {
        return IsOwnedBy(m_SceneRegistry, owner, item);
    }
}