#pragma once

#include "IEvent.hpp"

namespace Core::Events
{
    class WindowResizeEvent : public IEvent
    {

    public:
        unsigned int Width;
        unsigned int Height;


        WindowResizeEvent(unsigned int x, unsigned int y)
        : Width(x), Height(y) {}
    };
}