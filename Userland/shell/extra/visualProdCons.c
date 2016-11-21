#include "include/visualProdCons.h"
#include "../../libc/include/draw.h"
#include <stdio.h>
#include <syscall.h>


ColorRGB * blackColor;
ColorRGB * greenColor;
ColorRGB * redColor;
const int x = 450;
const int y = 150;
int currentY;
const int width = 100;


void initVisual() {
	clear();
	printn("Producer: Increase Speed - z | Decrease Speed - a  |||  Consumer: Increase Speed - x | Decrease Speed - s");
	printn("Press q to quit");
	blackColor = colorRGB(0,0,0);
	greenColor = colorRGB(102,204,0);
	redColor = colorRGB(255,0,0);
	currentY = y;
}

void visualProduce(char c) {
	drawRectangleBottonToTop(x, currentY, width, c, *greenColor);
	currentY += c;
}

void visualConsume(char c) {
	drawRectangleBottonToTop(x, currentY-c, width, c, *redColor);
	currentY -= c;
}

void clearConsumer() {
	drawRectangleBottonToTop(x, currentY, width, VGA_HEIGHT - currentY -40, *blackColor);
}