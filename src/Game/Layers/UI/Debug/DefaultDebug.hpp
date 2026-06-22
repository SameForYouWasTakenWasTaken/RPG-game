#pragma once
#include "API/Layer.hpp"
#include "Console/ConsoleUI.hpp"

namespace Game::Layers
{
    class DefaultDebug : public ILayer
    {
        UI::ConsoleUI m_ConsoleUI;
    public:
        void OnAttach() override;
        void OnDetach() override;

        void OnUpdate(float dt) override;
        void OnRender() override;
    };
}
