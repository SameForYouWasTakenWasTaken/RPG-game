#pragma once
#include "Window/Window.hpp"
#include "vendor/entt/entt.hpp"

namespace Core
{
    struct EngineContext
    {
        Rendering::Window* ActiveWindow;
    };
}