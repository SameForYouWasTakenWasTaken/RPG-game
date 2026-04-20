#pragma once

#include <memory>
#include <vector>

#include <Engine/Events/EventBus.hpp>
#include "Layer.hpp"

#include "Systems/Hierarchy.hpp"
#include "vendor/entt/entt.hpp"

namespace Game::Layers
{
    class ILayer;
}

namespace Game
{
    class Scene
    {
        std::vector<std::shared_ptr<Layers::ILayer>> m_Layers{};
    public:
        entt::registry registry{};
        Core::Events::EventBus eventBus;
        Game::Systems::Hierarchy hierarchy;
        
        Scene();
        ~Scene();

        virtual void OnUpdate(float dt);
        virtual void OnRender();

        virtual void OnAttach();
        virtual void OnDetach();

        void AddLayer(std::shared_ptr<Layers::ILayer> layer);
        void RemoveLayer(std::shared_ptr<Layers::ILayer> layer);
    };
}