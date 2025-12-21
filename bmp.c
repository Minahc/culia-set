#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "bmp.h"

// Convert 32 bit integer to char array of 4 bytes
void int_to_4_bytes(int32_t i, char* res) {
	res[3] = (i >> 24) & 0xFF;
	res[2] = (i >> 16) & 0xFF;
	res[1] = (i >> 8) & 0xFF;
	res[0] = i & 0xFF;
}

// Allocate memory for bitmap
void _init_bitmap(uint32_t hdr_size, int32_t width, int32_t height, uint16_t bpp, bmp_t* bitmap) {
	bitmap->row_width = ((bpp * width + 31) / 32) * 4;
	bitmap->img_size = bitmap->row_width * height;
	bitmap->size = hdr_size + bitmap->img_size;
	bitmap->data = calloc(bitmap->size, sizeof(char));
	bitmap->width = width;
	bitmap->height = height;
	bitmap->image = bitmap->data + hdr_size;
}

// Free allocated memory for bitmap
void free_bitmap(bmp_t* bitmap) {
	free(bitmap->data);
	bitmap->data = 0;
}

// Creates file and DIB header for BMP image using BITMAPINFOHEADER, 24 bit per pixel, no compression and vertical and horizontal resolution being 2835 pixles per meter
void bmp_make_header(int32_t width, int32_t height, uint16_t bpp, bmp_t* bitmap) {
	const uint32_t hdr_size = 54;
	_init_bitmap(hdr_size, width, height, bpp, bitmap);
	// -- FILE HEADER -- //
	
	// bitmap signature (2 bytes)
	bitmap->data[0] = 'B';
	bitmap->data[1] = 'M';

	// file size in bytes (4 bytes)
	char size[4];
	int_to_4_bytes((int) bitmap->size, size);
	for (int i = 0; i < 4; i++) {
		bitmap->data[i+2] = size[i];
	}

	// reserved fields, can be 0 when manually created (4 bytes)
	bitmap->data[6] = 0;
	bitmap->data[7] = 0;
	bitmap->data[8] = 0;
	bitmap->data[9] = 0;
	
	// offset to start of pixel array (4 bytes)
	bitmap->data[10] = hdr_size;
	bitmap->data[11] = 0;
	bitmap->data[12] = 0;
	bitmap->data[13] = 0;

	// -- BITMAP HEADER -- //
	
	// size of header (4 bytes)
	bitmap->data[14] = 40;
	bitmap->data[15] = 0;
	bitmap->data[16] = 0;
	bitmap->data[17] = 0;

	// bitmap width in pixels, signed integer (4 bytes)
	char widthArr[4];
	int_to_4_bytes(width, widthArr);
	for (int i = 0; i < 4; i++) {
		bitmap->data[i+18] = widthArr[i];
	}
	
	// bitmap height in pixels, signed integer (4 bytes)
	char heightArr[4];
	int_to_4_bytes(height, heightArr);
	for (int i = 0; i < 4; i++) {
		bitmap->data[i+22] = heightArr[i];
	}

	// number of color planes, must be 1 (2 bytes)
	bitmap->data[26] = 1;
	bitmap->data[27] = 0;

	// number of bits per pixel (2 bytes)
	bitmap->data[28] = bpp; 
	bitmap->data[29] = 0;

	// compression method (4 bytes)
	bitmap->data[30] = 0;
	bitmap->data[31] = 0;
	bitmap->data[32] = 0;
	bitmap->data[33] = 0;

	// image size, size of raw bitmap data (4 bytes)
	char imgSize[4];
	int_to_4_bytes(bitmap->img_size, imgSize);
	for (int i = 0; i < 4; i++) {
		bitmap->data[i+34] = imgSize[i];
	}

	// horizontal resolution, pixels per meter, signed integer (4 bytes)
	bitmap->data[38] = 0b00010011;
	bitmap->data[39] = 0b00001011;
	bitmap->data[40] = 0;
	bitmap->data[41] = 0;

	// vertical resolution, pixels per meter, signed integer (4 bytes)
	bitmap->data[42] = 0b00010011;
	bitmap->data[43] = 0b00001011;
	bitmap->data[44] = 0;
	bitmap->data[45] = 0;

	// number of colors (4 bytes)
	bitmap->data[46] = 0; 
	bitmap->data[47] = 0;
	bitmap->data[48] = 0;
	bitmap->data[49] = 0;

	// number of important colors, 0 for every color (4 bytes)
	bitmap->data[50] = 0;
	bitmap->data[51] = 0;
	bitmap->data[52] = 0;
	bitmap->data[53] = 0;
}

void color_pixel_rgb(uint32_t x, uint32_t y, char red, char green, char blue, bmp_t* bitmap) {
	bitmap->image[x * 3 + y * bitmap->row_width + 0] = blue;
	bitmap->image[x * 3 + y * bitmap->row_width + 1] = green;
	bitmap->image[x * 3 + y * bitmap->row_width + 2] = red;
}

void write_bmp(const char* filename, bmp_t* bitmap) {
	FILE* bmp;
	bmp = fopen(filename, "w");
	for (int i = 0; i < bitmap->size; i++) {
		fputc(bitmap->data[i], bmp);
	}
	fclose(bmp);
}
