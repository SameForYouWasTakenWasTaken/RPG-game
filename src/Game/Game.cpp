#include "Game.hpp"
#include "Engine/Engine.hpp"

#include "Engine/EngineContext.hpp"
#include "Engine/Events/WindowResizeEvent.hpp"
#include "Engine/Window/Window.hpp"
#include "GameSettings.hpp"
#include "API/Scene.hpp"
#include "Layers/MainLayer/MainLayer.hpp"
#include "Layers/RenderLayer/RenderLayer.hpp"
#include <memory>

#include "Engine/Events/InputEvent.hpp"
#include "Engine/Events/WindowCloseEvent.hpp"
#include "Engine/Events/MouseEvent.hpp"
#include "tracy/Tracy.hpp"

using namespace Core::Events;

namespace Game
{
    Application::Application(GameSettings settings)
    : m_Window({
        .width = settings.windowWidth,
        .height = settings.windowHeight,
        .title = settings.windowTitle,
    })
    {
        Core::Engine& engine = Core::Engine::Get();
        engine.Init(m_Window);

    }
    
    void Application::Run()
    {
        Core::Engine& engine = Core::Engine::Get();

        Core::EngineContext& context = engine.GetContext(); 

        Scene MainScene;

        auto MainLayer = std::make_shared<Layers::MainLayer>();
        auto RenderLayer = std::make_shared<Layers::RenderLayer>();
        MainScene.AddLayer(MainLayer);
        MainScene.AddLayer(RenderLayer);

        sf::Clock clock;
        float then = clock.getElapsedTime().asSeconds();
        
        constexpr float MAX_DELTA = .05f;
        constexpr int MAX_EVENTS = 50;
        constexpr float STEP = 1.f / 60.f;

        while (m_Window.IsOpen())
        {
            float now = clock.getElapsedTime().asSeconds();
            float dt = now - then;
            dt = std::min(dt, MAX_DELTA);
            then = now;
            
            int count = 0;
            while ((PollEvents(MainScene, m_Window.PollEvents())) && count < MAX_EVENTS) {
                ++count;
            }

            m_Window.Clear();

            MainScene.OnUpdate(dt);
            MainScene.OnFixed(STEP);
            MainScene.OnRender();

            m_Window.Display();
            
            FrameMark;
        }

        m_Window.Close();
    }

    bool Application::PollEvents(Scene& scene, const std::optional<sf::Event> event)
    {
        if (!event) return false;
        
        auto& eventBus = scene.eventBus;
        auto& engine = Core::Engine::Get();

        if (IsSFMLEvent<sf::Event::Closed>(event))
            eventBus.Queue<WindowCloseEvent>();

        if (auto e = IsSFMLEvent<sf::Event::Resized>(event))
            eventBus.Queue<WindowResizeEvent>(e->size.x, e->size.y);

        if (auto e = IsSFMLEvent<sf::Event::KeyPressed>(event))
            eventBus.Emit<KeyPressedEvent>(e->code, e->scancode);

        if (auto e = IsSFMLEvent<sf::Event::KeyReleased>(event))
            eventBus.Emit<KeyReleasedEvent>(e->code, e->scancode);

        if (auto e = IsSFMLEvent<sf::Event::MouseButtonPressed>(event))
            eventBus.Emit<MouseClickEvent>(e->button, e->position.x, e->position.y);

        if (auto e = IsSFMLEvent<sf::Event::MouseButtonReleased>(event))
            eventBus.Emit<MouseReleaseEvent>(e->button, e->position.x, e->position.y);

        if (auto e = IsSFMLEvent<sf::Event::MouseWheelScrolled>(event))
            eventBus.Emit<MouseScrollEvent>(e->wheel, e->delta, e->position.x, e->position.y);

        engine.HandleEvents(event);

        return true;
    }

}