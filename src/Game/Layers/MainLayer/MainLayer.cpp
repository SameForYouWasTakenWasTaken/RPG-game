#include <iostream>
#include <tracy/Tracy.hpp>


#include "MainLayer.hpp"
#include "Components/Enemy.hpp"
#include "Components/Humanoid.hpp"
#include "Engine/Components/Geometry.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Events/InputEvent.hpp"
#include "Engine/Events/MouseEvent.hpp"
#include "Engine/Events/WindowResizeEvent.hpp"
#include "Engine/Renderer/Types.hpp"
#include "Entities/Default.hpp"
#include "Global/Helpers.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "Systems/ResourceManager.hpp"

#include "Engine/Components/Transform.hpp"
#include "Components/Sprite.hpp"

#include "Entities/Player.hpp"
#include "Entities/Enemy.hpp"
#include "glm/geometric.hpp"


namespace Game::Layers
{
        void MainLayer::OnUpdate(float dt) 
        {
            ZoneScoped;
            auto& registry = m_Scene->registry;
            auto& window = Core::Engine::Get().GetContext().ActiveWindow; // TODO: Set camera position on player

            auto playerView = registry.view<Entities::PlayerTag>();
            auto enemyView = registry.view<Entities::EnemyTag>();
            
            if (m_PlayerEntity != entt::null)
            {
                auto& transform = registry.get<Core::Components::Transform>(m_PlayerEntity);
                auto& humanoid = registry.get<Components::Humanoid>(m_PlayerEntity);
                float speed = humanoid.Speed * dt;
                bool walking = false;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ) 
                        walking = true;
                        
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                    transform.Move(glm::vec2{0.f, -speed});
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                    transform.Move(glm::vec2{-speed, 0.f});
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                    transform.Move(glm::vec2{0.f, speed});
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                    transform.Move(glm::vec2{speed, 0.f});

                if (walking)
                    CenterCameraToEntity(m_PlayerEntity);
            }
            
            for (auto enemyEntity : enemyView)
            {
                auto& enemyTransform = registry.get<Core::Components::Transform>(enemyEntity);
                auto& enemyHumanoid = registry.get<Components::Humanoid>(enemyEntity);
                auto& enemyComponent = registry.get<Components::Enemy>(enemyEntity);

                entt::entity nearestPlayer{entt::null};
                Core::Components::Transform nearestPlayerTransform;
                
                // Get nearest player entity
                for (auto playerEntity : playerView)
                {
                    auto& playerTransform = registry.get<Core::Components::Transform>(playerEntity);
                   
                    glm::vec2 delta = enemyTransform.GetLocalPosition() - playerTransform.GetLocalPosition();

                    if  (glm::length(delta) > enemyComponent.PlayerDetectionDist)
                        continue;

                    if (nearestPlayer == entt::null)
                    {
                        nearestPlayer = playerEntity;
                        nearestPlayerTransform = playerTransform;
                        continue;   
                    }

                    if (glm::length(nearestPlayerTransform.GetLocalPosition()) > glm::length(playerTransform.GetLocalPosition()))
                        nearestPlayerTransform = playerTransform;
                    
                }
                
                if (nearestPlayer == entt::null)
                    continue;

                // Approach nearest player
                glm::vec2 direction = glm::normalize(nearestPlayerTransform.GetLocalPosition() - enemyTransform.GetLocalPosition());
                enemyTransform.Move(direction * enemyHumanoid.Speed * dt);
                
            }
        }

        void MainLayer::OnRender() 
        {
            ZoneScoped;
            Core::Rendering::Renderer* renderer = Core::Engine::Get().GetRenderer();

            auto& registry = m_Scene->registry;
            auto view = registry.view<Game::Components::Sprite, Core::Components::Transform, Core::Components::Geometry>();

            view.each([&](
                auto entity, 
                const Game::Components::Sprite& sprite, 
                Core::Components::Transform& transform,
                Core::Components::Geometry& geometry
            ){
                if (!sprite.TextureHandle.IsValid()) return;
                
                const sf::Texture& texture = Systems::ResourceManager::GetTexture(sprite.TextureHandle);
                
                Core::Rendering::RenderObject obj{sprite.TextureHandle, &texture, transform, geometry};
                renderer->Submit(obj);
            });
        }

        void MainLayer::OnAttach() 
        {
            auto& registry = m_Scene->registry;

            auto player = Game::Entities::CreatePlayerEntity(registry);
            m_PlayerEntity = player;

            CenterCameraToEntity(m_PlayerEntity);
            ConnectToEvents();
        }

        void MainLayer::OnDetach() 
        {
            
        }

        void MainLayer::ConnectToEvents()
        {
            Core::Engine& engine = Core::Engine::Get();

            m_Scene->eventBus.Sink<Core::Events::KeyPressedEvent>().connect<&MainLayer::OnKey>(this);
            m_Scene->eventBus.Sink<Core::Events::MouseClickEvent>().connect<&MainLayer::OnMouse>(this);
            m_Scene->eventBus.Sink<Core::Events::WindowResizeEvent>().connect<&MainLayer::OnWindowResize>(this);
        }

        void MainLayer::OnMouse(Core::Events::MouseClickEvent& e)
        {
        }

        void MainLayer::OnKey(Core::Events::KeyPressedEvent& e)
        {
            
            if (e.key == sf::Keyboard::Key::F)
            {
                auto& registry = m_Scene->registry;

                auto enemy = Entities::CreateEnemyEntity(registry);
                auto& Transform = registry.get<Core::Components::Transform>(enemy);

                sf::Vector2i mousePos = sf::Mouse::getPosition();
                sf::Vector2i pixelPos = {mousePos.x, mousePos.y};
                
                auto& window = Core::Engine::Get().GetContext().ActiveWindow;
                auto world = window->GetRenderWindow().mapPixelToCoords(pixelPos, window->View);


                Transform.SetPosition({world.x, world.y});
                Transform.Move(-Transform.GetLocalSize() / 2.f);
            }
        }

        void MainLayer::OnWindowResize(Core::Events::WindowResizeEvent& e)
        {
            auto& window = Core::Engine::Get().GetContext().ActiveWindow;
            CenterCameraToEntity(m_PlayerEntity);
        }

        void MainLayer::CenterCameraToEntity(entt::entity player)
        {
            auto& window = Core::Engine::Get().GetContext().ActiveWindow;
            auto& transform = m_Scene->registry.get<Core::Components::Transform>(player);

            glm::vec2 pos = transform.GetWorldPos();
            pos += transform.GetLocalSize();

            window->View.setCenter(sf::Vector2f{pos.x, pos.y});
            window->ResetView();
        }
}