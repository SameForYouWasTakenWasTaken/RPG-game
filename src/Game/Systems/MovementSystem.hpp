#pragma once

#include "Engine/Components/Transform.hpp"
#include "glm/glm.hpp"
#include "Engine/Systems/ISystem.hpp"
#include "vendor/entt/entt.hpp"

namespace Game::Systems
{
    class Movement : public Core::Systems::ISystem
    {
        entt::registry& m_SceneRegistry;

        public:
        Movement(entt::registry& registry) : m_SceneRegistry(registry) {}
        
        
        static void LinearGoTo(Core::Components::Transform& Transform, const glm::vec2& targetPos, float speed);
    };
}