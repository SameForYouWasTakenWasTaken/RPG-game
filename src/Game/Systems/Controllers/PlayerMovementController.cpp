#include "PlayerMovementController.hpp"
#include "Components/Humanoid.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Events/WindowResizeEvent.hpp"
#include "SFML/Window/Keyboard.hpp"

namespace Game::Systems
{
    void PlayerMovementController::OnUpdate(float dt)
    {
        if (!AllowMovement) return;
        if (m_MainPlayer == entt::null) return;
        
            glm::vec2 input{0.f, 0.f};
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) input.y -= 1.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) input.x -= 1.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) input.y += 1.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) input.x += 1.f;

            if (input != glm::vec2{0.f, 0.f})
            {
                auto& transform = m_SceneRegistry.get<Core::Components::Transform>(m_MainPlayer);
                auto& humanoid = m_SceneRegistry.get<Components::Humanoid>(m_MainPlayer);
                input = glm::normalize(input);
                transform.Move(input * humanoid.Speed * dt);
                CenterCameraToPlayer();
            }
    }

    /**
     * @brief Centers the active window's camera on the player.
     *
     * Computes the target center from the entity's world position plus its local origin,
     * applies that position to the active window's view, and resets the view.
    */
    void PlayerMovementController::CenterCameraToPlayer()
    {
        if (m_MainPlayer == entt::null) return;

        auto& window = Core::Engine::Get().GetContext().ActiveWindow;
        auto& transform = m_SceneRegistry.get<Core::Components::Transform>(m_MainPlayer);
       
        glm::vec2 pos = transform.GetWorldPos();
        pos += transform.GetLocalOrigin();
        
        window->View.setCenter(sf::Vector2f{pos.x, pos.y});
        window->ResetView();
    }

    void PlayerMovementController::OnWindowResize(Core::Events::WindowResizeEvent& e)
    {
        CenterCameraToPlayer();
    }
}