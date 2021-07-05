/*
 * fbgl: A linux framebuffer graphics library
 * Copyright (C) 2021  Nate Choe
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Email me: natechoe9@gmail.com
 * */

#include <stdint.h>

typedef int32_t Color;
#define TRANSPARENT -1

typedef struct {
	Color *fb;
	//mmaped to /dev/fbX. Each value is of the form 0x00rrggbb with the
	//endianness of the system. The point at (x, y) is at index (y * width + x)
	int width;
	int height;
} Framebuffer;

Framebuffer createFramebuffer(char *fbpath);
//Creates a new Framebuffer using the specified framebuffer path (or /dev/fb0 if
//NULL)

#define rgb(r, g, b) ((r << 16) | (g << 8) | b)
void setPoint(Framebuffer fb, int x, int y, Color color);
void clearFramebuffer(Framebuffer fb, Color color);
void drawLine(Framebuffer fb, int x1, int y1, int x2, int y2, Color color);
void drawCircle(Framebuffer fb, int xc, int yc, int r, Color color);
//x center, y center, radius, color
void drawFilledCircle(Framebuffer fb,
		int xc, int yc, int r, Color border, Color fill);
//x center, y center, radius, border color, fill color
void drawPolygon(Framebuffer fb, int pointCount, int *points, Color color);

void drawRect(Framebuffer fb, int x1, int y1, int x2, int y2,
		int borderWidth, Color border);
void drawFilledRect(Framebuffer fb, int x1, int y1, int x2, int y2,
		int borderWidth, Color fill, Color border);

void noecho();
void yesecho();
//Just so that the console doesn't try to draw over your stuff.

Color getColor(Framebuffer fb, int x, int y);
//Gets color at coordinate, if the coordinate is invalid -1 is returned.
int saveFrame(Framebuffer fb, char *path);
//If this messes up, it'll return 1.
