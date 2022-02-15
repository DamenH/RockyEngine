# Rocky Engine

**Purpose:** A minimalistic cross-platform 3D game engine suitable for rapid prototyping of games with modern graphics, physics, networking, and Data-Oriented-Design

## Project Setup

1. Update submodules

```
git submodule update --init --recursive
```

2. Install dependencies

```
./vcpkg/vcpkg install
```

## Dependencies:

| Library | Version | Source | Function |
|-------- | ------- |------- |--------- |
| Ogre    | 1.12.9  | vcpkg  | 3D graphics |
| SDL2 | 2.0.16  | vcpkg  | Windowing, keyboard, and mouse |
| EnTT    | 3.8.0  | vcpkg  | Entity Component System implementation |
| Bullet3    | 3.17  | vcpkg  | 3D realtime physics simulation |
| Boost    | 1.77.0  | vcpkg  | Handy C++ libraries |
| ImGui    | 1.85  | vcpkg  | Immediate mode GUI |
| OpenEXR    | 2.5.0 | vcpkg  | HDR image loading |
| pugixml    | 1.114 | vcpkg  | XML parser |


## Planned Features

- [ ] PBR graphics
- [ ] Atmospheric effects
- [ ] Dynamic map loading for large open worlds
- [ ] Deterministic physics
- [ ] Intelligent NPCs
- [ ] Efficient networking for multiplayer


