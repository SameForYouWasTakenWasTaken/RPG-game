#include "EnemyAI.hpp"
#include "Components/Enemy.hpp"
#include "Components/Humanoid.hpp"
#include "Components/LonelyTags.hpp"
#include "Components/States/Death.hpp"
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
                Game::Components::Humanoid>(entt::exclude<Game::Components::Dead>);
    }

    /**
     * Finds the closest player entity to a given enemy world position.
     *
     * The selection uses the player's origin (player world position plus the transform's local origin)
     * and compares Euclidean distances to determine the nearest player.
     *
     * @param nearestEnemyPos Enemy position in world coordinates used as the reference point.
     * @return entt::entity The nearest player entity, or `entt::null` if no players are present.
     */
    entt::entity AISystem::GetNearestPlayer(const glm::vec2& nearestEnemyPos, float enemyDetectionRadius)
    {
        entt::entity targetPlayer = entt::null;
        float bestDist = std::numeric_limits<float>::max();

        auto nearestPlayers = m_SpatialGrid.QueryRadius(
            nearestEnemyPos,
            enemyDetectionRadius,
            [this](auto entity){
            return m_SceneRegistry.all_of<Entities::PlayerTag>(entity);
        });

        for (auto player : nearestPlayers)
        {
            auto& PlayerTransform = m_SceneRegistry.get<Core::Components::Transform>(player);
            auto playerOriginPosition = PlayerTransform.GetWorldPos() + PlayerTransform.GetLocalOrigin();

            glm::vec2 delta = playerOriginPosition - nearestEnemyPos;
            float dist = glm::dot(delta, delta); // Better  than glm::distance
             
            if (dist < bestDist)
            {
                bestDist = dist;
                targetPlayer = player;
            }
        }

        return targetPlayer;
    }
    void AISystem::OnUpdate(float dt)
    {
        auto enemies = m_SceneRegistry.view<
            Entities::EnemyTag,
            Game::Components::LonelyTags::Spatial,
            Core::Components::Transform,
            Game::Components::Enemy,
            Game::Components::Humanoid>();

        for (auto enemy : enemies)
        {
            auto& enemyTransform = m_SceneRegistry.get<Core::Components::Transform>(enemy);
            auto& enemyComp = m_SceneRegistry.get<Game::Components::Enemy>(enemy);

            // find closest player
            entt::entity targetPlayer = GetNearestPlayer(enemyTransform.GetWorldPos(), enemyComp.PlayerDetectionDist);
            if (targetPlayer == entt::null)
                continue;

            auto playerTransform = m_SceneRegistry.get<Core::Components::Transform>(targetPlayer);
            auto delta = playerTransform.GetWorldPos() - enemyTransform.GetWorldPos();
            auto dist = glm::dot(delta, delta);

            const float maxDistSq =
                enemyComp.PlayerMaxDistance *
                enemyComp.PlayerMaxDistance;
                    
            if (dist >= maxDistSq)
                ChaseEntity(enemy, targetPlayer, dt);
 
            if (Combat::InAttackRange(m_SceneRegistry, enemy, targetPlayer))
                Combat::AttackEntity(m_SceneRegistry, m_SceneEventBus, enemy, targetPlayer);
        }
    }


    bool AISystem::CanEnemyHear(entt::registry& r, entt::entity enemy, entt::entity other)
    {
        auto& EnemyTransform = r.get<Core::Components::Transform>(enemy);
        auto& OtherTransform = r.get<Core::Components::Transform>(other);

        auto& EnemyComponent = r.get<Game::Components::Enemy>(enemy);

        auto distance = glm::distance(OtherTransform.GetWorldPos(), EnemyTransform.GetWorldPos());

        if (distance <= EnemyComponent.PlayerHearDetectionDist)
            return true;

        return false;
    }

    bool AISystem::CanEnemySee(entt::registry& r, entt::entity enemy, entt::entity other)
    {
        auto& EnemyTransform = r.get<Core::Components::Transform>(enemy);
        auto& OtherTransform = r.get<Core::Components::Transform>(other);
        auto& EnemyComponent = r.get<Game::Components::Enemy>(enemy);
        
        auto distance = glm::distance(OtherTransform.GetWorldPos(), EnemyTransform.GetWorldPos());

        if (distance <= EnemyComponent.PlayerDetectionDist)
            return true;

        // TODO: Dot product

        return false;
    }

    bool AISystem::CanEnemyDetect(entt::registry& r, entt::entity enemy, entt::entity other)
    {
        auto& EnemyTransform = r.get<Core::Components::Transform>(enemy);
        auto& OtherTransform = r.get<Core::Components::Transform>(other);
        auto& EnemyComponent = r.get<Game::Components::Enemy>(enemy);

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

        Movement::LinearGoTo(enemyTransform, playerTransform.GetWorldPos(), enemyHumanoid.RunSpeed * dt);
    }

    bool AISystem::CanEnemyDetect(entt::entity enemy, entt::entity other)
    {
        return CanEnemyDetect(m_SceneRegistry, enemy, other);
    }
    
    bool AISystem::CanEnemyHear(entt::entity enemy, entt::entity other)
    {
        return CanEnemyHear(m_SceneRegistry, enemy, other);
    }

    bool AISystem::CanEnemySee(entt::entity enemy, entt::entity other)
    {
        return CanEnemySee(m_SceneRegistry, enemy, other);
    }
}