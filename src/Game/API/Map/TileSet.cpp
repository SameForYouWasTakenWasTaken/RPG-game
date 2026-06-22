#include "TileSet.hpp"

namespace Game::API
{
    Core::API::Rect<int> TileSet::GetTileRect(int localID) const
    {
        int x = (localID % Columns) * Width;
        int y = (localID / Columns) * Height;

        return { x, y, Width,Height };
    }

    bool TileSet::LoadXMLFromTSX(const TSXPath& path)
    {
        tinyxml2::XMLDocument doc;

        if (doc.LoadFile(path.string().c_str()) != tinyxml2::XML_SUCCESS)
            return false;

        auto* root = doc.FirstChildElement("tileset");
        if (!root)
            return false;

        if (root->QueryIntAttribute("tilewidth", &Width) != tinyxml2::XML_SUCCESS || Width <= 0)
            return false;
        if (root->QueryIntAttribute("tileheight", &Height) != tinyxml2::XML_SUCCESS || Height <= 0)
            return false;
        // root->QueryIntAttribute("tilecount", &out.Count); // For batching
        if (root->QueryIntAttribute("columns", &Columns) != tinyxml2::XML_SUCCESS || Columns <= 0)
            return false;
        
        auto* image = root->FirstChildElement("image");
        if (!image)
            return false;

        const char* relative_source = image->Attribute("source");
        if (!relative_source)
            return false;

        auto fullSource = std::filesystem::path(path).parent_path() / relative_source;
        TextureHandle = Systems::ResourceManager::LoadTexture(fullSource);
        
        if (!TextureHandle.IsValid())
            return false;

        return true;
    }
}