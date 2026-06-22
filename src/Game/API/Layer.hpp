#pragma once

#include "API/Scene.hpp"
#include "Scene.hpp"

namespace Game
{
    class Scene;
}

namespace Game::Layers {

    class ILayer {
    protected:
        Game::Scene* m_Scene = nullptr;

        template <typename TEvent, typename TListener>
        void SinkEvent(entt::dispatcher& dispatcher);

        template <typename TComponent, typename TListener>
        auto ConnectOnConstructEvent(entt::registry& registry, entt::dispatcher& dispatcher);

        template <typename TComponent, typename TListener>
        auto ConnectOnDestroyEvent(entt::registry& registry, entt::dispatcher& dispatcher);

        template <typename TComponent, typename TListener>
        auto ConnectOnUpdateEvent(entt::registry& registry, entt::dispatcher& dispatcher);
    public:
        ILayer() = default;
        virtual ~ILayer() = default;

        virtual void OnUpdate(float dt) = 0;
        virtual void OnFixed(float step) {};

        virtual void OnRender() {}
        
        virtual void OnAttach() = 0;
        virtual void OnDetach() = 0;

        void SetParentScene(Game::Scene* scene) {m_Scene = scene;}
    };

    template <typename TEvent, typename TListener>
    void ILayer::SinkEvent(entt::dispatcher& dispatcher)
    {
        dispatcher.sink<TEvent>().template connect<TListener>(this);
    }

    template <typename TComponent, typename TListener>
    auto ILayer::ConnectOnConstructEvent(entt::registry& registry, entt::dispatcher& dispatcher)
    {
        return registry.on_construct<TComponent>().template connect<TListener>(this);
    }

    template <typename TComponent, typename TListener>
    auto ILayer::ConnectOnDestroyEvent(entt::registry& registry, entt::dispatcher& dispatcher)
    {
        return registry.on_destroy<TComponent>().template connect<TListener>(this);
    }

    template <typename TComponent, typename TListener>
    auto ILayer::ConnectOnUpdateEvent(entt::registry& registry, entt::dispatcher& dispatcher)
    {
        return registry.on_update<TComponent>().template connect<TListener>(this);
    }
}; // namespace Game::Layers