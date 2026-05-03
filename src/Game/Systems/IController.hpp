#pragma once

#include "Engine/Events/EventBus.hpp"
#include "vendor/entt/entt.hpp"

namespace Game::Systems
{
    class IController
    {
    protected:
        entt::registry& m_SceneRegistry;
        Core::Events::EventBus& m_EventBus;
    public:
        IController(entt::registry& registry, Core::Events::EventBus& bus)
            : m_SceneRegistry(registry), m_EventBus(bus) {}

        virtual void OnUpdate(float dt) {}
        virtual void OnFixed(float step) {}

        template <typename Func, typename... TComponents>
        void ForEachEntity(Func&& func)
        {
            auto view = m_SceneRegistry.view<TComponents...>();
            
            for (auto entity : view)
                func(entity);
        }
    };
}