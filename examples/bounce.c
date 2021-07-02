#include "fbgl.h"

#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define between(low, value, high) ((low <= value) && (value <= high))

int main() {
	noecho();

	Framebuffer fb = createFramebuffer(NULL);

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
