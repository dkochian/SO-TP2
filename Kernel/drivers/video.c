#include "include/video.h"
#include "include/font.h"
#include "../include/common.h"
#include "../system/ipc/include/mutex.h"

//TODO: Delete of characters if fucked up, should be fixed

#define ROW(cursor_offset) ((cursor_offset)/WIDTH)
#define COL(cursor_offset) ((cursor_offset)%WIDTH)

static void reDrawMatrix();
static void iPrintNewLine();
static char isValidColor(char color);
static ColorRGB getRGBColor(char color);
static char isValidOffset(int row, int col);
static void printChar(char c, int row, int col, char color);
static void printBase(uint64_t value, uint32_t base, char color);
static void iPrintPixel(uint16_t x, uint64_t y, ColorRGB *color);
static void printCharPixelLine(char line, int x, int y, char color);
static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char
	defaultColor = COLOR_DEFAULT,
	defaultBgColor = COLOR_BG_DEFAULT;
static int offset = 0;
static bool must_lock; 
mutex v_mutex;
static charStructure charMatrix[HEIGHT][WIDTH];
static ColorRGB 
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

bool videoBuild() {
	v_mutex = lockBuild();

	if(v_mutex == NULL)
		return false;

	must_lock = false;

	return true;
}

void videoDestroy() {
	lockDestroy(v_mutex);
}

void videoStartLocking() {
	must_lock = true;
}

void videoStopLocking() {
	must_lock = false;
}

void print(const char* str, char color) {
	if(must_lock == true)
		lock(v_mutex);

	for(int i = 0; str[i] != '\0' && isValidOffset(ROW(offset), COL(offset)); i++) {
		if(str[i] == '\n')
			iPrintNewLine();
		else {
			printChar(str[i], ROW(offset), COL(offset), color);
			offset++;
		}
	}

	if(must_lock == true)
		unlock(v_mutex);
}

void printToast(const char* str, int size) {
	if(must_lock == true)
		lock(v_mutex);

	int
		bOffset = offset;
	offset = (HEIGHT - 1)*WIDTH + WIDTH/3;
	print(str, COLOR_NULL);
	offset = bOffset;

	if(must_lock == true)
		unlock(v_mutex);
}

void printDec(uint64_t value, char color) {
	if(must_lock == true)
		lock(v_mutex);

	printBase(value, 10, color);

	if(must_lock == true)
		unlock(v_mutex);
}

void printHex(uint64_t value, char color) {
	if(must_lock == true)
		lock(v_mutex);

	printBase(value, 16, color);

	if(must_lock == true)
		unlock(v_mutex);
}

void putChar(char c, char color) {
	if(must_lock == true)
		lock(v_mutex);

	if(c == '\b') {
		offset--;
		printChar(' ', ROW(offset), COL(offset), color);
	} else if(c == '\n') {
		iPrintNewLine();
		if(ROW(offset) == HEIGHT)
			reDrawMatrix();
	}
	else {
		printChar(c, ROW(offset), COL(offset), color);
		offset++;
		if(ROW(offset) == HEIGHT)
			reDrawMatrix();
	}

	if(must_lock == true)
		unlock(v_mutex);
}

void printNewLine() {
	if(must_lock == true)
		lock(v_mutex);


	iPrintNewLine();

	if(must_lock == true)
		unlock(v_mutex);
}

void clear() {
	if(must_lock == true)
		lock(v_mutex);

	ColorRGB bgcolor = getRGBColor(defaultBgColor);
	for(int i = 0; i < 1025; i ++)
		for(int j=0; j<769; j++)
			iPrintPixel(i, j, &bgcolor);

	offset = 0;

	if(must_lock == true)
		unlock(v_mutex);
}

void setDefaultColor(char color) {
	if(must_lock == true)
		lock(v_mutex);

	defaultColor = color;

	if(must_lock == true)
		unlock(v_mutex);
}

void setBgDefaultColor(char color) {
	if(must_lock == true)
		lock(v_mutex);

	defaultBgColor = color;

	if(must_lock == true)
		unlock(v_mutex);
}

char getDefaultColor() {
	if(must_lock == true)
		lock(v_mutex);

	char aux = defaultColor;

	if(must_lock == true)
		unlock(v_mutex);

	return aux;
}

void drawSquare(uint16_t x, uint16_t y, uint16_t l, char colorCode) {
	ColorRGB color = getRGBColor(colorCode);

	uint64_t vesaBufferInt = *(uint32_t *)0x5080;
	unsigned char* vesaBuffer = (unsigned char *)vesaBufferInt;
	unsigned char* address;
	
	for(int i=0; i<l; i++) {
		for(int j=0;j<l;j++) {
			address = vesaBuffer + 3*(x+i + (y+j)*1024);
			address[2] = color.r;
			address[1] = color.g;
			address[0] = color.b;
		}
	}
}

void printPixel(uint16_t x, uint16_t y, ColorRGB *color) {
	if(must_lock == true)
		lock(v_mutex);

	iPrintPixel(x, y, color);

	if(must_lock == true)
		unlock(v_mutex);
}

static void iPrintNewLine() {
	offset += WIDTH - COL(offset);
}

static void iPrintPixel(uint16_t x, uint64_t y, ColorRGB *color) {
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
			
		iPrintPixel(x+i, y, &rColor);
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

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base) {
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