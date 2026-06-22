#include <fstream>
#include <stdexcept>
#include "TileMap.hpp"
#include "Entities/Default.hpp"
#include "Global/Helpers.hpp"
#include "Global/Types.hpp"
#include "Systems/ResourceManager.hpp"
#include "Engine/Components/Transform.hpp"
#include "Engine/Components/Geometry.hpp"
#include "Components/Sprite.hpp"
#include "TileSet.hpp"
#include "tinyxml2.h"

using NJson = nlohmann::json;

namespace Game::API
{

    void TileMap::Load(const JSONPath& mapPath)
    {
        TileSets.clear();
        Layers.clear();

        Loaded = false;

        std::ifstream file(mapPath);
        if (!file.is_open())
        {
            Loaded = false;
            return;
        }

        NJson json;
        file >> json;

        Width = json.at("width").get<int>();
        Height = json.at("height").get<int>();
        TileSize = json.at("tilewidth").get<int>();

        if (json.at("compressionlevel") != -1)
        {
            throw std::runtime_error("Compression level not supported!");
        }

        std::filesystem::path base = std::filesystem::path(mapPath).parent_path();
        for (auto& tileset : json.at("tilesets"))
        {
            int firstGid = tileset.at("firstgid").get<int>();
            std::string source = tileset.at("source").get<std::string>();
            
            std::filesystem::path tsxPath = base/source;
            
            TileSet set;
            set.FirstGid = firstGid;
            if (!set.LoadXMLFromTSX(tsxPath))
            {
                throw std::runtime_error("Failed to load TSX");
            }

            TileSets.push_back(std::move(set));
        }

        for (auto& layer : json.at("layers"))
        {

            if (layer.at("type").get<std::string>() != "tilelayer")
                continue;

            TileLayer outLayer;
            
            outLayer.height = layer.at("height").get<int>(); 
            outLayer.width = layer.at("width").get<int>(); 
            outLayer.data = layer.at("data").get<TileLayer::Tiles>();
            outLayer.visible = layer.at("visible").get<bool>();

            Layers.push_back(std::move(outLayer));
        }

        std::sort(TileSets.begin(), TileSets.end(), [](const auto& setA, const auto& setB){
            return setA.FirstGid < setB.FirstGid;
        });
        
        Loaded = !Layers.empty() && !TileSets.empty();
        
        if (Loaded)
            JSONMapDirectory = mapPath;
    }

    const TileSet* TileMap::FindNearestTileset(int gid)
    {
        const TileSet* tileSet = nullptr;
        for (auto& set : TileSets)
        {
            if (gid >= set.FirstGid)
                tileSet = &set;
        }

        return tileSet;
    }

    void TileMap::AppendTileGeometry(
        std::vector<sf::Vertex>& vertices,
        float worldX,
        float worldY,
        float size,
        int texX,
        int texY,
        int tileW,
        int tileH)
    {
        size_t start = vertices.size();
        vertices.resize(start + 6);

        sf::Vertex* v = &vertices[start];

        // Triangle 1
        v[0].position = {worldX, worldY};
        v[1].position = {worldX + size, worldY};
        v[2].position = {worldX + size, worldY + size};

        // Triangle 2
        v[3].position = {worldX, worldY};
        v[4].position = {worldX + size, worldY + size};
        v[5].position = {worldX, worldY + size};

        // UVs
        v[0].texCoords = {(float)texX, (float)texY};
        v[1].texCoords = {(float)(texX + tileW), (float)texY};
        v[2].texCoords = {(float)(texX + tileW), (float)(texY + tileH)};

        v[3].texCoords = {(float)texX, (float)texY};
        v[4].texCoords = {(float)(texX + tileW), (float)(texY + tileH)};
        v[5].texCoords = {(float)texX, (float)(texY + tileH)};
    }

    void TileMap::CreateTileEntities(entt::registry& registry)
    {
        int layerCount = 0;

        for (const auto& layer : Layers)
        {
            if (!layer.visible)
                continue;

            // One vertex array per texture
            std::unordered_map<Global::Types::TextureID, std::vector<sf::Vertex>> batches;

            for (size_t i = 0; i < layer.data.size(); i++)
            {
                int gid = layer.data[i];

                if (gid == 0)
                    continue;

                const TileSet* tileset = FindNearestTileset(gid);
                
                assert(tileset->TextureHandle.IsValid());
                if (!tileset)
                    continue;

                int localID = gid - tileset->FirstGid;

                int tileX = i % layer.width;
                int tileY = i / layer.width;

                int texX = (localID % tileset->Columns) * tileset->Width;
                int texY = (localID / tileset->Columns) * tileset->Height;

                auto& vertices = batches[tileset->TextureHandle.id];

                AppendTileGeometry(
                    vertices,
                    tileX * TileSize,
                    tileY * TileSize,
                    TileSize,
                    texX,
                    texY,
                    tileset->Width,
                    tileset->Height
                );
            }

            // Create one entity per batch
            for (auto& [textureID, vertices] : batches)
            {
                auto entity = Entities::CreateEntity(registry);

                registry.emplace<Core::Components::Transform>(entity);

                registry.emplace<Core::Components::Geometry>(
                    entity,
                    Core::Components::Geometry(std::move(vertices))
                );

                auto& sprite =
                    registry.emplace<Game::Components::Sprite>(entity);

                sprite.TextureHandle = Global::Types::TextureHandle{textureID, Global::Types::TextureStatus::Valid};
                sprite.zIndex = layerCount;
            }

            layerCount++;
        }
    }
}