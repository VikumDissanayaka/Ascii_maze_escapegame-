# Ascii_maze_escapegame-
# ASCII Maze Escape Game (C)

A terminal-based ASCII maze game written in C, where the player navigates a grid,
avoids line-of-sight detection from patrolling enemies, and can undo moves via a
custom undo stack — all rendered live in the terminal using raw input mode.

## Overview

The player (`P`) moves through a map loaded from a text file, trying to reach the
goal (`G`) while avoiding enemies (shown as `^ > v <` depending on facing direction).
Enemies rotate each turn and will chase the player down a corridor if they come into
their line of sight. Walls/obstacles (`O`, `*`) block both movement and enemy vision.

## Features

- **Map loading from file** — grid layout, player start, goal, obstacles, and enemies
  are all defined in a simple text map format (see `sample_map.txt`)
- **Line-of-sight enemy detection** — each enemy checks whether the player is directly
  in its facing direction with no obstacles in between
- **Enemy chase behaviour** — once an enemy spots the player, it moves toward them
  step by step until it catches them or the player escapes
- **Undo system** — a generic linked-list-based undo stack stores deep copies of the
  map state, letting the player undo their last move (`u`) at any time
- **Raw terminal input** — the terminal is switched to raw mode (via `termios`) so
  single keypresses are read immediately without waiting for Enter

## Controls

| Key | Action |
|-----|--------|
| `w` / `a` / `s` / `d` | Move up / left / down / right |
| `u` | Undo last move |

## Files

- `main.c` — program entry point, game loop, terminal setup/teardown
- `map.c` / `map.h` — map loading, grid rendering, deep copy, and memory management
- `game.c` / `game.h` — player movement, enemy rotation, line-of-sight detection, and
  chase logic
- `terminal.c` / `terminal.h` — raw terminal mode handling and single-character input
- `undo.c` / `undo.h` — generic linked-list undo stack (stores deep-copied game states)
- `sample_map.txt` — example map file
- `Makefile` — builds the `escape` executable

## Build & Run

```
make
./escape sample_map.txt
```

## Map file format

The first line gives the grid size (`rows cols`). Each following row is a line of
space-separated integers:

| Value | Meaning |
|-------|---------|
| `0` | Empty floor |
| `1` | Player start |
| `2` | Goal |
| `3` | Obstacle (blocks movement and line of sight) |
| `4` | Enemy (random starting direction) |

## Skills demonstrated

C programming (C89), dynamic memory management, 2D grid manipulation, generic data
structures via function pointers (the undo stack works with any copy/free function),
terminal I/O with `termios`, and Makefile-based builds.
