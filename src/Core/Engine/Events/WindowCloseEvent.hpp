#pragma once

#include "IEvent.hpp"

namespace Core::Events
{
    class WindowCloseEvent : public IEvent
    {
    public:
        WindowCloseEvent() = default;
        EVENT_TYPE(WindowCloseEvent);
    };
}