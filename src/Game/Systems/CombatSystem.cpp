#include <iostream>
#include "CombatSystem.hpp"
#include "Components/Humanoid.hpp"
#include "Engine/Components/Transform.hpp"
#include "Entities/Weapon.hpp"
#include "Events/AttackedEvent.hpp"
#include "Events/Died.hpp"

namespace Game::Systems
{
    void Combat::HandleHumanoids()
    {
        auto humanoids = m_SceneRegistry.view<Game::Components::Humanoid>();
        for (auto entity : humanoids)
        {
            auto& humanoid = m_SceneRegistry.get<Game::Components::Humanoid>(entity);
            if (humanoid.Health <= 0)
                m_EventBus.Emit<Game::Events::Died>(entity);   
        }
    }

    void Combat::HandleWeapons(float dt)
    {
        auto weapons = m_SceneRegistry.view<Weapons::Weapon>();

        // Handle weapon timers
        for (auto entity : weapons)
        {
            auto& weapon = m_SceneRegistry.get<Weapons::Weapon>(entity);

            // Only update the timer if there was any contact to begin with
            if (weapon.cooldownTimer >= weapon.AttackCooldown)
            {
                weapon.cooldownTimer = 0;

                // If the player was already hit, then turn it off (so first contact can reactivate itself if needed)
                if (weapon.hitRegistered)
                    weapon.hitRegistered = false;
            }

            if (weapon.hitRegistered)
                weapon.cooldownTimer += dt;
        }
    }
    void Combat::OnUpdate(float dt)
    {

        HandleWeapons(dt);
        HandleHumanoids();
    }

    bool Combat::InHitWindow(const Weapons::Weapon& weapon)
    {
        return weapon.cooldownTimer >= weapon.HitWindowBegin && weapon.cooldownTimer <= weapon.HitWindowEnd;
    }

    bool Combat::InAttackRange(entt::registry& r, entt::entity enemy, entt::entity other)
    {
        auto& weapon = r.get<Game::Weapons::Weapon>(enemy);
        auto& EnemyTransform = r.get<Core::Components::Transform>(enemy);
        auto& OtherTransform = r.get<Core::Components::Transform>(other);

        auto distance = glm::distance(OtherTransform.GetWorldPos(), EnemyTransform.GetWorldPos());

        if (distance <= weapon.AttackRange)
            return true;

        return false;
    }

    inline void Combat::AttackEntity(entt::entity attacker, entt::entity attackee)
    {
        Combat::AttackEntity(m_SceneRegistry, m_EventBus, attacker, attackee);
    }

    inline bool Combat::InAttackRange(entt::entity attacker, entt::entity attackee)
    {
        return Combat::InAttackRange(m_SceneRegistry, attacker, attackee);
    }

    void Combat::AttackEntity(entt::registry& r, Core::Events::EventBus& eventBus, entt::entity attacker, entt::entity attackee)
    {
        auto& weapon = r.get<Weapons::Weapon>(attacker);
        auto& attackerHumanoid = r.get<Components::Humanoid>(attacker);

        // Make sure the hit windows are not poorly configured or longer than the actual cooldown
        assert(weapon.HitWindowBegin < weapon.HitWindowEnd);
        assert(weapon.HitWindowEnd <= weapon.AttackCooldown);

        bool firstContact = false;

        // If it is the first contact with the player, then instantly attack
        if (weapon.cooldownTimer == 0)
            firstContact = true;

        // trigger hit ONCE inside window
        if (!weapon.hitRegistered && InHitWindow(weapon) || firstContact)
        {
            float dmg = attackerHumanoid.BaseDamage + weapon.Damage;
            eventBus.Queue<Events::Attacked>(attacker, attackee, dmg);
            weapon.hitRegistered = true;
        }

        // finish attack after window
        if (weapon.cooldownTimer > weapon.HitWindowEnd)
        {
            if (!weapon.hitRegistered)
            {
                // attack missed
            }
        }
    }
}