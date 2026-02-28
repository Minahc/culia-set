#include <stdint.h>
#include <math.h>

#include "colors.h"

#define MODULO(a, b) ((a) - floor((a) / (b)) * (b))
#define MIN(a, b) a < b ? a : b
#define MAX(a, b) a > b ? a : b
#define CLIP(a) MIN(MAX(a, 0.0), 1.0)

void val_to_hsl(double val, uint32_t maxVal, double* h, double* s, double* l) {
	//*h = 240.0;
	//*s = 1.0;
	//*l = pow(val / (double) maxVal, 0.1);
	*h = 0.0;//(1.0 + 10 * (val / maxVal)) * 360;
	*s = 0.6;
	*l = CLIP((-val / maxVal) * 20.0 + 1.0);
	while (*h > 360) *h -= 360.0;
}

void hsl_to_rgb(double h, double s, double l, char* r, char* g, char* b) {
	double c = (1.0 - fabs((2.0 * l) - 1.0)) * s;
	double hp = h / 60.0;
	double x = c * (1.0 - fabs(MODULO(hp, 2.0) - 1.0));
	double r1, g1, b1;
	if (hp >=0 && hp < 1) {
		r1 = c;
		g1 = x;
		b1 = 0.0;
	} else if (hp >= 1 && hp < 2) {
		r1 = x;
		g1 = c;
		b1 = 0.0;
	} else if (hp >= 2 && hp < 3) {
		r1 = 0.0;
		g1 = c;
		b1 = x;
	} else if (hp >= 3 && hp < 4) {
		r1 = 0.0;
		g1 = x;
		b1 = c;
	} else if (hp >= 4 && hp < 5) {
		r1 = x;
		g1 = 0.0;
		b1 = c;
	} else if (hp >= 5 && hp < 6) {
		r1 = c;
		g1 = 0.0;
		b1 = x;
	} else {
		r1 = 0.0;
		g1 = 0.0;
		b1 = 0.0;
	}

	double m = l - c / 2.0;
	
	*r = ((int32_t)((r1 + m) * 255)) & 0xFF;
	*g = ((int32_t)((g1 + m) * 255)) & 0xFF;
	*b = ((int32_t)((b1 + m) * 255)) & 0xFF;
}
