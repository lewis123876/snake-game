# Candy Snake Game
A candy-themed Snake game built in **C++ using Raylib**, featuring jelly-style visuals, bouncing candy food, sound effects, and a soft pastel UI. This project is a modern take on the classic Snake game with a playful, sugary aesthetic.

## Features

- Random food (gummies, candy, croissants, strawberries)
- Bouncing and scaling food animation
- Sound effects for eating and collisions
- Game Over overlay with restart prompt
- Score tracking

## Controls

| Key   | Action       |
| ----- | ------------ |
| ↑     | Move up      |
| ↓     | Move down    |
| ←     | Move left    |
| →     | Move right   |
| ENTER | Restart game |
| ESC   | Quit         |

## How to Run

### Requirements

- C++ compiler (clang or g++)
- [Raylib](https://www.raylib.com/) installed (already in the include folder so no need to install)

### Build & Run (example)

clang++ main.cpp -L lib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a -o compiled-raylib-game
