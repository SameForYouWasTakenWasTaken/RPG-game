#pragma once
#include <filesystem>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "Engine/Components/Geometry.hpp"
#include "SFML/Graphics/VertexBuffer.hpp"
#include "TileSet.hpp"
#include "TileLayer.hpp"


namespace Game::API
{
    class TileMap
    {
    public:
        using JSONPath = std::filesystem::path;
        sf::VertexBuffer VBuffer;
        
        int Width = 0;
        int Height = 0;
        int TileSize = 0;
        JSONPath JSONMapDirectory;

        std::vector<TileLayer> Layers;
        std::vector<TileSet> TileSets;

        bool Loaded = false;
        void Load(const JSONPath& map);
        const TileSet* FindNearestTileset(int gid);
        void CreateTileEntities(entt::registry& registry);
        void AppendTileGeometry(
            std::vector<sf::Vertex>& vertices,
            float worldX,
            float worldY,
            float size,
            int texX,
            int texY,
            int tileW,
            int tileH);
    };
}