#pragma once

#include "Engine/Events/EventBus.hpp"
#include "Engine/Events/WindowResizeEvent.hpp"
#include "Systems/IController.hpp"

namespace Game::Systems
{
    class PlayerMovementController : public IController
    {
        entt::entity m_MainPlayer{entt::null};

        void OnWindowResize(Core::Events::WindowResizeEvent& e);
    public:
        bool AllowMovement = true;

        PlayerMovementController(entt::registry& registry, Core::Events::EventBus& bus)
            : IController(registry, bus) 
        {
            m_EventBus.Sink<Core::Events::WindowResizeEvent>().connect<&PlayerMovementController::OnWindowResize>(this);
        }

        void OnUpdate(float dt) override;

        void SetMainPlayer(entt::entity entity) {m_MainPlayer = entity;}
        void CenterCameraToPlayer();
    };
}