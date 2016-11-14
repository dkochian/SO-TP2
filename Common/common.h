#ifndef _INC_COMMON_

	#define _INC_COMMON_

	#include <stdint.h>

	//Utils
	#define NULL				0x0
	#define MAX_BUFFER			128

	//boolean values
	#define true				1
	#define false				!true

	//Misc
	#define STDOUT				1
	#define STDERR				2
	#define STDIN				3
	#define COLOR				4
	#define TOAST				5
	#define BGCOLOR				6

	//System calls ids
	#define SYSREAD				3
	#define SYSWRITE			4
	#define SETCOLOR			5
	#define CLEAR				6
	#define GETTIME				7
	#define GETDATE				8
	#define SLEEP				9
	#define DRAWPIXEL			10
	#define MALLOC				11
	#define FREE				12
	#define NEWPROCESS			13	
	#define KILLPROCESS			14
	#define BLOCKPROCESS		15
	#define UNBLOCKPROCESS		16
	#define PS					17
	#define PID					18
	#define YIELD				19

	//Keyboard
	#define EMPTY				0
	#define WIDTH 				128
	#define KB_SIZE				112

	//Colors
	#define BLACK				0
	#define BLUE				1
	#define GREEN				2
	#define CYAN				3
	#define RED					4
	#define MAGENTA				5
	#define BROWN				6
	#define LIGHT_GRAY			7
	#define DARK_GRAY			8
	#define LIGHT_BLUE			9
	#define LIGHT_GREEN			10
	#define LIGHT_CYAN			11
	#define LIGHT_RED			12
	#define LIGHT_MAGENTA		13
	#define YELLOW				14
	#define WHITE				15

	#define COLOR_DEFAULT		WHITE
	#define COLOR_BG_DEFAULT	BLACK
	#define COLOR_ERROR			RED
	#define COLOR_CLEAR			BLACK
	#define COLOR_INFO			BLUE
	#define COLOR_NULL			(-1)

	//Process
	#define INVALID_PROCESS_ID	(-1)

	typedef struct {
		uint8_t
			secs,
			mins,
			hrs;
	} timeStruct;

	typedef struct {
		uint8_t
			day,
			dayOfWeak,
			month,
			year;
	} dateStruct;

	typedef struct {
		uint8_t
	  		r,
	  		g,
	  		b;
	} ColorRGB;

	typedef unsigned int size_t;

	typedef char bool;

	typedef int (*func) (int, char **);

#endif
