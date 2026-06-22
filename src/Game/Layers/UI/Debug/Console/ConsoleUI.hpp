#pragma once
#include "Console.hpp"
#include "API/Layer.hpp"

namespace Game::UI
{
    class ConsoleUI : public Layers::ILayer
    {
    public:
        ConsoleUI() = default;

        void OnAttach() override {}
        void OnDetach() override {}

        void OnUpdate(float dt) override;
        void OnRender() override;
    };
}
