# Map Creation

> **⚠️ WARNING**  
> This documentation is maintained by a single developer and may occasionally become outdated. Please verify against the codebase when in doubt.

---

## Overview

Maps in this game are created using **[Tiled](https://www.mapeditor.org/)**, a free, open‑source 2D map editor.  
When working with Tiled you will produce three core asset types:

- **TiledProject (.tproj)** – Lightweight project file that saves your editor state.  
- **Tilemap (.tmj)** – The actual map layout, exported as a JSON file.  
- **Tileset (.tsx)** – An XML file describing a collection of tiles (usually a texture atlas).

---

## Architecture Details

### Tilemaps

- Exported as `.tmj` (Tiled Map JSON) files.
- Parsed at runtime with **nlohmann::json** to extract layout, layers, and referenced tilesets.
- Store them under `Resources/TileMaps/<MapName>/` for clarity.

### Tilesets

Tilesets require a bit more care because they involve XML parsing and path resolution.

#### Backend loading flow (simplified)

```cpp
void TileMap::Load(const JSONPath& mapPath)
{
    // Base directory of the .tmj file
    std::filesystem::path base = std::filesystem::path(mapPath).parent_path();

    for (auto& tileset : json.at("tilesets"))
    {
        std::string source = tileset.at("source").get<std::string>();
        std::filesystem::path tsxPath = base / source;   // e.g. ..\Shared\TileSets\player.tsx

        if (!TileSet::LoadXMLFromTSX(tsxPath))
            throw std::runtime_error("Failed to load TSX");
    }
}
```

```cpp
bool TileSet::LoadXMLFromTSX(const TSXPath& path)
{
    const char* relative_source = image->Attribute("source");   // e.g. ../Textures/player.png
    auto fullSource = path.parent_path() / relative_source;    // resolve relative to .tsx
    // … load the texture at fullSource …
}
```

#### Path resolution example
1. Your `.tmj` file lives at:  
   `src/Game/Resources/TileMaps/Testing/map01.tmj`
2. Inside that JSON you find:  
   `"source": "..\\..\\Shared\\TileSets\\player.tsx"`
3. Going up two folders from the `.tmj` lands you at `src/Game/Resources/`.  
   Append the relative path → `src/Game/Resources/Shared/TileSets/player.tsx`
4. The `.tsx` file contains:  
   `<image source="../Textures/player.png" … />`
5. From the `.tsx` location, go up one folder (`Shared/TileSets → Shared`) then into `Textures` →  
   `src/Game/Resources/Shared/Textures/player.png`

> **Tip:** For the people who don't know, think of each `..` in the JSON or XML as "go up one directory" from the current file's location.

#### Recommended folder layout

| Folder | Location (relative to `src/Game/Resources/`) | Purpose | Subfolders allowed? |
|--------|---------------------------------------------|---------|---------------------|
| **TileSets** | `Shared/Textures/` | Individual tileset collections (e.g. `grass.tsx`, `desert.tsx`) | ✅ |
| **TileMaps** | (root) | Exported `.tmj` map files | ✅ |
| **TiledProjects** | (root) | `.tproj` files for ongoing Tiled work | ✅ |
| **Global TileSets** | `Shared/` | Tilesets shared across many maps (e.g. `player.tsx`, `ui.tsx`) | ✅ (recommended) |

#### 📌 Critical Reminder
Always keep your **TiledProject** (`*.tproj`) inside `src/Game/Resources/` (or a subfolder thereof).  
If you open or save the project elsewhere, the relative paths stored in `.tmj` and `.tsx` files will become incorrect, causing runtime failures.  
Fixing this manually by editing paths is error‑prone—just work from the correct root folder.

---

## Quick Checklist for Adding a New Map

1. **Create a TiledProject**  
   `File → New → Project…` → save under `src/Game/Resources/TiledProjects/`

2. **Design your map**  
   - Add TileLayers, ObjectLayers, etc.  
   - Assign appropriate tilesets (see layout above).

3. **Export**  
   - **Map** → `File → Export As…` → choose **JSON Map File (.tmj)** → place in `src/Game/Resources/TileMaps/<YourMap>/`  
   - **Tileset** (if edited) → `File → Export As Tileset…` → save as `.tsx` under `src/Game/Resources/Shared/Textures/` (or `Shared/` for global sets).

4. **Verify paths**  
   Open the exported `.tmj` and ensure all `"source"` entries correctly point upward to your tileset locations.

5. **(OPTIONAL) Test in-game**
   Launch the game and confirm the map loads without errors. (Be sure it gets loaded, developer help required)

---

*Happy mapping!* 🎮
