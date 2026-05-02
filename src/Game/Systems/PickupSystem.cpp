#include <iostream>
#include "PickupSystem.hpp"
#include "Components/Inventory.hpp"
#include "Components/InventoryItem.hpp"
#include "Components/Pickable.hpp"
#include "Engine/Components/Transform.hpp"
#include "Entities/Player.hpp"
#include "InventorySystem.hpp"
#include <limits>

namespace Game::Systems
{
    /**
     * @brief Determines whether an entity is eligible to be picked up.
     *
     * @param item The entity to evaluate.
     * @return true if the entity is a registered inventory item and has both a `Game::Components::Pickable` component and a `Core::Components::Transform` component, false otherwise.
     */
    bool Pickup::IsPickable(entt::registry& r, entt::entity item)
    {
        assert(item != entt::null);

        return Game::Systems::Inventory::IsItem(r, item) &&
            r.all_of<
                Game::Components::Pickable,
                Core::Components::Transform
            >(item);
    }
  
    /**
     * @brief Attempts to pick up an item and add it to a picker's inventory.
     *
     * Marks the item's `Pickable::CanBePicked` as false to prevent concurrent pickup attempts,
     * tries to add the item to the picker's inventory, and restores `CanBePicked` to true if adding fails.
     *
     * @param item The entity being picked up; must satisfy the precondition checked by `IsPickable`.
     * @param picker The entity attempting the pickup (typically a player).
     * @return `true` if the function completed its pickup attempt (current implementation always returns `true`).
     */
    bool Pickup::PickUp(entt::registry& r, entt::entity item, entt::entity picker)
    {
        if (item == entt::null || picker == entt::null)
            return false;
        
        if (!r.all_of<Game::Components::Pickable, Core::Components::Transform>(item) ||
            !Game::Systems::Inventory::IsItem(r, item) ||
            !r.all_of<Game::Components::Inventory>(picker))
            return false;
        
        auto& pickable = r.get<Components::Pickable>(item);
        pickable.CanBePicked = false;

        const bool added = Inventory::AddItem(r, picker, item);
        if (!added)
            pickable.CanBePicked = true;// Reset to true if it failed
        


        return added;
    }

    /**
     * @brief Scans the scene each frame and attempts to transfer nearby pickable items into players' inventories.
     *
     * Iterates all entities that have Pickable+Item+Transform and, for each item currently allowed to be picked,
     * finds the nearest player among entities with PlayerTag+Inventory+Transform. If the nearest player is within
     * the item's pickup radius, attempts to pick up the item and add it to that player's inventory.
     *
     * @param dt Time elapsed since the last update, in seconds.
     */
    void Pickup::OnUpdate(float dt)
    {
        auto Players = m_SceneRegistry.view<
            Game::Entities::PlayerTag,
            Game::Components::Inventory,
            Core::Components::Transform>();
        
        auto Pickables = m_SceneRegistry.view<
            Components::Pickable,
            Components::Item,
            Core::Components::Transform
        >();
        
        for (auto itemEntity : Pickables)
        {
            auto& itemTransform = m_SceneRegistry.get<Core::Components::Transform>(itemEntity);
            auto& itemPickable = m_SceneRegistry.get<Game::Components::Pickable>(itemEntity);
            auto& itemComponent = m_SceneRegistry.get<Game::Components::Item>(itemEntity);
            
            if (!itemPickable.CanBePicked) continue;

            entt::entity nearestPlayer{entt::null};
            float bestDist = std::numeric_limits<float>::max();

            for (auto playerEntity : Players)
            {
                auto& playerTransform = m_SceneRegistry.get<Core::Components::Transform>(playerEntity);
                auto playerOriginPosition = playerTransform.GetWorldPos() + playerTransform.GetLocalOrigin();
                auto itemOriginPosition = itemTransform.GetWorldPos() + itemTransform.GetLocalOrigin();

                auto distToItem = glm::distance(itemOriginPosition, playerOriginPosition);
                
                if (distToItem <= bestDist)
                {
                    bestDist = distToItem;
                    nearestPlayer = playerEntity;
                }
            }

            if (nearestPlayer == entt::null) continue;
            if (bestDist > itemPickable.Radius) continue;

            if (!PickUp(itemEntity, nearestPlayer))
                std::cout << "Couldn't pick up item!" << std::endl;
        }
    }

    bool Pickup::PickUp(entt::entity item, entt::entity owner)
    {
        return PickUp(m_SceneRegistry, item, owner);
    }

    bool Pickup::IsPickable(entt::entity item)
    {
        return IsPickable(m_SceneRegistry, item);
    }
}