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
        
        root->QueryIntAttribute("tilewidth", &Width);
        root->QueryIntAttribute("tileheight", &Height);
        // root->QueryIntAttribute("tilecount", &out.Count); // For batching
        root->QueryIntAttribute("columns", &Columns);
        
        auto* image = root->FirstChildElement("image");
        if (!image)
            return false;
        
        const char* relative_source = image->Attribute("source");
        auto fullSource = path/".."/relative_source; // source references the folder its in with the .tsx, so the extra ".." is needed
        TextureHandle = Systems::ResourceManager::LoadTexture(fullSource);
        
        if (!TextureHandle.IsValid())
            return false;

        return true;
    }
}