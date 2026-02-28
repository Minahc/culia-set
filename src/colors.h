#ifndef COLOR_H
#define COLOR_H

void val_to_hsl(double val, uint32_t maxVal, double* h, double* s, double* l);
void hsl_to_rgb(double h, double s, double l, char* r, char* g, char* b);

#endif
