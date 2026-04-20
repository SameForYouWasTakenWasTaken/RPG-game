#pragma once

#include "vendor/entt/entt.hpp"
#include "Components/HierarchyComponent.hpp"
namespace Game::Entities
{
    inline entt::entity CreateEntity(entt::registry& registry)
    {
        auto entity = registry.create();
        registry.emplace<Components::Hierarchy>(entity);

        return entity;
    }
}