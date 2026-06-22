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

#include "imgui-SFML.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Engine/Events/InputEvent.hpp"
#include "Engine/Events/WindowCloseEvent.hpp"
#include "Engine/Events/MouseEvent.hpp"
#include "Layers/UI/Debug/DefaultDebug.hpp"
#include "tracy/Tracy.hpp"

using namespace Core::Events;

namespace Game
{
    Application::Application(GameSettings settings)
    {
        Core::Engine& engine = Core::Engine::Get();

        // Configure OpenGL context settings before Window creation
        engine.GetContext().SFContext.majorVersion = 4;
        engine.GetContext().SFContext.minorVersion = 6;
        engine.GetContext().SFContext.attributeFlags = sf::ContextSettings::Attribute::Core;

        // Now construct Window with proper context settings
        m_Window = Core::Rendering::Window({
            .width = settings.windowWidth,
            .height = settings.windowHeight,
            .title = settings.windowTitle,
        });

        engine.Init(m_Window);

        if (!ImGui::SFML::Init(m_Window.GetRenderWindow()))
            throw std::runtime_error("SFML Init Failed");
    }

    Application::~Application()
    {
        ImGui::SFML::Shutdown();
    }

    void Application::Run()
    {
        Core::Engine& engine = Core::Engine::Get();

        Core::EngineContext& context = engine.GetContext(); 

        Scene MainScene;
        auto MainLayer = std::make_shared<Layers::MainLayer>();
        auto RenderLayer = std::make_shared<Layers::RenderLayer>();
        auto DebugLayer = std::make_shared<Layers::DefaultDebug>();

        MainScene.AddLayer(MainLayer);
        MainScene.AddLayer(RenderLayer);
        MainScene.AddLayer(DebugLayer);

        sf::Clock clock;
        float then = clock.getElapsedTime().asSeconds();
        
        constexpr float MAX_DELTA = .05f;
        constexpr float STEP = 1.f / 60.f;

        while (m_Window.IsOpen())
        {
            float now = clock.getElapsedTime().asSeconds();
            float dt = now - then;
            dt = std::min(dt, MAX_DELTA);
            then = now;
            
            int count = 0;
            while (PollEvents(MainScene, m_Window.PollEvents())) {
                ++count;
            }

            // 2. START THE FRAME HERE.
            // This turns WithinFrameScope to TRUE before anything else runs.
            ImGui::SFML::Update(m_Window.GetRenderWindow(), sf::seconds(dt));

            // 3. Now these are 100% safe, even if they contain ImGui logic!
            assert(ImGui::GetCurrentContext()->WithinFrameScope == true);
            MainScene.OnUpdate(dt);

            assert(ImGui::GetCurrentContext()->WithinFrameScope == true);
            MainScene.OnFixed(STEP);

            // 4. Render pass
            m_Window.Clear();

            MainScene.OnRender();

            ImGui::SFML::Render(m_Window.GetRenderWindow());
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
        ImGui::SFML::ProcessEvent(m_Window.GetRenderWindow(), event.value());


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