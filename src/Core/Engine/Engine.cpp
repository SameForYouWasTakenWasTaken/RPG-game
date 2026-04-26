#include "Engine.hpp"
#include <cassert>

using namespace Core::Events;

namespace Core
{
    void Engine::Init(Rendering::Window& window)
    {
        Rendering::Window* NewWindow = &window;

        m_Context.ActiveWindow = NewWindow;
        m_Context.ActiveWindow->GetRenderWindow().setFramerateLimit(165);

        m_Renderer = std::make_unique<Rendering::Renderer>(m_Context.ActiveWindow);
        m_Renderer->SetWindow(NewWindow);
    }

    void Engine::HandleEvents(std::optional<sf::Event> event)
    {
        // Unload cached events
        m_EventBus.Update();
        
        if (IsSFMLEvent<sf::Event::Closed>(event))
            m_EventBus.Emit <WindowCloseEvent>();

        if (auto e = IsSFMLEvent<sf::Event::Resized>(event))
            m_EventBus.Emit<WindowResizeEvent>(e->size.x, e->size.y);
    }
}