# Pac-Man

C implementation of Pac-Man using SDL2, with collision detection, ghost AI, random maze generation, and file-based map loading.

> Systems Programming project completed during a course at  
> [ENSEIRB-MATMECA](https://enseirb-matmeca.bordeaux-inp.fr/), Bordeaux INP —  
> Telecommunications department, 2025/2026.

## Features

- Smooth Pac-Man movement with buffered direction input
- Collision detection against maze walls (tile-based)
- Four ghosts with directional AI (chase / flee depending on vulnerability state)
- Pac-gum system — ghosts become vulnerable after every 30 pac-gums eaten
- Random maze generation via iterative DFS with backtracking
- File-based map loading (custom `.txt` format)
- Oriented Pac-Man and ghost sprites (up / down / left / right)
- Pause mode with mouse and Pac-Man coordinate overlay

## Files

| File | Description |
|------|-------------|
| `main.c` | Game loop, SDL event handling |
| `algo.c / algo.h` | Game state, collision detection, ghost & Pac-Man logic |
| `affichage.c / affichage.h` | SDL rendering (map, sprites, HUD) |
| `generate_lab.c` | Random maze generation (iterative DFS + stack) |
| `Makefile` | Build system |
| `maps/` | Example map files |
| `images/` | Sprite assets (PNG) |

## Usage

```bash
make
./pacman              # default built-in map
./pacman map.txt      # custom map file
```

### Controls

| Key | Action |
|-----|--------|
| ↑ ↓ ← → | Move Pac-Man |
| `P` | Pause / unpause |
| `R` | Restart (after game over) |
| `Q` | Quit |

## Map format

Maps are plain text files where each character represents a tile:

```
WWWWWWWWWWWWWWWWWWWWWWWWW
WS                       W
W  WW WWWWWWWWWWWWWWWW   W
W     G    G    G    G   W
WWWWWWWWWWWWWWWWWWWWWWWWW
```

| Character | Tile |
|-----------|------|
| `W` | Wall |
| ` ` | Path |
| `S` | Pac-Man start |
| `G` | Ghost start |

## Random maze generation

Mazes are generated with an **iterative DFS** (depth-first search with backtracking):

1. Mark the starting cell as visited and push it onto the stack
2. While the stack is not empty:
   - Pop the current cell
   - If it has unvisited neighbours: push it back, pick one at random, remove the wall between them, mark the neighbour visited and push it

A maze of `cells_w × cells_h` cells produces a `(2·cells_w+1) × (2·cells_h+1)` tile map.

```c
srand(time(NULL));
create_map_txt("map.txt", 13, 14);  // → 27×29 tile map
```

## Dependencies

- [SDL2](https://www.libsdl.org/)
- SDL2_image
- SDL2_ttf

```bash
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

## Reference

Pac-Man original game — Namco, 1980.  
Maze generation — [Maze generation algorithm, Wikipedia](https://en.wikipedia.org/wiki/Maze_generation_algorithm).
