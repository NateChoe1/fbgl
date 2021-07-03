/*
 * bounce: An fbgl example
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

#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define between(low, value, high) ((low <= value) && (value <= high))

int main() {
	noecho();

	Framebuffer fb = createFramebuffer(getenv("FRAMEBUFFER"));

	clearFramebuffer(fb, rgb(0, 0, 0));

	int x = rand() % fb.width;
	int y = rand() % fb.height;
	int xSpeed = rand() % 10;
	int ySpeed = 0;
	for (;;) {
		clearFramebuffer(fb, rgb(0, 0, 0));

		drawFilledCircle(fb, x, y, 5, rgb(0, 0, 0), rgb(255, 255, 255));

		x += xSpeed;
		y += ySpeed;
		if (!between(0, x, fb.width))
			xSpeed = -xSpeed;
		if (y > fb.height) {
			ySpeed = -ySpeed;
			y += ySpeed;
		}
		ySpeed += 1;

		usleep(16666);
	}

	yesecho();
}
