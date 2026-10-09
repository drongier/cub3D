#!/usr/bin/env python3
"""Convertit une image (PNG...) en XPM ; les couleurs données deviennent "None".

    tools/png_to_xpm.py textures/enemies/mutant_sheet.png \\
        textures/enemies/mutant.xpm 63747D 7D929E

Les couleurs transparentes s'écrivent en hexadécimal RRGGBB. Il faut Pillow.
"""
import string
import sys

from PIL import Image

CHARS = string.ascii_letters + string.digits + "#$%&*+-/:;<=>?@^_~|"


def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    img = Image.open(sys.argv[1]).convert("RGB")
    clear = {tuple(int(h[i:i + 2], 16) for i in (0, 2, 4)) for h in sys.argv[3:]}
    w, h = img.size
    px = img.load()
    colors = sorted({px[x, y] for y in range(h) for x in range(w)})
    cpp = 1
    while len(CHARS) ** cpp < len(colors):
        cpp += 1
    codes = {}
    for i, c in enumerate(colors):
        code, n = "", i
        for _ in range(cpp):
            code += CHARS[n % len(CHARS)]
            n //= len(CHARS)
        codes[c] = code
    name = sys.argv[2].rsplit("/", 1)[-1].rsplit(".", 1)[0]
    lines = ["/* XPM */", "static char *%s[] = {" % name,
             '"%d %d %d %d",' % (w, h, len(colors), cpp)]
    for c in colors:
        value = "None" if c in clear else "#%02X%02X%02X" % c
        lines.append('"%s c %s",' % (codes[c], value))
    for y in range(h):
        row = "".join(codes[px[x, y]] for x in range(w))
        lines.append('"%s"%s' % (row, "," if y < h - 1 else "};"))
    with open(sys.argv[2], "w") as f:
        f.write("\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
