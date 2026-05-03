#pragma once

#include "Components/Humanoid.hpp"
#include "Engine/Events/EventBus.hpp"
#include "Engine/Systems/ISystem.hpp"
#include "vendor/entt/entt.hpp"
#include "Components/Progression.hpp"

namespace Game::Systems
{
    class Progression : public Core::Systems::ISystem
    {
        entt::registry& m_SceneRegistry;
        Core::Events::EventBus& m_EventBus;
    public:
        Progression(entt::registry& registry, Core::Events::EventBus& bus) 
        : m_SceneRegistry(registry), m_EventBus(bus) {}
        
        static int XPToNextLevel(int level);
        static int CalculateBaseXP(int entityLevel);
        static int CalculateXPGain(int level, int otherLevel, int baseXP);
        static void RecalculateMaxHealth(Components::CombatStats& stats, Components::Humanoid& humanoid);

        static inline void AddXP(Components::Progression& prog, int xpGain);

        void OnUpdate(float dt) override;
    };
}