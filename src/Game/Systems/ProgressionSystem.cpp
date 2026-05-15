#include "ProgressionSystem.hpp"
#include "Components/Humanoid.hpp"
#include "Components/Progression.hpp"
#include "Events/LevelUp.hpp"
#include <iostream>
#include <iterator>

namespace Game::Systems
{
    int Progression::XPToNextLevel(int level)
    {
        constexpr int baseXP = 100;
        constexpr float exponent = 1.8f;
        constexpr float offset = 2.0f; // smoother early game

        return baseXP * std::pow(level + offset, exponent);
    }

    int Progression::CalculateBaseXP(int entityLevel)
    {
        constexpr float c = 20.0f;
        return static_cast<int>(c * std::pow(entityLevel, 1.5f));
    }

    int Progression::CalculateXPGain(int level, int otherLevel, int baseXP)
    {
        constexpr float k = 1.6f;

        // prevent division by 0
        level = level == 0 ? 1 : level;
        float ratio = static_cast<float>(otherLevel) / level;

        // Prevent abuse
        ratio = std::clamp(ratio, 0.5f, 2.0f);

        float xp = baseXP * std::pow(ratio, k);

        return static_cast<int>(xp);
    }

    void Progression::AddXP(Components::Progression& prog, int xpGain)
    {
        prog.XP += xpGain;
    }

    void Progression::OnLevelUp(Events::LevelUp& e)
    {
        auto* humanoid = m_SceneRegistry.try_get<Components::Humanoid>(e.entity);
        auto* stats = m_SceneRegistry.try_get<Components::CombatStats>(e.entity);
        if (!humanoid || !stats) return;

        RecalculateMaxHealth(*stats, *humanoid);
    }
    
    void Progression::RecalculateMaxHealth(Components::CombatStats &stats, Components::Humanoid &humanoid)
    {
        float newMax = 100.0f + stats.Vitality * 10.0f;

        float ratio = humanoid.Health / humanoid.MaxHealth;

        humanoid.MaxHealth = newMax;
        humanoid.Health = ratio * newMax; // preserve % HP
    }

    void Progression::OnUpdate(float dt)
    {
        auto view = m_SceneRegistry.view<Game::Components::Progression>();

        for (auto entity : view)
        {
            auto& progression = m_SceneRegistry.get<Components::Progression>(entity);

            if (!progression.CanEarnXP) continue;
            int xpNeeded = XPToNextLevel(progression.Level);

            while (progression.XP >= xpNeeded)
            {
                progression.XP -= xpNeeded;
                progression.Level++;
                m_EventBus.Queue<Events::LevelUp>(entity, progression.Level);
                xpNeeded = XPToNextLevel(progression.Level);
            }
        }
    }
}