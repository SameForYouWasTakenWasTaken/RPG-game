#pragma once

#include "glm/glm.hpp"
#include "Components/States/Combat.hpp"
#include "Engine/Events/EventBus.hpp"
#include "Engine/Systems/ISystem.hpp"
#include "vendor/entt/entt.hpp"

namespace Game::Systems
{
    class AISystem : public Core::Systems::ISystem
    {
        entt::registry& m_SceneRegistry;
        Core::Events::EventBus& m_SceneEventBus;

        inline auto GetPlayers();

        void ChaseEntity(entt::entity enemy, entt::entity other, float dt);
        entt::entity GetNearestPlayer(const glm::vec2& nearestEnemyPos);
    public:
        AISystem(entt::registry& registry, Core::Events::EventBus& bus) 
        : m_SceneRegistry(registry), m_SceneEventBus(bus) {}

        bool CanEnemyDetect(entt::entity enemy, entt::entity other);
        bool CanEnemyHear(entt::entity enemy, entt::entity other);
        bool CanEnemySee(entt::entity enemy, entt::entity other);
        
        void OnUpdate(float dt) override;
    }; 
}