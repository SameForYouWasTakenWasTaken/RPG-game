#pragma once
#include "Console.hpp"
#include "API/Layer.hpp"

namespace Game::UI
{
    class ConsoleUI : public Layers::ILayer
    {
        char m_InputBuffer[256] = "";
    public:
        ConsoleUI() = default;

        void OnAttach() override {}
        void OnDetach() override {}

        void OnUpdate(float dt) override;
        void OnRender() override;
    };
}
