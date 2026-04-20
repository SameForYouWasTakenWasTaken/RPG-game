#pragma once

#include "Engine/Components/Geometry.hpp"
#include "vendor/entt/entt.hpp"
#include "Engine/Components/Transform.hpp"
#include "Systems/ResourceManager.hpp"
#include "Components/Humanoid.hpp"
#include "Components/Sprite.hpp"
#include "Components/Enemy.hpp"
#include "Default.hpp"

namespace Game::Entities
{
    struct EnemyTag
    {};
    
    static inline auto EnemyTextureHandle = Systems::ResourceManager::LoadTexture(RESOURCES_DIRECTORY "Textures/enemy.jpg");
    
    inline entt::entity CreateEnemyEntity(entt::registry& registry)
    {
        auto entity = CreateEntity(registry);
        auto& texture = Systems::ResourceManager::GetTexture(EnemyTextureHandle);

        auto& Transform = registry.emplace<Core::Components::Transform>(entity);
        auto& Humanoid = registry.emplace<Components::Humanoid>(entity);
        auto& Geometry = registry.emplace<Core::Components::Geometry>(entity);
        auto& Enemy = registry.emplace<Game::Components::Enemy>(entity);
        
        Geometry = Core::Components::CreateDefaultGeometry(texture, sf::Color::White);

        registry.emplace<Game::Components::Sprite>(entity, EnemyTextureHandle); 
        registry.emplace<EnemyTag>(entity);

        Transform.Scale({150.f, 150.f});

        return entity;
    }
}