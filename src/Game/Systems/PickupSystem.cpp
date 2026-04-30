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
    bool Pickup::IsPickable(entt::entity item)
    {
        assert(item != entt::null);

        return Game::Systems::Inventory::IsItem(m_SceneRegistry, item) &&
            m_SceneRegistry.all_of<
                Game::Components::Pickable,
                Core::Components::Transform
            >(item);
    }

    bool Pickup::PickUp(entt::entity item, entt::entity picker)
    {
        assert(IsPickable(item));
        
        auto& pickable = m_SceneRegistry.get<Components::Pickable>(item);
        pickable.CanBePicked = false;

        const bool added = Inventory::AddItem(m_SceneRegistry, picker, item);
        if (!added)
            pickable.CanBePicked = true;// Reset to true if it failed
        


        return added;
    }

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
}