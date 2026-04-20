#pragma once

#include <string>

#include "SFML/Graphics.hpp"
#include <SFML/Window.hpp>
#include "SFML/Window/WindowEnums.hpp"
#include "vendor/entt/entt.hpp"

#include "Engine/Events/WindowCloseEvent.hpp"
#include "Engine/Events/WindowResizeEvent.hpp"


namespace Core::Rendering
{
    struct WindowData
    {
        unsigned int width{};
        unsigned int height{};

        std::uint32_t style = sf::Style::Default;
        sf::State state = sf::State::Windowed;
        
        std::string title{};
    };

    class Window
    {
    protected:
        sf::RenderWindow m_Window;
        WindowData m_Data;

        void OnCloseEvent(Core::Events::WindowCloseEvent& e);
        void OnResize(Core::Events::WindowResizeEvent& e);
    public:
        sf::View View;

        Window(WindowData settings);
        virtual ~Window() = default;

        virtual WindowData& GetData() { return m_Data; }

        virtual bool IsOpen() { return m_Window.isOpen(); }
        virtual std::optional<sf::Event> PollEvents() { return m_Window.pollEvent(); }

        void Close();
        void Clear();
        void Display();
        void ResetView();

        sf::RenderWindow& GetRenderWindow() {return m_Window;}
    };
}