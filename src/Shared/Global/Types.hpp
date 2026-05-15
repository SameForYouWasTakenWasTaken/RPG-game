#pragma once
#include <cstdint>

namespace Global::Types
{
    using TextureID = uint32_t;
    using zIndexType = uint8_t;

    enum class TextureStatus
    {
        Null = 0,
        Valid,
        
        NoFile, // Serves as a way of saying the file does not exist or is corrupted
    };
    struct TextureHandle
    {
        TextureID id = 0;
        TextureStatus status = TextureStatus::Null;

        bool IsValid() const { return id != 0 && status == TextureStatus::Valid; }
    };
}