#pragma once

#include "vendor/entt/entt.hpp"
namespace Game::Components
{
    struct Inventory
    {
        std::vector<entt::entity> Items{};
        size_t Capacity = 20;
    };
}