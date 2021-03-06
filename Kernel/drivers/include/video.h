#ifndef __VIDEO__H__

	#define __VIDEO__H__

	#include "../../include/common.h"

	#define HEIGHT 				47
	
	#define MAX_CHAR_LINE		MAX_BUFFER

	typedef struct {
		char c;
		char color;
	} charStructure;

	bool videoBuild();
	void videoDestroy();
	void videoStartLocking();
	void videoStopLocking();
	void print(const char* str, char color);
	void putChar(char c, char color);
	void printDec(uint64_t value, char color);
	void printHex(uint64_t value, char color);
	void printNewLine();
	void clear();
	void setDefaultColor(char color);
	void setBgDefaultColor(char color);
	char getDefaultColor();
	void printPixel(uint16_t x, uint16_t y, ColorRGB* color);
	void printToast(const char* str, int size);

	//TODO: remove next, it's just for testing and should never be used
	void unsafe_print(const char* str, char color);
	void unsafe_printNewLine();

#endif