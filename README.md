# cub3D

A raycasting engine in C, written for the 42 school project of the same name. It reads a plain
text scene file and turns it into a first person view you can walk around with the keyboard.

The technique is the one Wolfenstein 3D used back in 1992: cast one ray per screen column, stop at
the first wall, and use that distance to size the wall stripe. No 3D library, no mesh, just maths
over a grid of characters.

Built at 42 Berlin in 2025 by Damien Rongier ([@drongier](https://github.com/drongier)) and
Mertcan Kunduraci ([@mekundur](https://github.com/mekundur)).

## How the rendering works

The map is a grid of 64 pixel blocks, stored once at load time as a flat array of cells. Every
frame the engine builds a camera from the player position, the view direction and a camera plane
perpendicular to it, sized for a 66 degree field of view. Each of the 1280 columns gets a ray
through its point on that plane, and the ray walks the grid line by line (DDA) until it lands on a
wall. The DDA gives the distance to the camera plane directly, so there is no fisheye correction and
no trigonometry per column, and straight walls stay straight up to the screen edges. The wall
height uses the focal length that matches the field of view, so a block looks like a cube. The hit
also tells which face was struck, to pick the north, south, west or east texture, and where on the
face, so the texture is read left to right on every face. Floor and ceiling are flat colors read
from the scene file.

Each frame starts by filling the top half with the ceiling color and the bottom half with the floor
color, one whole row at a time, then draws the wall columns over it. Textures are transposed at
load time so that walking down a wall column reads contiguous memory, and the texel index moves
down with an integer step and remainder instead of a division per pixel.

Each frame is drawn into a framebuffer in memory and pushed to the window through SDL3 in one
call, at 1280x720.

## Build

You need a C compiler, make, pkg-config and SDL3.

```sh
brew install sdl3 pkg-config          # macOS
sudo apt install libsdl3-dev          # recent Debian/Ubuntu (25.04+), Fedora and Arch have it too
make
```

On older distributions (Ubuntu 22.04/24.04), build SDL3 from source once:

```sh
git clone --depth 1 --branch release-3.4.12 https://github.com/libsdl-org/SDL.git
cmake -S SDL -B SDL/build -DCMAKE_BUILD_TYPE=Release
cmake --build SDL/build -j
sudo cmake --install SDL/build
```

Targets: `make` builds an optimized `cub3D` (`-O2`, objects in `build/`), `make debug` builds
`cub3D_debug` with AddressSanitizer and UBSan (objects in `build-debug/`), `make test` runs the
unit tests, `make clean`, `make fclean` and `make re` do the usual. `make OPT=-O0` builds without
optimization.

## Run

```sh
./cub3D [--check] [--no-vsync] [--fps N] [--bench [N]] maps/good/cheese_maze.cub
```

One `.cub` scene file, options in any order. The window title shows the frame rate, the frame time
and the time spent rendering, refreshed twice a second.

- `--check` loads the scene and its textures, prints `OK` and exits without a window
- `--no-vsync` renders as fast as possible instead of following the screen refresh
- `--fps N` caps the frame rate at N (10 to 1000); with vsync on, the slower of the two wins
- `--bench [N]` plays N frames (1000 by default) with vsync off and no cap, turning on the spot
  with a fixed 1/60 s step, then prints the average, median, p99, min and max render and frame
  times and exits

| Key | Action |
|---|---|
| `W` / `S` | walk forward / backward |
| `A` / `D` | strafe left / right |
| `Left` / `Right` | turn |
| `Esc` | quit |
| window close button | quit |

Keys are read by physical position: on an AZERTY keyboard, walk with `Z` `Q` `S` `D`.

Movement follows the clock, not the frame count: the player walks about 2.8 blocks and turns
about 103 degrees per second at any frame rate, and walking diagonally is not faster. A frame that
took longer than 50 ms counts as 50 ms, so a stall never makes the player jump.

The player cannot walk through walls. Each axis is tested on its own before the move is applied, so
you slide along a wall instead of sticking to it.

## Scene file format

Six elements describe the scene, then a blank line, then the map grid.

```c
NO textures/mossy.xpm
SO textures/wood.xpm
WE textures/red_brick.xpm
EA textures/blue_stone.xpm

F 220,100,0
C 225,30,0

1111111
1000001
100N001
1000001
1111111
```

The six elements can come in any order, one per line, with any amount of spaces or tabs around
them. Each must appear exactly once.

`NO`, `SO`, `WE` and `EA` are the wall textures for the north, south, west and east faces: one
path, without spaces, ending in `.xpm`, to a file that exists and can be read.

`F` and `C` are the floor and ceiling colors: exactly three numbers from 0 to 255, separated by
commas. Spaces around the numbers are fine, `F 50 , 50 , 50` is accepted; `F 1,2`, `F 1,2,3,`,
`F 12x,4,5` or `F a,b,c` are not.

The grid comes last. `1` is a wall, `0` is an empty cell, a whitespace character counts as empty,
and one of `N`, `S`, `E`, `W` marks where the player starts and which way they look. Exactly one
spawn point. Rows can be shorter than the longest one, the parser pads them. The grid is limited to
1000 x 1000 cells and the file to 16 MB. Windows line endings (`\r\n`) are accepted.

## What the parser refuses

The parser is strict on purpose. It never exits on its own: it returns the error to the caller,
which prints `Error`, then the line of the file at fault when there is one, then the reason.

```
Error
line 5: color must be three numbers from 0 to 255, as R,G,B
```

- a file that is not a regular `.cub` file, cannot be read, or holds a NUL byte
- an unknown identifier (`N`, `NOO`, `hello`...), or an element given twice
- a texture path that is missing, is not a single word, does not end in `.xpm`, or cannot be
  loaded
- a malformed color
- a map that starts before all six elements are given, or anything but blank lines after it,
  including an empty line inside the grid
- a map character outside `0`, `1`, whitespace and `NSEW`
- no spawn point, or more than one
- a grid that is not sealed: everything the player can reach from the spawn without crossing a
  wall, diagonals included, must stay inside the grid. The check walks the grid with an explicit
  stack, so a 1000 x 1000 map is checked in a few milliseconds without recursion.

`maps/bad/` holds one file per rejected case. `./cub3D --check <scene.cub>` loads a scene, its
textures included, prints `OK` and exits without opening a window.

## Tests

`make test` runs the unit tests (XPM loader, command line, statistics, movement, polygon fill,
ray casting, wall columns, scene parsing).

`test.sh` runs the binary over the whole `maps/bad/` folder and expects every scene to be refused:
exit code 1 and an `Error` message, no crash, and the game must not still be running after 5
seconds. It then runs `--check` on every scene of `maps/good/` and expects `OK`. When valgrind is installed (Linux), it also checks for leaks and invalid accesses. It
ends with a `passed/total` line and a non-zero exit code on failure.

```sh
make && ./test.sh
```

On macOS, check leaks with `leaks --atExit -- ./cub3D --bench 200 maps/good/cheese_maze.cub`.
Benchmarks are tracked in `docs/perf/benchmarks.md`.

## Project layout

```
includes/             cub3d.h (structs, constants) and one header per module
sources/main.c        entry point
sources/loop.c        main loop, fps counter, benchmark
sources/options.c     command line
sources/stats.c       benchmark statistics
sources/motion.c      movement and rotation per second, frame time clamp
sources/raster.c      alpha blending, spans and polygon fill
sources/grid.c        flat map grid, void outside the map found by flood fill
sources/raycast.c     camera, DDA ray casting, wall height
sources/pixels.c      row fills, textured wall columns, column-major textures
sources/platform/     SDL3 window, input and clock; XPM loader
sources/level/        scene file parsing and validation (no exit, errors with line numbers)
sources/game.c        load and unload a scene: textures, grid, player
sources/drawing/      frame drawing, player update, minimap
libft/                our own libft, including ft_printf and get_next_line
tests/                unit tests
maps/good/            valid scenes, from small test maps to full mazes
maps/bad/             scenes that must be rejected, one per error case
textures/             xpm textures
test.sh               batch tester: every bad scene refused, every good one loads
docs/                 specs, plans and benchmarks
```

## Bonus features

Both are compiled in by default and can be turned off with `BONUS 0` in `includes/cub3d.h`.

- a 200x200 radar in the bottom right corner, centered on the player with north up, 12 pixels per
  block whatever the map size: translucent floor, light walls, darker void outside the map, a green
  arrow for the player and a translucent cone for the field of view, built from the wall hits of the
  3D view so it costs about 0.1 ms per frame
- a small crosshair at the center of the screen

## Known limits

- the field of view, the movement speed and the block size (64) are constants, and there is no mouse
  look or sprint
- floors and ceilings are solid colors, only the walls are textured
- no sprites and no doors, so nothing moves in the scene but the player
