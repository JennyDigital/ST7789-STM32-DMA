#!/usr/bin/env python3

from PIL import Image
import sys
import os

if len(sys.argv) != 3:
    print("Usage: {} <image-file> <varname>".format(sys.argv[0]))
    sys.exit(1)

fname = sys.argv[1]

img = Image.open(fname)
outfile = open( fr"{sys.argv[2]}.h", "w")

outfile.write( f"#define {sys.argv[2]}_X  {img.width}\n");
outfile.write( f"#define {sys.argv[2]}_Y  {img.height}\n\n");


outfile.write( f"const uint16_t {sys.argv[2]}[][{img.width}] = {{\n" );

for y in range(0, img.height):
    s = "  {\n    "
    for x in range(0, img.width):
        (r, g, b) = img.getpixel( (x, y) )
        color565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3)

        # for right endiness, so ST7735_DrawImage would work

        color565 = ((color565 & 0xFF00) >> 8) | ((color565 & 0xFF) << 8)
        s += "0x{:04X}".format(color565)
        if x != img.width-1:
            s += ", ";
        else:
            s += " ";

        if (x % 8) == 7:
            if x == img.width-1:
                s += "\n  ";
            else:
                s += "\n    ";

    if y != img.height-1:
        s += "}, \n";
    else:
        s += "} ";
    outfile.write( s );

# Closing brace for variable.
outfile.write( "\n};" );
outfile.close();
