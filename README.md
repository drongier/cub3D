# cub3D

A raycasting engine in C, written for the 42 school project of the same name. It reads a plain
text scene file and turns it into a first person view you can walk around with the keyboard.

The technique is the one Wolfenstein 3D used back in 1992: cast one ray per screen column, stop at
the first wall, and use that distance to size the wall stripe. No 3D library, no mesh, just maths
over a grid of characters.

Built at 42 Berlin in 2025 by Damien Rongier ([@drongier](https://github.com/drongier)) and
Mertcan Kunduraci ([@mekundur](https://github.com/mekundur)).

## How the rendering works

The map is a grid of 64 pixel blocks stored as a 2D char array. Every frame the engine walks the
1280 pixels of the window from left to right, and for each column it casts a ray inside a 60 degree
field of view. The ray advances cell by cell until it lands on a wall, which tells the engine how
far the wall is and which face it hit, so it knows whether to sample the north, south, west or east
texture. The xpm image is then sampled along the wall stripe with a column ratio, which keeps the
perspective right even with the nose against the wall. Floor and ceiling are flat colors read from
the scene file.

Each frame is drawn into an mlx image buffer and pushed to the window in one call, at 1280x720.

## Build

You need a C compiler, make, and the X11 development files.

```sh
sudo apt install build-essential libx11-dev libxext-dev
make
```

minilibx ships inside the repository (`minilibx-linux/`) and the Makefile builds it for you. The
link uses the X11 backend (`libmlx_Linux.a`), so GLFW is not required.

Useful targets: `make` builds `cub3D`, `make clean` drops the object files, `make fclean` also
removes the binaries and the libraries, `make re` rebuilds everything from scratch.

## Run

```sh
./cub3D maps/good/cheese_maze.cub
```

Exactly one argument, a `.cub` scene file. Anything else prints an error and exits. You need a
graphical session since the rendering goes straight to an X11 window.

| Key | Action |
|---|---|
| `W` / `S` | walk forward / backward |
| `A` / `D` | strafe left / right |
| `Left` / `Right` | turn |
| `Esc` | quit |
| window close button | quit |

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

`NO`, `SO`, `WE` and `EA` are the wall textures for the north, south, west and east faces. Paths
must point to an `.xpm` file that exists and can be read.

`F` and `C` are the floor and ceiling colors, three integers between 0 and 255 separated by commas.
Spaces around the numbers are fine, `F 50 , 50 , 50` is accepted.

The grid comes last. `1` is a wall, `0` is an empty cell, a whitespace character counts as empty,
and one of `N`, `S`, `E`, `W` marks where the player starts and which way they look. Exactly one
spawn point. Rows can be shorter than the longest one, the parser pads them.

## What the parser refuses

The parser is strict on purpose, and every failure prints a message that names the problem.

- a missing or duplicated element (`Multiple/missing NO texture path!`, `Multiple color entry!`)
- a texture path that does not exist, has no read rights, or is not an `.xpm`
  (`Texture files' format should be '.xpm' !`)
- malformed colors (`Wrong color format for ceiling!`, `Missing/redundant color channel for ceiling!`)
- a map character outside `0`, `1`, whitespace and `NSEW` (`Broken or polluted map!`)
- no spawn point, or more than one (`Multiple spawning points!`)
- a grid that is not sealed: the parser flood fills from the player, and the fill escaping the
  borders is an error (`Open map borders or player is borderline!`)
- anything other than blank lines before or after the grid (`There's mysterious stuff
  before/after the map!`)

`maps/bad/` holds 49 files, one per rejected case, so the parser can be regression tested by hand.

## Tests

`test.sh` runs the binary under valgrind over the whole `maps/bad/` batch plus a list of valid maps,
then greps the output for leaks, invalid reads and writes, and segfaults. It prints one line per map
and writes the raw valgrind report to `output.txt`, which it deletes at the end. Build first, then:

```sh
chmod +x test.sh
./test.sh
```

valgrind has to be installed (`sudo apt install valgrind`).

## Project layout

```
includes/cub3d.h      structs, key codes, window and grid constants
sources/main.c        entry point, mlx hooks, cleanup on exit
sources/init.c        game, player and texture setup
sources/parser/       scene parsing and validation
sources/drawing/      raycasting, walls, floor, ceiling, minimap
libft/                our own libft, including ft_printf and get_next_line
minilibx-linux/       vendored minilibx, X11 backend
maps/good/            valid scenes, from small test maps to full mazes
maps/bad/             49 scenes that must be rejected
maps/walls/           wall textures used by most scenes
textures/             more xpm textures
test.sh               valgrind batch tester
```

## Bonus features

Both are compiled in by default and can be turned off with `BONUS 0` in `includes/cub3d.h`.

- a minimap in the bottom right corner, scaled to fit the map, drawing the walls, the player and
  the fan of rays for the current field of view
- a small crosshair at the center of the screen

## Known limits

- the field of view, the movement speed and the block size (64) are constants, and there is no mouse
  look or sprint
- floors and ceilings are solid colors, only the walls are textured
- no sprites and no doors, so nothing moves in the scene but the player
