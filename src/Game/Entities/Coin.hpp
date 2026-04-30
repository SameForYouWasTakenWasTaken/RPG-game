#pragma once

#include "Components/InventoryItem.hpp"
#include "Components/Pickable.hpp"
#include "Components/Sprite.hpp"
#include "Engine/Components/Geometry.hpp"
#include "Engine/Components/Transform.hpp"
#include "Systems/ResourceManager.hpp"
#include "vendor/entt/entt.hpp"
#include "Default.hpp"

namespace Game::Entities
{
    static inline auto CoinTextureHandle = Game::Systems::ResourceManager::LoadTexture(RESOURCES_DIRECTORY "Textures/coin.png");
    inline entt::entity CreateCoin(entt::registry& registry)
    {
        auto& texture = Game::Systems::ResourceManager::GetTexture(CoinTextureHandle);
        
        auto entity = CreateEntity(registry);
        
        auto& geometry = registry.emplace<Core::Components::Geometry>(entity);
        geometry = Core::Components::CreateDefaultGeometry(texture);

        auto& transform = registry.emplace<Core::Components::Transform>(entity);
        transform.SetSize({50.f, 50.f});
        transform.SetOrigin(transform.GetLocalSize() / 2.f);

        auto& sprite = registry.emplace<Game::Components::Sprite>(entity);
        sprite.TextureHandle = CoinTextureHandle;
        
        auto& item = registry.emplace<Game::Components::Item>(entity);
        item.Name = "Coin";
        item.StackCount = 100.f;
        
        auto& pickable = registry.emplace<Game::Components::Pickable>(entity);
        return entity;
    }
}