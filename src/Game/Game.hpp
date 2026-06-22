#pragma once

#include "API/Scene.hpp"
#include "Engine/Window/Window.hpp"
#include "GameSettings.hpp"
#include "SFML/Window/Event.hpp"

namespace Game
{
    class Application final {

        Core::Rendering::Window m_Window;

        bool PollEvents(Scene& scene, std::optional<sf::Event> e);
    public:
        Application(GameSettings settings);
        ~Application();

        void Run();
    };
}