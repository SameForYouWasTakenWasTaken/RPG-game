#pragma once

#include "Global/Types.hpp"

namespace Game::Components
{
    struct Sprite
    {
        Global::Types::TextureHandle TextureHandle{};
        Global::Types::zIndexType zIndex = 0;
    };
}