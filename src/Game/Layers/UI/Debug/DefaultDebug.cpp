#include "DefaultDebug.hpp"

namespace Game::Layers
{
    void DefaultDebug::OnAttach()
    {
    }

    void DefaultDebug::OnDetach()
    {
    }

    void DefaultDebug::OnUpdate(float dt)
    {
        m_ConsoleUI.OnUpdate(dt);
    }

    void DefaultDebug::OnRender()
    {
        m_ConsoleUI.OnRender();
    }
}
