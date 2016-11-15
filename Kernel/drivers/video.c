#include "include/video.h"
#include "include/font.h"

#include "../include/clock.h"

#define ROW(cursor_offset) ((cursor_offset)/WIDTH)
#define COL(cursor_offset) ((cursor_offset)%WIDTH)

static char isValidOffset(int row, int col);
static void printChar(char c, int row, int col, char color);
static void reDrawMatrix();
static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
static void printBase(uint64_t value, uint32_t base, char color);
static void printCharPixelLine(char line, int x, int y, char color);
static ColorRGB getRGBColor(char color);
static char isValidColor(char color);

char
	defaultColor = COLOR_DEFAULT,
	defaultBgColor = COLOR_BG_DEFAULT;
char*
	video;
int 
	offset = 0;
charStructure
	charMatrix[HEIGHT][WIDTH];
ColorRGB 
	baseColors[16] = {
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

void printA() {
	print("printing A", -1);
	printNewline();
	sleep(2);
}

void printB() {
	print("printing B", -1);
	printNewline();
}

void printC() {
	print("printing C", -1);
	printNewline();
}

void print(const char* str, char color) {
	for(int i = 0; str[i] != '\0' && isValidOffset(ROW(offset), COL(offset)); i++) {
		printChar(str[i], ROW(offset), COL(offset), color);
		offset++;
	}
}

void printToast(const char* str, int size) {
	int
		bOffset = offset;
	offset = (HEIGHT - 1)*WIDTH + WIDTH/3;
	print(str, COLOR_NULL);
	offset = bOffset;
}

void printDec(uint64_t value, char color) {
	printBase(value, 10, color);
}

void printHex(uint64_t value, char color) {
	printBase(value, 16, color);
}

void putChar(char c, char color) {
	if(c == '\b') {
		offset--;
		printChar(' ', ROW(offset), COL(offset), color);
	} else if(c == '\n') {
		printNewline();
		if(ROW(offset) == HEIGHT)
			reDrawMatrix();
	}
	else {
		printChar(c, ROW(offset), COL(offset), color);
		offset++;
		if(ROW(offset) == HEIGHT)
			reDrawMatrix();
	}
}

void printNewline() {
	offset += WIDTH - COL(offset);
}

void clear() {
	ColorRGB bgcolor = getRGBColor(defaultBgColor);
	for(int i = 0; i < 1025; i ++)
		for(int j=0; j<769; j++)
			printPixel(i, j, &bgcolor);

	offset = 0;
}

void setDefaultColor(char color) {
	defaultColor = color;
}

void setBgDefaultColor(char color) {
	defaultBgColor = color;
}

char getDefaultColor() {
	return defaultColor;
}

void printPixel(uint16_t x, uint16_t y, ColorRGB* color) {
	uint64_t vesaBufferInt = *(uint32_t *)0x5080;
	unsigned char * vesaBuffer = (unsigned char *)vesaBufferInt;;
	unsigned char*
		adress = vesaBuffer + 3*(x + y * 1024);

    adress[2] = color->r;
    adress[1] = color->g;
    adress[0] = color->b;
}

static char isValidOffset(int row, int col) {
	return row < HEIGHT && col < WIDTH && row >= 0 && col >= 0;
}

static void printChar(char c, int row, int col, char color) {
	if(isValidOffset(row, col)) {
		int
			x = col*CHAR_WIDTH,
			y = row*CHAR_HEIGHT;
		unsigned char
			pixel;
		unsigned char*
			map = pixel_map(c);
		if(color == COLOR_NULL)
			color = defaultColor;

		charMatrix[ROW(offset)][COL(offset)].c = c;
		charMatrix[ROW(offset)][COL(offset)].color = color;

		for(int i=0; i<CHAR_HEIGHT; i++) {
			pixel = map[i];
			printCharPixelLine(pixel, x, y + i, color);
		}
	}
}

static void printCharPixelLine(char line, int x, int y, char color) {
	char
		aux,
		mask[] = {128, 64, 32, 16, 8, 4, 2, 1};
	ColorRGB
		rColor;
	for(int i=0; i<CHAR_WIDTH; i++) {
		aux = line & mask[i];
		if(aux != 0)
			rColor = getRGBColor(color);
		else
			rColor = getRGBColor(defaultBgColor);
			
		printPixel(x+i, y, &rColor);
	}
}

static void reDrawMatrix() {
	offset = 0;
	for(int row=0; row<HEIGHT; row++) {
		for(int col=0; col<WIDTH; col++) {
			if(row == HEIGHT - 1) {
				charMatrix[row][col].c = ' ';
				charMatrix[row][col].color = defaultColor;
			}
			else {
				charMatrix[row][col].c = charMatrix[row + 1][col].c;
				charMatrix[row][col].color = charMatrix[row + 1][col].color;
			}
			printChar(charMatrix[row][col].c, row, col, charMatrix[row][col].color);
		}
	}
	offset = WIDTH*(HEIGHT - 1);
}

static void printBase(uint64_t value, uint32_t base, char color) {
	char
		buffer[64] = {'0'};

	uintToBase(value, buffer, base);
	for(int i = 0; buffer[i] != '\0' && isValidOffset(ROW(offset), COL(offset)); i++) {
		printChar(buffer[i], ROW(offset), COL(offset), color);
		offset++;
	}
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

static ColorRGB getRGBColor(char color) {
	if(isValidColor(color))
		return baseColors[(int) color];
	return baseColors[(int) defaultColor];
}

static char isValidColor(char color) {
	return color >= BLACK && color <= WHITE;
}