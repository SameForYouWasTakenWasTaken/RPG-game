#include "Engine.hpp"
#include <cassert>

#include "glad/gl.h"

using namespace Core::Events;

namespace Core
{
    void Engine::Init(Rendering::Window& window)
    {
        m_Context.SFContext.majorVersion = 4;
        m_Context.SFContext.minorVersion = 6;

        m_Context.SFContext.attributeFlags = sf::ContextSettings::Attribute::Core;
        Rendering::Window* NewWindow = &window;

        m_Context.ActiveWindow = NewWindow;
        m_Context.ActiveWindow->GetRenderWindow().setFramerateLimit(165);


        m_Renderer = std::make_unique<Rendering::Renderer>(m_Context.ActiveWindow);
        m_Renderer->SetWindow(NewWindow);

        // 1. Let SFML get the function pointer manually for context check
        auto proc = sf::Context::getFunction("glGetString");
        if (!proc) {
            throw std::runtime_error("SFML failed to create an OpenGL context!");
        }

        // 2. Use GLAD's internal loader macro directly
        if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(sf::Context::getFunction))) {
            throw std::runtime_error("Failed to initialize OpenGL loader!");
        }
    }

    void Engine::HandleEvents(const std::optional<sf::Event>& event)
    {
        if (!event) return;

        // Unload cached events
        m_EventBus.Update();

        if (IsSFMLEvent<sf::Event::Closed>(event))
            m_EventBus.Emit<WindowCloseEvent>();

        if (auto e = IsSFMLEvent<sf::Event::Resized>(event))
            m_EventBus.Emit<WindowResizeEvent>(e->size.x, e->size.y);
    }
}