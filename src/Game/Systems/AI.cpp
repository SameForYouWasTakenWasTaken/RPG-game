#include "AI.hpp"
#include "Components/Enemy.hpp"
#include "Components/Humanoid.hpp"
#include "Engine/Components/Transform.hpp"
#include "Entities/Enemy.hpp"
#include "Entities/Player.hpp"
#include "Components/States/Movement.hpp"
#include "Components/States/Combat.hpp"
#include "glm/geometric.hpp"
#include "Events/AttackedEvent.hpp"
#include <iostream>
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

    void AISystem::OnFixed(float step)
    {
        auto players = GetPlayers();
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

            if (isChasing)
                if (auto* attack = m_SceneRegistry.try_get<Game::Components::Attacking>(enemy))
                    {
                        attack->timer += step;
                        attack->elapsedCooldownCount -= step;
                    }
            // find closest player
            entt::entity targetPlayer = entt::null;
            float bestDist = std::numeric_limits<float>::max();
            
            for (auto player : players)
            {
                auto& PlayerTransform = m_SceneRegistry.get<Core::Components::Transform>(player);

                float dist = glm::distance(PlayerTransform.GetWorldPos(), enemyTransform.GetWorldPos());

                if (dist < bestDist)
                {
                    bestDist = dist;
                    targetPlayer = player;
                }
            }

            if (targetPlayer == entt::null)
                continue;

            bool detected = CanEnemyDetect(enemy, targetPlayer);
            if (!detected)
            {
                m_SceneRegistry.remove<Game::Components::Chasing>(enemy);
                continue;
            }

            ChaseEntity(enemy, targetPlayer, step);

            if (InAttackRange(enemy, targetPlayer))
                AttackEntity(enemy, targetPlayer);
            else
                // Force restarting the cooldown by removing the attacking component
                m_SceneRegistry.remove<Game::Components::Attacking>(enemy);
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

        glm::vec2 delta = playerTransform.GetWorldPos() - enemyTransform.GetWorldPos();
        float distance = glm::length(delta);

        if (distance < 0.0001f)
            return;

        
        auto& enemyCfg = m_SceneRegistry.get<Game::Components::Enemy>(enemy);
        if (distance <= enemyCfg.PlayerMaxDistance)
            return;

        glm::vec2 direction = delta / distance;

        enemyTransform.Move(direction * enemyHumanoid.Speed * dt);

        m_SceneRegistry.emplace_or_replace<Game::Components::Chasing>(enemy);
    }


    bool AISystem::InAttackRange(entt::entity enemy, entt::entity other)
    {
        auto& EnemyComponent = m_SceneRegistry.get<Game::Components::Enemy>(enemy);
        auto& EnemyTransform = m_SceneRegistry.get<Core::Components::Transform>(enemy);
        auto& OtherTransform = m_SceneRegistry.get<Core::Components::Transform>(other);

        auto distance = glm::distance(OtherTransform.GetWorldPos(), EnemyTransform.GetWorldPos());

        if (distance <= EnemyComponent.AttackRange)
            return true;

        return false;
    }

    bool AISystem::IsAttacking(entt::entity enemy)
    {
        return m_SceneRegistry.all_of<Game::Components::Attacking>(enemy);
    }

    bool AISystem::InHitWindow(Game::Components::Attacking attack, float begin, float end)
    {
        return attack.timer >= begin && attack.timer <= end;
    }

    void AISystem::AttackEntity(entt::entity enemy, entt::entity other)
    {
        auto& enemyCfg = m_SceneRegistry.get<Game::Components::Enemy>(enemy);

        // start attack if not already attacking
        if (!m_SceneRegistry.all_of<Game::Components::Attacking>(enemy))
        {
            auto& a = m_SceneRegistry.emplace<Game::Components::Attacking>(enemy);
            a.elapsedCooldownCount = enemyCfg.AttackCooldown;
        }

        auto& attack = m_SceneRegistry.get<Game::Components::Attacking>(enemy);
        auto& enemyHumanoid = m_SceneRegistry.get<Game::Components::Humanoid>(enemy);

        if (attack.elapsedCooldownCount > 0.f)
            return;

        // trigger hit ONCE inside window
        if (!attack.hitRegistered && attack.timer >= enemyCfg.AttackBegin)
        {
            attack.hitRegistered = true;

            float dmg = enemyHumanoid.BaseDamage;
            m_SceneEventBus.Queue<Events::Attacked>(enemy, other, dmg);
        }

        // finish attack after window
        if (attack.timer > enemyCfg.AttackEnd)
        {
            if (!attack.hitRegistered)
            {
                // m_SceneEventBus.Queue<Events::AttackMissed>(enemy, player);
            }

            m_SceneRegistry.remove<Game::Components::Attacking>(enemy);
        }
    }
}