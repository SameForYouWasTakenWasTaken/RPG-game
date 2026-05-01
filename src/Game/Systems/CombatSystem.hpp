#pragma once

#include "Engine/Events/EventBus.hpp"
#include "Engine/Systems/ISystem.hpp"
#include "Components/Weapon.hpp"
#include "vendor/entt/entt.hpp"

namespace Game::Systems
{
    class Combat : public Core::Systems::ISystem
    {
        entt::registry& m_SceneRegistry;
        Core::Events::EventBus& m_EventBus;

        void HandleWeapons(float dt);
        void HandleHumanoids(float dt);
    public:
        /**
         * @brief Constructs a Combat system bound to a scene registry and an event bus.
         *
         * Stores references to the provided `entt::registry` and `Core::Events::EventBus` for the
         * system's lifetime so it can operate on scene entities and emit or consume events.
         */
        Combat(entt::registry& registry, Core::Events::EventBus& eventBus) 
        :  m_SceneRegistry(registry), m_EventBus(eventBus) {}

        static void AttackEntity(entt::registry& r, Core::Events::EventBus& eventBus, entt::entity attacker, entt::entity attackee);
        static bool InHitWindow(const Components::Weapon& weapon);
        static bool InAttackRange(entt::registry& r, entt::entity enemy, entt::entity other);
        
        // Inline because they use wrap the static methods
        inline void AttackEntity(entt::entity attacker, entt::entity attackee);
        inline bool InAttackRange(entt::entity enemy, entt::entity other);

        void OnUpdate(float dt) override;
    };
}