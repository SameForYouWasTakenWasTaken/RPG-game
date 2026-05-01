#pragma once

#include "Components/HierarchyComponent.hpp"
#include "Components/Inventory.hpp"
#include "Engine/Components/Geometry.hpp"
#include "Engine/Components/Transform.hpp"
#include "Systems/ResourceManager.hpp"
#include "Components/Weapon.hpp"
#include "vendor/entt/entt.hpp"
#include "Components/Sprite.hpp"
#include "Components/Humanoid.hpp"
#include "Default.hpp"

namespace Game::Entities
{
    struct PlayerTag
    {};
    static inline auto PlayerTextureHandle = Systems::ResourceManager::LoadTexture(RESOURCES_DIRECTORY "Textures/player.png");
    
    /**
     * @brief Creates and registers a player entity with default player components and initial configuration.
     *
     * The created entity is populated with transform, humanoid, geometry, weapon, inventory, sprite, and a PlayerTag.
     * Geometry and sprite are initialized from the shared player texture handle; the transform is scaled and centered,
     * and the humanoid movement speed is set to a default value.
     *
     * @param registry The ECS registry to which the player entity and its components will be added.
     * @return entt::entity Handle to the newly created player entity.
     */
    inline entt::entity CreatePlayerEntity(entt::registry& registry)
    {

        auto entity = CreateEntity(registry);

        auto& Transform = registry.emplace<Core::Components::Transform>(entity);
        auto& Humanoid = registry.emplace<Components::Humanoid>(entity);
        auto& Geometry = registry.emplace<Core::Components::Geometry>(entity);

        auto& texture = Systems::ResourceManager::GetTexture(PlayerTextureHandle);
        
        auto& weapon = registry.emplace<Game::Components::Weapon>(entity);
        auto& inventory = registry.emplace<Game::Components::Inventory>(entity);
        
        Geometry = Core::Components::CreateDefaultGeometry(texture, sf::Color::White);
        registry.emplace<Game::Components::Sprite>(entity, PlayerTextureHandle); 
        registry.emplace<PlayerTag>(entity);

        Transform.Scale({150.f, 150.f});
        Transform.SetOrigin(Transform.GetLocalSize() / 2.f);

        Humanoid.Speed = 500.f;

        return entity;
    }
}