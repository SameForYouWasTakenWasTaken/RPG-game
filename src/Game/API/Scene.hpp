#pragma once

#include <memory>
#include <vector>

#include <Engine/Events/EventBus.hpp>
#include "Layer.hpp"

#include "Systems/CombatSystem.hpp"
#include "Systems/EnemyAI.hpp"
#include "Systems/Hierarchy.hpp"
#include "Systems/MovementSystem.hpp"
#include "Systems/InventorySystem.hpp"
#include "Systems/PickupSystem.hpp"
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

        // Required systems
        Core::Events::EventBus eventBus;
        Game::Systems::Hierarchy hierarchy;
        
        // Basic systems
        Game::Systems::AISystem AI{registry, eventBus};
        Game::Systems::Combat Combat{registry, eventBus};
        
        Game::Systems::Inventory Inventory{registry};
        Game::Systems::Pickup Pickup{registry};
        Game::Systems::Movement Movement{registry};
        
        Scene();
        ~Scene();

        virtual void OnUpdate(float dt);
        virtual void OnFixed(float step);
        virtual void OnRender();

        virtual void OnAttach();
        virtual void OnDetach();

        void AddLayer(std::shared_ptr<Layers::ILayer> layer);
        void RemoveLayer(std::shared_ptr<Layers::ILayer> layer);
    };
}