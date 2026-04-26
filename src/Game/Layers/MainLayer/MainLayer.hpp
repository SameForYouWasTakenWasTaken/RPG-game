#pragma once

#include "API/Layer.hpp"
#include "Engine/Events/EventBus.hpp"
#include "Engine/Events/InputEvent.hpp"
#include "Engine/Events/MouseEvent.hpp"
#include "Engine/Events/WindowResizeEvent.hpp"
#include "Events/AttackedEvent.hpp"
#include "Events/Died.hpp"

namespace Game::Layers
{
    class MainLayer : public ILayer
    {
        entt::entity m_PlayerEntity{entt::null};
        void CenterCameraToEntity(entt::entity player);

        void OnKey(Core::Events::KeyPressedEvent& e);
        void OnMouse(Core::Events::MouseClickEvent& e);
        void OnWindowResize(Core::Events::WindowResizeEvent& e);
        void OnAttack(Game::Events::Attacked& e);
        void OnDeath(Game::Events::Died& e);

        bool PlayerLeft = false;
        bool PlayerRight = false;
        bool PlayerUp = false;
        bool PlayerDown = false;
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