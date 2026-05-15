#pragma once

#include "SFML/Graphics.hpp"
#include "vendor/entt/entt.hpp"

namespace Core::Events
{
    class EventBus
    {
        entt::dispatcher m_Dispatcher;
        public:
        EventBus() = default;
        ~EventBus()
        {
            m_Dispatcher.clear();
            
        }

        template <typename TEventFire, typename... Args>
        void Queue(Args&&... args);

        template <typename TEventFire, typename... Args>
        void Emit(Args&... args);

        template<typename TEventU>
        void Update() {m_Dispatcher.update<TEventU>();} // Update single event
        void Update() {m_Dispatcher.update();} // Update all events


        template <typename TEvent>
        auto Sink() {return m_Dispatcher.sink<TEvent>();}
    };

    template <typename TEventFire, typename... Args>
    void EventBus::Queue(Args&&... args)
    {
        m_Dispatcher.enqueue(TEventFire{std::forward<Args>(args)...});
    }

    template <typename TEventFire, typename... Args>
    void EventBus::Emit(Args&... args)
    {
        m_Dispatcher.trigger(TEventFire{std::forward<Args>(args)...});
    }

    
    template <typename TEventReceive>
    TEventReceive* IsSFMLEvent(std::optional<sf::Event> event)
    {
        if (auto* e = event->getIf<TEventReceive>())
            return e;

        return nullptr;
    }
}