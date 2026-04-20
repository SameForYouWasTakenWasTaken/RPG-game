#pragma once

#include "IEvent.hpp"
#include "SFML/Window/Keyboard.hpp"

namespace Core::Events
{
    class KeyPressedEvent : public IEvent
    {
    public:
        sf::Keyboard::Key key;
        sf::Keyboard::Scan scan;

        KeyPressedEvent(sf::Keyboard::Key key, sf::Keyboard::Scan scan)
        : key(key), scan(scan) {}

        EVENT_TYPE(KeyPressedEvent);
    };

    class KeyReleasedEvent : public IEvent
    {
    public:
        sf::Keyboard::Key key;
        sf::Keyboard::Scan scan;

        KeyReleasedEvent(sf::Keyboard::Key key, sf::Keyboard::Scan scan)
        : key(key), scan(scan) {}

        EVENT_TYPE(KeyReleasedEvent);
    };
}