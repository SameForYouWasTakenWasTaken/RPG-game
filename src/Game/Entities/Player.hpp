#pragma once

#include "Components/HierarchyComponent.hpp"
#include "Engine/Components/Geometry.hpp"
#include "Engine/Components/Transform.hpp"
#include "Systems/ResourceManager.hpp"
#include "Weapon.hpp"
#include "vendor/entt/entt.hpp"
#include "Components/Sprite.hpp"
#include "Components/Humanoid.hpp"
#include "Default.hpp"

namespace Game::Entities
{
    struct PlayerTag
    {};
    static inline auto PlayerTextureHandle = Systems::ResourceManager::LoadTexture(RESOURCES_DIRECTORY "Textures/player.png");
    
    inline entt::entity CreatePlayerEntity(entt::registry& registry)
    {

        auto entity = CreateEntity(registry);

        auto& Transform = registry.emplace<Core::Components::Transform>(entity);
        auto& Humanoid = registry.emplace<Components::Humanoid>(entity);
        auto& Geometry = registry.emplace<Core::Components::Geometry>(entity);

        auto& texture = Systems::ResourceManager::GetTexture(PlayerTextureHandle);
        
        auto& weapon = registry.emplace<Game::Weapons::Weapon>(entity);
        
        Geometry = Core::Components::CreateDefaultGeometry(texture, sf::Color::White);
        registry.emplace<Game::Components::Sprite>(entity, PlayerTextureHandle); 
        registry.emplace<PlayerTag>(entity);

        Transform.Scale({150.f, 150.f});
        Humanoid.Speed = 500.f;

        return entity;
    }
}