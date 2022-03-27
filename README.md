# Rocky Engine

**Purpose:** A custom cross-platform 3D game engine suitable for rapid prototyping of games with modern graphics, physics, networking, and Data-Oriented-Design

## Project Setup

- Clone repo and checkout desired branch

- `git submodule update --init`

- `cd build; cmake ..\CMakeLists.txt; make;`

## Dependencies:

| Library | Version | Source | Function                       |
|-------- | ------- |------- |------------------------------- |
| Raylib  | 4.0.0   |  git   | Graphics and interface functions, asset loading |
| GLFW    | 3.3.6   | Raylib | Windowing, keyboard, and mouse |
| EnTT    | 3.9.0   |  git   | Entity Component System        |
| ImGui   | 1.87    |  git   | Debug GUI                      |
| rlImGui | `9ed6344` |  git   | Integration of ImGui to RayLib |

## Compiler Support

- CLANG 13.0.0
- GCC 11.2.0

## Features

- Geometry LOD and instancing

## Planned Features

- PBR graphics
- Atmospheric effects
- 3D audio with environmental effects
- Dynamic map loading for large open worlds
- Deterministic physics
- Intelligent NPCs
- Efficient networking for multiplayer
- One-click setup for new game projects
- HTML+CSS-based UI system
- Live reloading of assets

## Goals

- [ ] Suitable for use in [**Ludum Dare 51 — September 30th - October 3rd, 2022**](https://ludumdare.com/#schedule)
- [ ] Suitable for public use by **March 31st, 2023**