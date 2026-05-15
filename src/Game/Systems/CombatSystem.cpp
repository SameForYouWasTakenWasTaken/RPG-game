#include <iostream>
#include "CombatSystem.hpp"
#include "Components/Humanoid.hpp"
#include "Components/Progression.hpp"
#include "Engine/Components/Transform.hpp"
#include "Components/Weapon.hpp"
#include "Events/AttackedEvent.hpp"
#include "Events/Died.hpp"
#include "Components/States/Death.hpp"
namespace Game::Systems
{
    /**
     * @brief Processes all alive humanoid entities to handle death and optional health regeneration.
     *
     * Iterates over humanoids (entities with Game::Components::Humanoid and without Game::Components::Dead).
     * If a humanoid's Health is less than or equal to zero, emits a Game::Events::Died event for that entity
     * and adds Game::Components::Dead to the entity. If CanRegenerateHP is enabled, advances the humanoid's
     * regeneration timer by dt; when timer_HealthRegen is at or above RegenerateSpeed, reduces the timer by
     * RegenerateSpeed and increases Health by RegenerateHP, clamped to MaxHealth.
     *
     * @param dt Time step (seconds) used to advance regeneration timers.
     */
    void Combat::HandleHumanoids(float dt)
    {
        auto AliveHumanoids = m_SceneRegistry.view<Game::Components::Humanoid>(entt::exclude<Game::Components::Dead>);

        for (auto entity : AliveHumanoids)
        {
            auto& humanoid = m_SceneRegistry.get<Game::Components::Humanoid>(entity);
            const bool isAlive = humanoid.Health > 0;

            if (!isAlive)
            {
                m_EventBus.Emit<Game::Events::Died>(entity); 
                m_SceneRegistry.emplace<Game::Components::Dead>(entity); // Assign it dead
            }
            
            if (humanoid.CanRegenerateHP)
            {
                if (humanoid.timer_HealthRegen >= humanoid.RegenerateSpeed)
                {
                    humanoid.timer_HealthRegen -= humanoid.RegenerateSpeed;
                    
                    humanoid.Health = std::min(
                        humanoid.Health + humanoid.RegenerateHP,
                        humanoid.MaxHealth
                    );
                }
                humanoid.timer_HealthRegen += dt;
            }
        }
    }

    /**
     * @brief Updates cooldown timers and hit-registration state for all weapons.
     *
     * Advances each weapon's `cooldownTimer` by `dt` when a hit has been registered,
     * resets the timer to 0 once it reaches or exceeds `AttackCooldown`, and clears
     * `hitRegistered` when the cooldown completes so the weapon can register a new hit.
     *
     * @param dt Time elapsed since the last update, in seconds.
     */
    void Combat::HandleWeapons(float dt)
    {
        auto weapons = m_SceneRegistry.view<Components::Weapon>();

        // Handle weapon timers
        for (auto entity : weapons)
        {
            auto& weapon = m_SceneRegistry.get<Components::Weapon>(entity);

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
    /**
     * @brief Updates combat systems for the current frame.
     *
     * Advances weapon cooldowns and processes humanoid state (health regeneration and death)
     * using the provided time step.
     *
     * @param dt Time elapsed since the last update, in seconds.
     */
    void Combat::OnUpdate(float dt)
    {
        HandleWeapons(dt);
        HandleHumanoids(dt);
    }

    /**
     * @brief Checks whether a weapon is currently inside its hit window.
     *
     * @param weapon Weapon whose cooldown and hit window are evaluated.
     * @return `true` if `weapon.cooldownTimer` is greater than or equal to `weapon.HitWindowBegin`
     * and less than or equal to `weapon.HitWindowEnd`, `false` otherwise.
     */
    bool Combat::InHitWindow(const Components::Weapon& weapon)
    {
        return weapon.cooldownTimer >= weapon.HitWindowBegin && weapon.cooldownTimer <= weapon.HitWindowEnd;
    }

    bool Combat::InAttackRange(entt::registry& r, entt::entity enemy, entt::entity other)
    {
        auto& weapon = r.get<Components::Weapon>(enemy);
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
        auto& weapon = r.get<Components::Weapon>(attacker);
        auto& attackerHumanoid = r.get<Components::Humanoid>(attacker);
        auto& attackerCombatStats = r.get<Components::CombatStats>(attacker);
        auto& attackeeCombatStats = r.get<Components::CombatStats>(attackee);

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
            float baseDamage = attackerCombatStats.Strength + weapon.Damage;
            float defense = attackeeCombatStats.Dexterity;
            float finalDamage = baseDamage * (baseDamage / (baseDamage + defense));
             
            eventBus.Queue<Events::Attacked>(attacker, attackee, finalDamage);

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
