#pragma once

#include "API/Layer.hpp"

namespace Game::Layers
{
    class RenderLayer : public ILayer
    {
    public:
        RenderLayer() = default;

        void OnUpdate(float dt) override {};
        void OnRender() override;

        void OnAttach() override {};
        void OnDetach() override {};

    };
}