#pragma once

#include "vendor/entt/entt.hpp"

namespace Game::Components
{
    struct Hierarchy
    {
        entt::entity Parent{entt::null};
        entt::entity FirstChild{entt::null};
        entt::entity NextSibling{entt::null};
    };
}