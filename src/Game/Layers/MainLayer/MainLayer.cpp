#include <iostream>

#include <tracy/Tracy.hpp>
#include "MainLayer.hpp"
#include "Components/Humanoid.hpp"
#include "Components/Inventory.hpp"
#include "Components/InventoryItem.hpp"
#include "Engine/Components/Geometry.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Events/InputEvent.hpp"
#include "Engine/Events/MouseEvent.hpp"
#include "Engine/Events/WindowResizeEvent.hpp"
#include "Engine/Renderer/Types.hpp"
#include "Entities/Weapon.hpp"
#include "Entities/Coin.hpp"
#include "Events/AttackedEvent.hpp"
#include "Events/Died.hpp"
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

namespace Game::Layers
{
        void MainLayer::OnUpdate(float dt) 
        {
            bool walking = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ) 
                    walking = true;

                        auto& registry = m_Scene->registry;
            
            auto& transform = registry.get<Core::Components::Transform>(m_PlayerEntity);
            auto& humanoid = registry.get<Components::Humanoid>(m_PlayerEntity);
            float speed = humanoid.Speed * dt;
                    
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

        void MainLayer::OnFixed(float step)
        {
        }

        /**
         * @brief Submits all entities with sprite, transform, and geometry components to the renderer for drawing.
         *
         * For each matching entity, if the sprite's texture handle is valid this fetches the texture and submits a RenderObject
         * composed of the sprite's texture handle, the resolved texture, the entity transform, and its geometry to the engine renderer.
         */
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
            m_Scene->eventBus.Sink<Game::Events::Attacked>().connect<&MainLayer::OnAttack>(this);
            m_Scene->eventBus.Sink<Game::Events::Died>().connect<&MainLayer::OnDeath>(this);
        }

        void MainLayer::OnMouse(Core::Events::MouseClickEvent& e)
        {
        }

        void MainLayer::OnDeath(Game::Events::Died& e)
        {
            std::cout << "Entity died!" << std::endl;
        }

        /**
         * @brief Handles key-press events to spawn entities at the current mouse world position.
         *
         * When the F key is pressed, creates an enemy entity and positions it at the mouse cursor's
         * world coordinates. When the G key is pressed, creates a coin entity and positions it at
         * the mouse cursor's world coordinates.
         *
         * @param e Key press event containing the pressed key.
         */
        void MainLayer::OnKey(Core::Events::KeyPressedEvent& e)
        {
            auto& registry = m_Scene->registry;
            
            if (e.key == sf::Keyboard::Key::F)
            {
                auto enemy = Entities::CreateEnemyEntity(registry);
                auto& Transform = registry.get<Core::Components::Transform>(enemy);

                auto& window = Core::Engine::Get().GetContext().ActiveWindow;
                auto& rw = window->GetRenderWindow();

                sf::Vector2i pixelPos = sf::Mouse::getPosition(rw);
                auto world = rw.mapPixelToCoords(pixelPos, window->View);

                Transform.SetPosition({world.x, world.y});
            }

            if (e.key == sf::Keyboard::Key::G)
            {
                auto& window = Core::Engine::Get().GetContext().ActiveWindow;
                auto& rw = window->GetRenderWindow();

                sf::Vector2i pixelPos = sf::Mouse::getPosition(rw);
                auto world = rw.mapPixelToCoords(pixelPos, window->View);
                
                auto coin = Game::Entities::CreateCoin(registry);
                
                auto& Transform = registry.get<Core::Components::Transform>(coin);
                
                Transform.SetPosition({world.x, world.y});
            }
        }

        void MainLayer::OnWindowResize(Core::Events::WindowResizeEvent& e)
        {
            auto& window = Core::Engine::Get().GetContext().ActiveWindow;
            CenterCameraToEntity(m_PlayerEntity);
        }

        /**
         * @brief Centers the active window's camera on the specified entity.
         *
         * Computes the target center from the entity's world position plus its local origin,
         * applies that position to the active window's view, and resets the view.
         *
         * @param player Entity whose transform will be used as the camera center.
         */
        void MainLayer::CenterCameraToEntity(entt::entity player)
        {
            auto& window = Core::Engine::Get().GetContext().ActiveWindow;
            auto& transform = m_Scene->registry.get<Core::Components::Transform>(player);

            glm::vec2 pos = transform.GetWorldPos();
            pos += transform.GetLocalOrigin();

            window->View.setCenter(sf::Vector2f{pos.x, pos.y});
            window->ResetView();
        }

        void MainLayer::OnAttack(Game::Events::Attacked& e)
        {
            auto attacker = e.Attacker;
            auto attackee = e.Attackee;
            auto damage = e.DamageTaken;

            auto& Humanoid = m_Scene->registry.get<Game::Components::Humanoid>(attackee);
            Humanoid.Health -= damage;

            std::cout << "Attacked an entity!\n" << "HP: " << Humanoid.Health << std::endl;
        }
}