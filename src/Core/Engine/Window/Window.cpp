#include "Window.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Events/WindowCloseEvent.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/View.hpp"
#include "SFML/System/Vector2.hpp"

namespace Core::Rendering
{
    Window::Window(WindowData settings)  : m_Data(settings)
    {
        m_Window = sf::RenderWindow(
            sf::VideoMode({m_Data.width, m_Data.height}), 
            m_Data.title,
            m_Data.style,
            m_Data.state
        );

        sf::Vector2f size = {
            static_cast<float>(m_Data.width),
            static_cast<float>(m_Data.height)
        };

        ResetView();
        
        auto& eventBus = Core::Engine::Get().GetEventBus();
        eventBus.Sink<Core::Events::WindowCloseEvent>().connect<&Window::OnCloseEvent>(this);
        eventBus.Sink<Core::Events::WindowResizeEvent>().connect<&Window::OnResize>(this);
    }

    void Window::Close()
    {
        m_Window.close();
    }


    void Window::Clear()
    {
        m_Window.clear(sf::Color::Blue);
    }

    void Window::Display()
    {
        m_Window.display();
    }

    void Window::OnCloseEvent(Core::Events::WindowCloseEvent& e)
    {
       Close();
    }

    void Window::OnResize(Core::Events::WindowResizeEvent& e)
    {
        float w = static_cast<float>(e.Width);
        float h = static_cast<float>(e.Height);
        
        sf::Vector2f size = {w, h};

        m_Data.height = e.Height;
        m_Data.width = e.Width;

        View.setViewport(sf::FloatRect{sf::Vector2f{0.f, 0.f}, sf::Vector2f{1.f, 1.f}});
        ResetView();
    }

    void Window::ResetView()
    {
        View.setSize(
            {            
            static_cast<float>(m_Data.width),
            static_cast<float>(m_Data.height)
            }
        );

        m_Window.setView(View);
    }
}