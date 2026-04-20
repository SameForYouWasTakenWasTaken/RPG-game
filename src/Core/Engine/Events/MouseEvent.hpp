#pragma once

#include "SFML/Graphics.hpp"
#include "IEvent.hpp"
#include "SFML/Window/Mouse.hpp"

namespace Core::Events
{
    class MouseClickEvent : public IEvent
    {
    public:
        sf::Mouse::Button Button;
        int xPos, yPos;
        
        MouseClickEvent(sf::Mouse::Button button, int x, int y)
        : Button(button), xPos(x), yPos(y) {}

        EVENT_TYPE(MouseClickEvent)
    };

    class MouseReleaseEvent : public IEvent
    {
    public:
        sf::Mouse::Button Button;
        int xPos, yPos;
        
        MouseReleaseEvent(sf::Mouse::Button button, int x, int y)
        : Button(button), xPos(x), yPos(y) {}
        
        EVENT_TYPE(MouseReleaseEvent)
    };

    class MouseScrollEvent : public IEvent
    {
    public:
        sf::Mouse::Wheel Wheel;
        float delta;
        int xPos, yPos;
        
        MouseScrollEvent(sf::Mouse::Wheel wheel, float delta, int x, int y)
        : Wheel(wheel), delta(delta), xPos(x), yPos(y) {}
        
        EVENT_TYPE(MouseScrollEvent)
    };
}