# Architecture

## WARNING
Because I am a lazy goof and I am working on this on my own,
there might be times where I forget to update this documentation, be aware of possible misinformation!

## Entity Component System

The game uses [EnTT](https://github.com/skypjack/entt) for its ECS (Entity Component System) architecture. This separates data (components) from logic (systems) for cleaner, more maintainable code.

## Scene Structure

```
Scene
├── registry (EnTT registry - holds all entities and components)
├── eventBus (Event communication between systems)
├── Systems (AI, Combat, Movement, Pickup, Progression, Reward, Hierarchy, Inventory) (PRONE TO CHANGE TO A MORE MANUAL APPROACH INSTEAD OF KEEPING EVERY SYSTEM IN)
└── Layers
    └── MainLayer (Gameplay input handling and entity spawning)
```

### Scene Lifecycle

| Method | Purpose |
|--------|---------|
| `OnUpdate(dt)` | Per-frame game logic |
| `OnFixed(step)` | Fixed timestep updates (physics) |
| `OnRender()` | Draw entities with sprites |
| `OnAttach()` | Layer added to scene |
| `OnDetach()` | Layer removed from scene |

## Components

Components are pure data attached to entities.

| Component | Purpose |
|-----------|---------|
| `Transform` | Position, rotation, scale |
| `Geometry` | Size and collision bounds |
| `Sprite` | Texture reference and render order |
| `Humanoid` | Health, speed, regeneration stats |
| `Weapon` | Damage, range, attack timing |
| `CombatStats` | Strength, Dexterity, Vitality |
| `Progression` | Level, XP, Gold |
| `Inventory` | Item storage (capacity-based) |
| `Item` | Name, stack count |
| `Pickable` | Can be picked up, pickup radius |
| `Hierarchy` | Parent-child entity relationships |

## Systems

Systems contain the logic and iterate over entities with specific components.

### Core Systems

| System | Components Query | Purpose |
|--------|-----------------|---------|
| `Combat` | Weapon, Humanoid | Attack timing and hit detection |
| `AISystem` | Enemy, Humanoid, Transform | Detection, chase behavior |
| `Movement` | Transform | Linear movement to targets |
| `Pickup` | Pickable | Item collection |
| `Inventory` | Inventory | Item management |
| `Progression` | Progression, Humanoid | XP and leveling |
| `Reward` | Reward | Quest/task rewards |

## Entities

Entities are created via factory functions and come pre-configured with components.

| Entity | Components | Description |
|--------|------------|-------------|
| `Player` | Transform, Humanoid, Weapon, Inventory, Progression, Sprite, PlayerTag | Player character |
| `Enemy` | Transform, Humanoid, Weapon, Enemy, Sprite, EnemyTag | AI-controlled enemy |
| `Coin` | Transform, Geometry, Sprite, Item, Pickable | Collectible currency |

## Event System

The game uses a decoupled event bus for communication:

| Event | Data | Purpose |
|-------|------|---------|
| `KeyPressedEvent` | Key code | Player input |
| `MouseClickEvent` | Button, position | Mouse input |
| `Attacked` | Attacker, Attackee, Damage | Combat hits |
| `Died` | Entity | Entity death |
| `LevelUp` | Entity, NewLevel | Level progression |
| `WindowResizeEvent` | Width, Height | Window resize |

## Controllers

Controllers handle player input and connect to the event bus:

- `IController` - Base class with registry access and `OnUpdate`
- `PlayerMovementController` - WASD movement, camera centering

## Rendering Pipeline

1. `MainLayer::OnRender()` iterates entities with Sprite, Transform, Geometry
2. Textures resolved from handle via `ResourceManager`
3. `RenderObject` submitted to `Renderer`
4. `Renderer::End()` draws all queued objects sorted by z-index

## Resource Management

Textures are loaded once via `ResourceManager::LoadTexture()` and referenced by handle. This avoids redundant loading and enables texture sharing across entities.