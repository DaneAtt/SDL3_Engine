# Custom C++ Game Engine

A lightweight 2D game engine built from scratch in C++ using SDL3, designed with a fully component-based architecture.

In Active development, new systems and components added regularly

---

## Features

### Entity Component System (ECS)
The entire engine is built around an ECS architecture, enabling clean separation of data and logic across all game objects.

**Components:**
- `TransformComponent` — position, rotation, and scale for all entities
- `SpriteComponent` — handles rendering of 2D sprites
- `CollisionComponent` — broad phase collision detection
- `HitboxComponent` — precise hitbox-based collision
- `DamageComponent` — damage values and interaction logic
- `HealthComponent` — entity health tracking
- `AIComponent` — drives enemy and NPC behaviour
- `KeyboardController` — maps keyboard input to entity actions
- `TeamComponent` — team/faction assignment for entities
- `TileComponent` — tile-based map entity support
- `Vector2D` — core math utility for 2D vectors

### Asset Pipeline
A custom JSON-based asset pipeline that loads and manages all game assets at runtime.
- Animation definitions via JSON
- Static object configurations via JSON
- UI animation support
- Skill/ability data loading

### UI Framework
A fully custom UI system built on top of the engine.
- `UIManager` — manages all active UI elements
- `UIContainer` — groups and layouts UI elements
- `UIElement` / `Widget` — base building blocks
- `ScrollableHelper` — scrollable containers
- `GridHelper` — grid-based layouts

### Spatial Grid
A spatial partitioning system for efficient collision detection and entity querying across large game worlds.

### Event System
A decoupled `EventBus` messaging system that allows engine systems to communicate without direct dependencies.

### Pathfinding
A* pathfinding implementation integrated into the engine via `PathFinder`.

### Additional Systems
- `SoundManager` — audio playback via SDL3
- `TextureManager` — centralised texture loading and caching
- `LoadingManager` / `LoadingScreen` — asset preloading with loading screen support
- `WindowRender` — window and rendering management
- `Map` — tile/world map management
- `Animation` — sprite animation controller

---

## Dependencies

| Library | Purpose |
|---|---|
| SDL3 | Windowing, rendering, input |
| SDL3_image | Image/texture loading |
| SDL3_ttf | Font rendering |

---

## Platform

| Platform | Status |
|---|---|
| Windows | ✅ Supported |
| Linux | 🔲 Not tested |
| macOS | 🔲 Not tested |

---

## Project Structure

```
├── ECS/
│   ├── Components.h
│   ├── ECS
│   ├── Vector2D
│   ├── AIComponent
│   ├── CollisionComponent
│   ├── DamageComponent
│   ├── HealthComponent
│   ├── HitboxComponent
│   ├── KeyboardController
│   ├── SpriteComponent
│   ├── TeamComponent
│   ├── TileComponent
│   └── TransformComponent
├── JSON/
│   ├── AnimationJSON
│   ├── SkillJSON
│   ├── StaticObjectJSON
│   └── UIAnimationJSON
├── UI/
│   ├── GridHelper
│   ├── ScrollableHelper
│   ├── UIContainer
│   ├── UIElement
│   ├── UIManager
│   └── Widget
├── Animation
├── AssetManager
├── Engine
├── EventBus
├── Events
├── LoadingManager
├── LoadingScreen
├── Map
├── PathFinder
├── SoundManager
├── SpatialGrid
├── TextureManager
└── WindowRender
```
