#ifndef DRAW_H
#define DRAW_H

#include "syscall.h"
#include "../../../Common/common.h"

#define MAXY 768

ColorRGB charToRGB(char in);

ColorRGB * colorRGB(uint8_t r, uint8_t g, uint8_t b);

void drawSquare(int x, int y, int l,  uint32_t c);

void drawRectangleBottonToTop(int x, int y, int width, int height, ColorRGB color);


#endif
