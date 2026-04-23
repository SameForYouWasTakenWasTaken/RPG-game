#pragma once

namespace Core::Systems
{
    class ISystem
    {

    public:
        ISystem() = default;

        virtual void OnUpdate(float dt) {}
        virtual void OnFixed(float dt) {}
    };
}