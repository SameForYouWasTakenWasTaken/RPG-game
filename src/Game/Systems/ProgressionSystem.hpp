#pragma once

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

        int XPToNextLevel(int level);
        int CalculateBaseXP(int entityLevel);
        int CalculateXPGain(int level, int otherLevel, int baseXP);
    public:
        Progression(entt::registry& registry, Core::Events::EventBus& bus) 
        : m_SceneRegistry(registry), m_EventBus(bus) {}

        static void AddXP(Components::Progression& prog, int xpGain);
        void OnUpdate(float dt) override;
    };
}