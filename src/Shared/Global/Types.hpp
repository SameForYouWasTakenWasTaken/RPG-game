#pragma once
#include <cstdint>

namespace Global::Types
{
    using TextureID = uint32_t;
    struct TextureHandle
    {
        TextureID id = 0;

        bool IsValid() const { return id != 0; }
    };
}