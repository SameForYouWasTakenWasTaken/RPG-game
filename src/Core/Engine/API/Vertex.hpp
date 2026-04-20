#pragma once

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/System/Vector2.hpp"
#include "glm/ext/vector_float2.hpp"
namespace Core::API
{
    struct Vertex
    {
        glm::vec2 Position{};
        sf::Color Color = sf::Color::White;
        glm::vec2 TexCoord{};

        inline sf::Vertex ToSFMLVertex() const
        {
            return sf::Vertex(
                sf::Vector2f{Position.x, Position.y}, 
                Color, 
                sf::Vector2f{TexCoord.x, TexCoord.y});
        }
    };
}