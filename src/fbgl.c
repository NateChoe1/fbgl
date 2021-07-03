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

#include "fbgl.h"

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#define between(low, value, high) ((low <= value) && (value <= high))

Framebuffer createFramebuffer(char *fbpath) {
	Framebuffer fb;

	if (fbpath == NULL)
		fbpath = "/dev/fb0";

	int fd = open(fbpath, O_RDWR);

	struct fb_var_screeninfo info;
	ioctl(fd, FBIOGET_VSCREENINFO, &info);

	fb.width = info.xres;
	fb.height = info.yres;

	fb.fb = mmap(
		NULL,
		fb.width * fb.height * info.bits_per_pixel / 8,
		PROT_READ | PROT_WRITE, MAP_SHARED,
		fd, 0
	);

	if (fb.fb == MAP_FAILED) {
		fprintf(stderr, "Error in mmap\n");
		exit(EXIT_FAILURE);
	}

	return fb;
}

void setPoint(Framebuffer fb, int x, int y, int color) {
	if (between(0, x, fb.width) && between(0, y, fb.height))
		fb.fb[y * fb.width + x] = color;
}

void clearFramebuffer(Framebuffer fb, int color) {
	for (int i = 0; i < fb.width * fb.height; i++)
		fb.fb[i] = color;
	//I could use setPoint, but this is much faster.
}

void drawLine(Framebuffer fb, int x1, int y1, int x2, int y2, int color) {
	if (x2 < x1) {
		int tmp;
		tmp = x1;
		x1 = x2;
		x2 = tmp;

		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}
	//make the line go from left to right

	int dx = x2 - x1;
	int dy = y2 - y1;

	char direction = (dy < 0) ? -1:1;

	int y = y1;
	int distance = 0;
	//vertical distance from the actual line scaled up dx
	for (int x = x1; x <= x2; x++) {
		setPoint(fb, x, y, color);

		while (distance > 0) {
			distance -= dx;
			/* Every time the pencil moves closer to the line, the vertical
			 * distance decreases by one, but this variable is scaled by dx. */

			y += direction;

			setPoint(fb, x, y, color);
		}

		distance += dy;
		/* Every time the "pencil" moves forward a pixel, the distance from the
		 * actual line increases by dy/dx, but to remove floating point math, we
		 * scale up distance by dx. */
	}

	for (; y != y2; y += direction)
		setPoint(fb, x2, y, color);
	//Sometimes the final column isn't done (think vertical lines)
}

void draw8Symmetry(Framebuffer fb, int xc, int yc, int x, int y, int color) {
		setPoint(fb, xc + x, yc + y, color);
		setPoint(fb, xc - x, yc + y, color);
		setPoint(fb, xc + x, yc - y, color);
		setPoint(fb, xc - x, yc - y, color);
		setPoint(fb, xc + y, yc + x, color);
		setPoint(fb, xc - y, yc + x, color);
		setPoint(fb, xc + y, yc - x, color);
		setPoint(fb, xc - y, yc - x, color);
}

void drawCircle(Framebuffer fb, int xc, int yc, int r, int color) {
	//beresenham's algorithm. I have no idea how this works, but it seems fast
	//so who cares.
	int d = 3 - (2 * r);
	int x = 0;
	int y = r;
	while (y >= x) {
		draw8Symmetry(fb, xc, yc, x, y, color);
		x++;

		if (d > 0) {
			y--;
			d = d + 4 * (x - y) + 10;
		} else
			d = d + 4 * x + 6;
	}
}

void drawFilledCircle(Framebuffer fb,
		int xc, int yc, int r, int border, int fill) {
	//Modified bresenham algorithm
	int d = 3 - (2 * r);
	int x = 0;
	int y = r;
	while (y >= x) {
		draw8Symmetry(fb, xc, yc, x, y, border);
		for (int i = y-1; i >= x; i--)
			draw8Symmetry(fb, xc, yc, x, i, fill);
		x++;

		if (d > 0) {
			y--;
			d = d + 4 * (x - y) + 10;
		} else
			d = d + 4 * x + 6;
	}
}

void drawPolygon(Framebuffer fb, int pointCount, int *points, int color) {
	if (pointCount == 0)
		return;
	int x1 = points[0];
	int y1 = points[1];
	for (int i = 1; i < pointCount; i++) {
		int x2 = points[i * 2];
		int y2 = points[i * 2 + 1];

		drawLine(fb, x1, y1, x2, y2, color);
		x1 = x2;
		y1 = y2;
	}

	drawLine(fb, points[0], points[1], x1, y1, color);
}

void noecho() {
	struct termios info;
	tcgetattr(fileno(stdin), &info);
	info.c_iflag &= ~ECHO;
	tcsetattr(fileno(stdin), 0, &info);
}

void yesecho() {
	struct termios info;
	tcgetattr(fileno(stdin), &info);
	info.c_iflag |= ECHO;
	tcsetattr(fileno(stdin), 0, &info);
}

int getColor(Framebuffer fb, int x, int y) {
	if (!(between(0, x, fb.width) && between(0, y, fb.height)))
		return -1;
	return fb.fb[y * fb.width + x];
}

int saveFrame(Framebuffer fb, char *path) {
	FILE *file = fopen(path, "w");

	if (file == NULL)
		return 1;

	fprintf(file, "P6\n"
	              "# Created by fbgl\n"
	              "%d %d\n"
	              "255\n", fb.width, fb.height);
	for (int i = 0; i < fb.width * fb.height; i++) {
		if (fprintf(file, "%c%c%c",
		            fb.fb[i] & 0xff,
		            (fb.fb[i] & 0xff00) >> 8,
		            (fb.fb[i] & 0xff0000) >> 16) < 0) {
		//I could use getColor, but this is much faster. Also haha lisp go brrr.
			return 1;
		}
	}

	fclose(file);
	return 0;
}
