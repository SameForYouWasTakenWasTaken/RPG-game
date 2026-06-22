# RPG Game

A top-down 2D action RPG built with C++ and SFML. Battle enemies, collect loot, and progress through levels in this ECS-powered game.

> **⚠️ WARNING**  
> This documentation is maintained by a single developer and may occasionally become outdated. Please verify against the codebase when in doubt.

> Furthermore, this game is in **ALPHA** stages -- meaning his is a very early in-dev game
and not suitable for a stable release.

## Quick Start

### Prerequisites
- CMake 3.16+
- C++20 compatible compiler (MSVC, GCC, Clang)
- SFML 2.5+

### Build

```bash
git clone https://github.com/SameForYouWasTakenWasTaken/RPG-Game.git
cd RPG-Game
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Run

```bash
cd bin
./Game.exe
```

## Controls

| Key | Action |
|-----|--------|
| W | Move up |
| A | Move left |
| S | Move down |
| D | Move right |
| F | Spawn enemy at cursor |
| G | Drop coin at cursor |

## Gameplay

**Combat** - Enemies will chase and attack you when in range. Your weapon has a hit window that determines when damage is applied. Time your engagements wisely.

**Progression** - Defeat enemies to earn XP and gold. Leveling up increases your stats including max health.

**Loot** - Enemies drop coins that can be picked up by walking near them.

## Game Loop

1. **Poll Events** - Handle keyboard input and window events
2. **Update** - Run game logic (movement, AI, combat)
3. **Fixed Update** - Physics and network sync
4. **Render** - Draw all entities with sprites to screen

The game currently uses a layered architecture where the `MainLayer` handles all gameplay logic.

## Project Structure

```
src/
├── Core/Engine/        # Engine core (renderer, window, events, scheduler)
├── Game/
│   ├── API/            # Scene and Layer interfaces
│   ├── Components/     # ECS components
│   ├── Entities/       # Entity factories (Player, Enemy, Coin)
│   ├── Events/         # Game events (Attacked, Died, LevelUp)
│   ├── Layers/         # Scene layers (MainLayer)
│   └── Systems/        # Game systems (Combat, AI, Inventory, etc.)
└── Shared/vendor/      # Third-party libraries (EnTT, Tracy)
```

For detailed architecture documentation, see [/docs/architecture.md](docs/architecture.md).
