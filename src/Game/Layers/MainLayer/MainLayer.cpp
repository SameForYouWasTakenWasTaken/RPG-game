#include <iostream>

#include <memory>
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
#include "Components/Weapon.hpp"
#include "Entities/Coin.hpp"
#include "Events/AttackedEvent.hpp"
#include "Events/Died.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "Systems/InventorySystem.hpp"
#include "Systems/PickupSystem.hpp"
#include "Systems/ResourceManager.hpp"

#include "Engine/Components/Transform.hpp"
#include "Components/Sprite.hpp"

#include "Entities/Player.hpp"
#include "Entities/Enemy.hpp"

namespace Game::Layers
{
        void MainLayer::OnUpdate(float dt) 
        {
            m_Controllers.RunAll([dt](Systems::IController& controller){
                controller.OnUpdate(dt);
            });
            
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
            auto view = registry.view<
                Game::Components::Sprite, 
                Core::Components::Transform, 
                Core::Components::Geometry>();

            for (auto entity : view)
            {
                auto& sprite = registry.get<Game::Components::Sprite>(entity);
                auto& transform = registry.get<Core::Components::Transform>(entity);
                auto& geometry = registry.get<Core::Components::Geometry>(entity);
                
                if (!sprite.TextureHandle.IsValid()) continue;
               
                if (m_Scene->Inventory.IsItem(entity))
                    if (!m_Scene->Inventory.IsOwnedBy(entt::null, entity)) // Don't draw owned items
                        continue;
                
                const sf::Texture& texture = Systems::ResourceManager::GetTexture(sprite.TextureHandle);
                
                Core::Rendering::RenderObject obj{sprite.TextureHandle, &texture, &transform, &geometry};
                obj.zIndex = sprite.zIndex;
                renderer->Submit(obj);
            }
        }

        void MainLayer::OnAttach() 
        {
            auto& registry = m_Scene->registry;

            auto player = Game::Entities::CreatePlayerEntity(registry);
            m_PlayerEntity = player;

            m_PlayerMovementController = std::make_unique<Game::Systems::PlayerMovementController>(registry, m_Scene->eventBus);
            m_PlayerMovementController->SetMainPlayer(m_PlayerEntity);
            m_PlayerMovementController->CenterCameraToPlayer();

            m_Controllers.Add(*m_PlayerMovementController);
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