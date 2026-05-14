#include "RewardSystem.hpp"
#include "Components/Progression.hpp"
#include "Events/RewardEvent.hpp"
namespace Game::Systems
{
    void Reward::Award(entt::entity entity, Components::Reward reward)
    {
        auto& Progression = m_SceneRegistry.get<Components::Progression>(entity);

        Progression.Gold += reward.Gold;
        Progression.XP += reward.XP;

        m_EventBus.Queue<Events::Reward>(entity, reward);       
    }
}