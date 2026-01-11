#ifndef BMP_H
#define BMP_H

typedef struct {
	char* data;
	char* image;
	int32_t width;
	uint32_t row_width;
	int32_t height;
	uint32_t size;
	uint32_t img_size;
}bmp_t;

// Should probably either be in different helper library or not in header file
void int_to_4_bytes(int32_t i, char* res);

// Free bitmap
void free_bitmap(bmp_t* bitmap);

// Set bitmap data
void bmp_make_header(int32_t width, int32_t height, uint16_t bpp, bmp_t* bitmap);
void color_pixel_rgb(uint32_t x, uint32_t y, char red, char green, char blue, bmp_t* bitmap);

// Write out bitmap to bmp file
void write_bmp(const char* filename, bmp_t* bitmap);

#endif
