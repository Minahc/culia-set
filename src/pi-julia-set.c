#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "bmp.h"
#include "colors.h"

#define WIDTH 1000 // width in pixels
#define HEIGHT 1000 // height in pixels
#define BPP 24 // bits per pixel
#define HEADER 54// size of header data
#define ROWSIZE (((BPP * WIDTH) / 32) * 4) // bytes per row
#define IMAGESIZE (ROWSIZE * HEIGHT) // size of image data
#define SIZE (HEADER + IMAGESIZE) // header data + image data

#define R 1.251105362355361781933320// escape radius of Julia set
#define MAX_ITERATION 1000

double julia(double zx, double zy) {
	double cx = M_PI / 10.0;
	double cy = 0.0;
	double smooth_color = exp(-sqrt(zx * zx + zy * zy));

	uint32_t iteration = 0;

	while (zx * zx + zy * zy < R * R && iteration < MAX_ITERATION) {
		double xtemp = zx * zx - zy * zy;
		zy = 2 * zx * zy + cy;
		zx = xtemp + cx;
		smooth_color += exp(-sqrt(zx * zx + zy * zy));

		iteration++;
	}

	return smooth_color;
}

int main() {
	// Initialize bitmap
	bmp_t bitmap;

	// Write BMP header to bitmap
	bmp_make_header(WIDTH, HEIGHT, BPP, &bitmap);

	if (!bitmap.data) { // check for bitmap initialization errors
		return 1;
	}

	// Calculate Julia set value for each pixel, convert value to hsl, then rgb and write to bitmap
	for (uint32_t y = 0; y < HEIGHT; y++) {
		double zy = (double)y / HEIGHT * 2 * R - R;
		for (uint32_t x = 0; x < WIDTH; x++) {
			double zx = (double)x / HEIGHT * 2 * R - R;
			double val = julia(zx, zy);

			double h,s,l;
			char r,g,b;
			val_to_hsl(val, MAX_ITERATION, &h, &s, &l);
			hsl_to_rgb(h, s, l, &r, &g, &b);
			color_pixel_rgb(x, y, r, g, b, &bitmap);
		}
	}

	// Write bitmap to BMP file
	write_bmp("pi-julia.bmp", &bitmap);

	// Free memory used for bitmap
	free_bitmap(&bitmap);
	return 0;
}
