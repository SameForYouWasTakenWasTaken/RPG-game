#pragma once
#include "Global/Types.hpp"
#include "Engine/API/Rect.hpp"
#include "tinyxml2.h"
#include "Systems/ResourceManager.hpp"

namespace Game::API
{
    struct TileSet
    {
        using TSXPath = std::filesystem::path;
        
        int FirstGid = 0;
        int Width = 0;
        int Height = 0;
        int Columns = 0;
        Global::Types::TextureHandle TextureHandle{};

        Core::API::Rect<int> GetTileRect(int localID) const;
        int LocalID(int gid) {return gid - FirstGid;}
        bool LoadXMLFromTSX(const TSXPath& path);
    };
}