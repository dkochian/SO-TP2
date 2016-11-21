#include "include/draw.h"

ColorRGB baseColors[16] = {
		{0,		0,		0},
		{0,		0,		255},
		{0,		128,	0},
		{0,		255,	255},
		{255,	0,		0},
		{255,	0,		255},
		{165,	42,		42},
		{211,	211,	211},
		{128,	128,	128},
		{0,		128,	255},
		{144,	238,	144},
		{224,	255,	255},
		{255,	77,		77},
		{237,	83,		147},
		{255,	255,	0},
		{255,	255,	255}
	};

ColorRGB charToRGB(char in) {
	//if(in<16 && in>=0) {
		return baseColors[(uint32_t)in];

}

ColorRGB * colorRGB(uint8_t r, uint8_t g, uint8_t b) {
	ColorRGB * color = malloc(sizeof(ColorRGB));
	color->r = r;
	color->g = g;
	color->b = b;

	return color;
}

void drawSquare(int x, int y, int l, uint32_t c) {
	ColorRGB color = charToRGB(c);
	for (int i = x; i < x+l; i++) {
		for (int j = y; j < y+l; j++) {
			putPixel(i,j,&color);
		}
	}
}


void drawRectangleBottonToTop(int x, int y, int width, int height, ColorRGB color) {

	int i,j;

	for (i = x; i < x+width; i++) {
		for (j = MAXY-y-height; j < MAXY-y; j++) {
			putPixel(i,j,&color);
		}
	}
}