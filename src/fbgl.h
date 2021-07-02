typedef struct {
	int *fb;
	//mmaped to /dev/fbX. Each value is of the form 0x00rrggbb with the
	//endianness of the system. The point at (x, y) is at index (y * width + x)
	int width;
	int height;
} Framebuffer;

Framebuffer createFramebuffer(char *fbpath);
//Creates a new Framebuffer using the specified framebuffer path (or /dev/fb0 if
//NULL)

#define rgb(r, g, b) ((r << 16) | (g << 8) | b)
void setPoint(Framebuffer fb, int x, int y, int color);
void clearFramebuffer(Framebuffer fb, int color);
void drawLine(Framebuffer fb, int x1, int y1, int x2, int y2, int color);
void drawCircle(Framebuffer fb, int xc, int yc, int r, int color);
//x center, y center, radius, color
void drawFilledCircle(Framebuffer fb,
		int xc, int yc, int r, int border, int fill);
//x center, y center, radius, border color, fill color
void drawPolygon(Framebuffer fb, int pointCount, int *points, int color);

void noecho();
void yesecho();
//Just so that the console doesn't try to draw over your stuff.

int getColor(Framebuffer fb, int x, int y);
//Gets color at coordinate, if the coordinate is invalid -1 is returned.
int saveFrame(Framebuffer fb, char *path);
//If this messes up, it'll return 1.
