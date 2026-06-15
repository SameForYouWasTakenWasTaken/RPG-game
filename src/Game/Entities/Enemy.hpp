#pragma once

#include "Components/Progression.hpp"
#include "Engine/Components/Geometry.hpp"
#include "Components/Weapon.hpp"
#include "vendor/entt/entt.hpp"
#include "Engine/Components/Transform.hpp"
#include "Systems/ResourceManager.hpp"
#include "Components/Humanoid.hpp"
#include "Components/Sprite.hpp"
#include "Components/Enemy.hpp"
#include "Default.hpp"
#include "Components/LonelyTags.hpp"

namespace Game::Entities
{
    struct EnemyTag
    {};
    
    static inline auto EnemyTextureHandle = Systems::ResourceManager::LoadTexture(RESOURCES_DIRECTORY "Shared/Textures/enemy.jpg");
    
    /**
     * @brief Creates and configures a new enemy entity in the provided registry.
     *
     * The created entity receives Transform, Humanoid, Geometry, Enemy, Weapon,
     * Sprite, CombatStats, Weapon, and EnemyTag components. Humanoid attributes (Speed, RunSpeed,
     * Health) and combat stats (Strength) are initialized; geometry is created from the enemy
     * texture, and the transform's scale and origin are set so the origin is at
     * the geometry's center.
     *
     * @param registry The entt registry to which the enemy entity and its components are added.
     * @return entt::entity Handle to the newly created enemy entity.
     */
    inline entt::entity CreateEnemyEntity(entt::registry& registry)
    {
        auto entity = CreateEntity(registry);
        auto& texture = Systems::ResourceManager::GetTexture(EnemyTextureHandle);
        auto& Transform = registry.emplace<Core::Components::Transform>(entity);
        auto& Humanoid = registry.emplace<Components::Humanoid>(entity);
        auto& Geometry = registry.emplace<Core::Components::Geometry>(entity);
        auto& Enemy = registry.emplace<Game::Components::Enemy>(entity);

        auto& combatStats = registry.emplace<Game::Components::CombatStats>(entity);
        auto& weapon = registry.emplace<Game::Components::Weapon>(entity);

        
        Humanoid.Speed = 300.f;
        Humanoid.RunSpeed = Humanoid.Speed * 1.2;
        combatStats.Strength = 5.f;
        Humanoid.Health = 30.f;
        
        Geometry = Core::Components::CreateDefaultGeometry(texture, sf::Color::White);

        auto& sprite = registry.emplace<Game::Components::Sprite>(entity, EnemyTextureHandle); 
        sprite.zIndex = 10;
        
        registry.emplace<EnemyTag>(entity);

        Transform.Scale({150.f, 150.f});
        Transform.SetOrigin(Transform.GetLocalSize() / 2.f);

        registry.emplace<Game::Components::LonelyTags::Spatial>(entity);
        return entity;
    }
}