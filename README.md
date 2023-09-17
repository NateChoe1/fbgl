# fbgl - The framebuffer graphics library
A graphics library that interfaces with the [Linux framebuffer](https://en.wikipedia.org/wiki/Linux_framebuffer).

The entire library revolves around a single function, `setPoint(Framebuffer fb, int x, int y, Color color)`,
which is used to draw lines, rectangles, circles, and polygons.

This project was really just an excuse to mess with the framebuffer and to discover the 2d graphics formulas, no
reasonable program should exclusively target the Linux framebuffer.

I'm not going to put documentation here, you can read `fbgl.h` for that.
