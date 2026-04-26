#include "EnemyAI.hpp"
#include "Components/Enemy.hpp"
#include "Components/Humanoid.hpp"
#include "Engine/Components/Transform.hpp"
#include "Entities/Enemy.hpp"
#include "Entities/Player.hpp"
#include "Components/States/Movement.hpp"
#include "MovementSystem.hpp"
#include "glm/geometric.hpp"
#include "CombatSystem.hpp"
#include <limits>

namespace Game::Systems
{
    inline auto AISystem::GetPlayers()
    {
        return m_SceneRegistry.view<                
                Entities::PlayerTag, 
                Core::Components::Transform,
                Game::Components::Humanoid>();
    }

    entt::entity AISystem::GetNearestPlayer(const glm::vec2& nearestEnemyPos)
    {
        entt::entity targetPlayer = entt::null;
        float bestDist = std::numeric_limits<float>::max();
        
        for (auto player : GetPlayers())
        {
            auto& PlayerTransform = m_SceneRegistry.get<Core::Components::Transform>(player);
            float dist = glm::distance(PlayerTransform.GetWorldPos(), nearestEnemyPos);
            if (dist < bestDist)
            {
                bestDist = dist;
                targetPlayer = player;
            }
        }

        return targetPlayer;
    }
    void AISystem::OnFixed(float step)
    {
        auto enemies = m_SceneRegistry.view<
            Entities::EnemyTag,
            Core::Components::Transform,
            Game::Components::Enemy,
            Game::Components::Humanoid>();

        for (auto enemy : enemies)
        {
            auto& enemyTransform = m_SceneRegistry.get<Core::Components::Transform>(enemy);
            auto& enemyComp = m_SceneRegistry.get<Game::Components::Enemy>(enemy);
            bool isChasing = m_SceneRegistry.all_of<Game::Components::Chasing>(enemy);

            // find closest player
            entt::entity targetPlayer = GetNearestPlayer(enemyTransform.GetWorldPos());
            if (targetPlayer == entt::null)
                continue;

            bool detected = CanEnemyDetect(enemy, targetPlayer);
            if (!detected)
            {
                m_SceneRegistry.remove<Game::Components::Chasing>(enemy);
                continue;
            }

            auto playerTransform = m_SceneRegistry.get<Core::Components::Transform>(targetPlayer);
            auto dist = glm::distance(playerTransform.GetWorldPos(), enemyTransform.GetWorldPos());
            
            if (dist >= enemyComp.PlayerMaxDistance)
                ChaseEntity(enemy, targetPlayer, step);

            if (Combat::InAttackRange(m_SceneRegistry, enemy, targetPlayer))
                Combat::AttackEntity(m_SceneRegistry, m_SceneEventBus, enemy, targetPlayer);
        }
    }


    bool AISystem::CanEnemyHear(entt::entity enemy, entt::entity other)
    {
        auto& EnemyTransform = m_SceneRegistry.get<Core::Components::Transform>(enemy);
        auto& OtherTransform = m_SceneRegistry.get<Core::Components::Transform>(other);

        auto& EnemyComponent = m_SceneRegistry.get<Game::Components::Enemy>(enemy);

        auto distance = glm::distance(OtherTransform.GetWorldPos(), EnemyTransform.GetWorldPos());

        if (distance <= EnemyComponent.PlayerHearDetectionDist)
            return true;

        return false;
    }

    bool AISystem::CanEnemySee(entt::entity enemy, entt::entity other)
    {
        auto& EnemyTransform = m_SceneRegistry.get<Core::Components::Transform>(enemy);
        auto& OtherTransform = m_SceneRegistry.get<Core::Components::Transform>(other);
        auto& EnemyComponent = m_SceneRegistry.get<Game::Components::Enemy>(enemy);
        
        auto distance = glm::distance(OtherTransform.GetWorldPos(), EnemyTransform.GetWorldPos());

        if (distance <= EnemyComponent.PlayerDetectionDist)
            return true;

        // TODO: Dot product

        return false;
    }

    bool AISystem::CanEnemyDetect(entt::entity enemy, entt::entity other)
    {
        auto& EnemyTransform = m_SceneRegistry.get<Core::Components::Transform>(enemy);
        auto& OtherTransform = m_SceneRegistry.get<Core::Components::Transform>(other);
        auto& EnemyComponent = m_SceneRegistry.get<Game::Components::Enemy>(enemy);

        auto distance = glm::distance(OtherTransform.GetWorldPos(), EnemyTransform.GetWorldPos());
        
        if (distance <= EnemyComponent.PlayerDetectionDist)
            return true;
        return false;
    }

    void AISystem::ChaseEntity(entt::entity enemy, entt::entity other, float dt)
    {
        auto& enemyTransform = m_SceneRegistry.get<Core::Components::Transform>(enemy);
        auto& enemyHumanoid  = m_SceneRegistry.get<Game::Components::Humanoid>(enemy);
        auto& playerTransform = m_SceneRegistry.get<Core::Components::Transform>(other);
        auto& enemyComponent = m_SceneRegistry.get<Components::Enemy>(enemy);

        Movement::LinearGoTo(enemyTransform, playerTransform.GetWorldPos(), enemyHumanoid.Speed * dt);
        m_SceneRegistry.emplace_or_replace<Game::Components::Chasing>(enemy);
    }
}