#!/usr/bin/env python3
"""Découpe une rangée de la planche d'armes en XPM, un fichier par image.

La planche est une grille de cases de 64 x 64 : séparateurs turquoise d'un
pixel entre les colonnes, bandes de 16 pixels entre les rangées. Le violet
du fond devient transparent ("None").

    tools/sheet_to_xpm.py textures/weapon/wolf3d_weapons.png 1 textures/weapon/pistol

Rangées : 0 couteau, 1 pistolet, 2 mitraillette, 3 gatling (puis les mêmes
avec une autre manche). Écrit <prefix>_0.xpm à <prefix>_4.xpm.
"""
import string
import sys

from PIL import Image

CELL = 64
FRAMES = 5
BACKGROUND = (152, 0, 136)
CHARS = string.ascii_letters + string.digits + "#$%&*+-/:;<=>?@^_~|"


def cell(sheet, row, frame):
    x = 1 + frame * (CELL + 1)
    y = 16 + row * (CELL + 16)
    return sheet.crop((x, y, x + CELL, y + CELL))


def write_xpm(img, path, name):
    px = img.load()
    colors = sorted({px[x, y] for y in range(CELL) for x in range(CELL)})
    cpp = 1 if len(colors) <= len(CHARS) else 2
    codes = {}
    for i, c in enumerate(colors):
        codes[c] = CHARS[i % len(CHARS)] + (CHARS[i // len(CHARS)] if cpp == 2 else "")
    lines = ["/* XPM */", "static char *%s[] = {" % name,
             '"%d %d %d %d",' % (CELL, CELL, len(colors), cpp)]
    for c in colors:
        value = "None" if c == BACKGROUND else "#%02X%02X%02X" % c
        lines.append('"%s c %s",' % (codes[c], value))
    for y in range(CELL):
        row = "".join(codes[px[x, y]] for x in range(CELL))
        lines.append('"%s"%s' % (row, "," if y < CELL - 1 else "};"))
    with open(path, "w") as f:
        f.write("\n".join(lines) + "\n")


def main():
    if len(sys.argv) != 4:
        sys.exit(__doc__)
    sheet = Image.open(sys.argv[1]).convert("RGB")
    row, prefix = int(sys.argv[2]), sys.argv[3]
    for frame in range(FRAMES):
        path = "%s_%d.xpm" % (prefix, frame)
        write_xpm(cell(sheet, row, frame), path, path.rsplit("/", 1)[-1][:-4])
        print(path)


if __name__ == "__main__":
    main()
