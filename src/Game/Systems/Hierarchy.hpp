#pragma once

#include <glm/glm.hpp>
#include "Engine/Events/EventBus.hpp"
#include "Engine/Systems/ISystem.hpp"
#include "vendor/entt/entt.hpp"


namespace Game::Systems
{
    class Hierarchy : public Core::Systems::ISystem
    {
        entt::registry& m_SceneRegistry;
        Core::Events::EventBus& m_EventBus;

        void UpdateRecursive(entt::entity entity, const glm::mat3& parentWorld);
    public:
        Hierarchy(entt::registry& registry, Core::Events::EventBus& eventBus);

        void OnUpdate(float dt) override;
        void AddChild(entt::entity parent, entt::entity child);

        void Traverse(entt::entity parent);
    };
}