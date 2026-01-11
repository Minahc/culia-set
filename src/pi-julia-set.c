#include <string.h>
#include <stdlib.h>
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

#define MAX_ITERATION 1000

double to_double(char * str) {
	uint32_t i;
	double div = 1.0;

	uint32_t out_int = 0;
	uint32_t out_dec = 0;

	char *decimal_place = strchr(str, '.');

	out_int = atoi(str);

	if (decimal_place) {
		*decimal_place = 0;
		out_dec = atoi(decimal_place + 1);

		for (i = 0; i < strlen(decimal_place + 1); i++) {
			div /= 10.0;
		}
	}

	return out_int + out_dec * div;
}

double calc_r(double cx, double cy) {
	return 0.5 * (sqrt(4 * sqrt(cx * cx + cy * cy) + 1) + 1);
}

double julia(double zx, double zy, double cx, double cy, double R) {
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

int main(int argc, char ** argv) {
	// Initialize bitmap
	bmp_t bitmap;

	// Write BMP header to bitmap
	bmp_make_header(WIDTH, HEIGHT, BPP, &bitmap);

	if (!bitmap.data) { // check for bitmap initialization errors
		return 1;
	}

	if (argc < 2) {
		printf("Error: Not enough arguments\n\nUSAGE: %s <cr> <ci>\n", argv[0]);
		return 1;
	}

	double cx = to_double(argv[1]);
	double cy = to_double(argv[2]);
	double R = calc_r(cx, cy);

	// Calculate Julia set value for each pixel, convert value to hsl, then rgb and write to bitmap
	for (uint32_t y = 0; y < HEIGHT; y++) {
		double zy = (double)y / HEIGHT * 2 * R - R;
		for (uint32_t x = 0; x < WIDTH; x++) {
			double zx = (double)x / HEIGHT * 2 * R - R;
			double val = julia(zx, zy, cx, cy, R);

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
