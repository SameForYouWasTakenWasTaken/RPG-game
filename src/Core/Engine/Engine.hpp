#pragma once
#include "EngineContext.hpp"
#include "Events/EventBus.hpp"
#include "Window/Window.hpp"
#include "Renderer/Renderer.hpp"

namespace Core
{
    class Engine
    {
        Engine() = default;
        EngineContext m_Context{};
        Events::EventBus m_EventBus;

        std::unique_ptr<Rendering::Renderer> m_Renderer;
    public:
        ~Engine() = default;

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;
        
        static Engine& Get() {
            static Engine engine;
            return engine;
        }
        
        void Init(Rendering::Window& window);
        void HandleEvents(std::optional<sf::Event> event);

        EngineContext& GetContext() { return m_Context; }
        Events::EventBus& GetEventBus() { return m_EventBus; }
        Rendering::Renderer* GetRenderer() {assert(m_Renderer); return m_Renderer.get();}
    };
}