#pragma once

#include "Engine/Events/EventBus.hpp"
#include "Engine/Systems/ISystem.hpp"
#include "Entities/Weapon.hpp"
#include "vendor/entt/entt.hpp"

namespace Game::Systems
{
    class Combat : public Core::Systems::ISystem
    {
        entt::registry& m_SceneRegistry;
        Core::Events::EventBus& m_EventBus;

        void HandleWeapons(float dt);
        void HandleHumanoids();
    public:
        Combat(entt::registry& registry, Core::Events::EventBus& eventBus) 
        :  m_SceneRegistry(registry), m_EventBus(eventBus) {}

        static void AttackEntity(entt::registry& r, Core::Events::EventBus& eventBus, entt::entity attacker, entt::entity attackee);
        static bool InHitWindow(const Weapons::Weapon& weapon);
        static bool InAttackRange(entt::registry& r, entt::entity enemy, entt::entity other);
        
        // Inline because they use wrap the static methods
        inline void AttackEntity(entt::entity attacker, entt::entity attackee);
        inline bool InAttackRange(entt::entity enemy, entt::entity other);

        void OnUpdate(float dt) override;
    };
}