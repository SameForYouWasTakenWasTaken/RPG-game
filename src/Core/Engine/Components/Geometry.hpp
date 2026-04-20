#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "Engine/API/Vertex.hpp"
#include "Engine/Components/Transform.hpp"
#include "Global/Helpers.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/PrimitiveType.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Graphics/VertexBuffer.hpp"

namespace Core::Components
{
    struct Geometry
    {
        std::vector<sf::Vertex> vertices{};
        sf::VertexArray vertexArray{};

        Geometry() = default;

        Geometry(const std::vector<API::Vertex>& apiVertices)
        {
            vertices.reserve(apiVertices.size());

            for (const auto& vertex : apiVertices)
                vertices.emplace_back(vertex.ToSFMLVertex());

            RebuildVertexArray();
        }

        Geometry(const std::vector<sf::Vertex>& sfVertices)
            : vertices(sfVertices)
        {
            RebuildVertexArray();
        }
        inline void RebuildVertexArray()
        {
            vertexArray.resize(vertices.size());
            vertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

            for (size_t i = 0; i < vertices.size(); ++i)
                vertexArray[i] = vertices[i];
        }

        Geometry& operator=(const Geometry& other)
        {
            if (this == &other)
                return *this;

            vertices = other.vertices;

            RebuildVertexArray();
            vertexArray.setPrimitiveType(other.vertexArray.getPrimitiveType());
            return *this;
        }
    };

    static Geometry CreateDefaultGeometry(
        sf::Texture& texture,
        sf::Color c = sf::Color::White)
    {
        Geometry geom;

        float x = 0;
        float y = 0;
        float w = 1;
        float h = 1;

        sf::Vector2f uvMin = {0.f, 0.f};
        sf::Vector2f uvMax = static_cast<sf::Vector2f>(texture.getSize());

        geom.vertices.reserve(6);
        // Triangle 1
        geom.vertices.emplace_back(sf::Vector2f(0, 0), c, uvMin);
        geom.vertices.emplace_back(sf::Vector2f(w, 0), c, sf::Vector2f{uvMax.x, uvMin.y});
        geom.vertices.emplace_back(sf::Vector2f(w, h), c, uvMax);

        // Triangle 2
        geom.vertices.emplace_back(sf::Vector2f(0, 0), c, uvMin);
        geom.vertices.emplace_back(sf::Vector2f(w, h), c, uvMax);
        geom.vertices.emplace_back(sf::Vector2f(0, h), c, sf::Vector2f{uvMin.x, uvMax.y});

        return geom;
    }
}