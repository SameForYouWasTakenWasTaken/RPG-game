#pragma once

#include "API/Layer.hpp"
#include "Engine/Events/EventBus.hpp"
#include "Engine/Events/InputEvent.hpp"
#include "Engine/Events/MouseEvent.hpp"
#include "Engine/Events/WindowResizeEvent.hpp"
#include "Engine/Systems/Scheduler.hpp"
#include "Events/AttackedEvent.hpp"
#include "Events/Died.hpp"
#include "Systems/IController.hpp"
#include "Systems/Controllers/PlayerMovementController.hpp"

namespace Game::Layers
{
    class MainLayer : public ILayer
    {
        entt::entity m_PlayerEntity{entt::null};

        void OnKey(Core::Events::KeyPressedEvent& e);
        void OnMouse(Core::Events::MouseClickEvent& e);
        void OnWindowResize(Core::Events::WindowResizeEvent& e);
        void OnAttack(Game::Events::Attacked& e);
        void OnDeath(Game::Events::Died& e);

        Core::Systems::Scheduler<Systems::IController> m_Controllers{};

        std::unique_ptr<Game::Systems::PlayerMovementController> m_PlayerMovementController;
    public:
        MainLayer() = default;

        void OnUpdate(float dt) override;
        void OnFixed(float step) override;
        void OnRender() override;
        void OnAttach() override;
        void OnDetach() override;

        void ConnectToEvents();
    };
}