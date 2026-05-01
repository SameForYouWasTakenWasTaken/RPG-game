#pragma once

#include "Engine/API/Vertex.hpp"
#include "Engine/Components/Geometry.hpp"
#include "Engine/Components/Transform.hpp"
#include "Global/Types.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <cstdint>

namespace Core::Rendering
{

    struct RenderObject
    {
        Global::Types::TextureHandle TextureHandle{};
        const sf::Texture* Texture;

        const Core::Components::Transform* Transform;
        const Core::Components::Geometry* Geometry;

        uint8_t zIndex = 0;

        RenderObject(
            Global::Types::TextureHandle TextureHandle, const sf::Texture* Texture, Core::Components::Transform* Transform, const Core::Components::Geometry* geometry
        ) : TextureHandle(TextureHandle), Texture(Texture), Transform(Transform), Geometry(geometry) {}
    };

    struct InstanceData
    {
        sf::Transform transform;
    };
    
    struct Batch
    {
        Global::Types::zIndexType zIndex = 0;
        Components::Geometry Geometry;
        const sf::Texture* Texture;
    };

}