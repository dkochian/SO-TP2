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
	#define STDCOLOR			4
	#define STDTOAST			5
	#define STDBGCOLOR			6
	#define STDPIPE				7

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
	#define PS					15
	#define PID					16
	#define YIELD				17
	#define MINIT				18
	#define MLOCK				19
	#define MUNLOCK				20
	#define MDESTROY			21
	#define MISLOCK				22
	#define BLOCKSELF			23
	#define WAITPID				24
	#define SEMOPEN				25
	#define SEMCLOSE			26
	#define SEMWAIT				27
	#define SEMPOST				28
	#define CVINIT				29
	#define CVWAIT				30
	#define CVSIG				31
	#define CVBROAD				32
	#define CVDESTROY			33
	#define PINIT				34
	#define PDESTROY			35

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

	//Pipes
	#define INVALID_PIPE_ID		(0)

	//Other
	#define VGA_WIDTH		1024
	#define VGA_HEIGHT		768
	#define VGA_DEPTH		24

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

    typedef struct psContext{
     	uint64_t numbProcess;
     	uint64_t memoryUsed;
     	char **processes;
     	char separateChar;
   }psContext;
	
	typedef char bool;
	typedef struct mutex *mutex;
	typedef struct semaphore *semaphore;
	typedef struct list *cond_t;
	typedef unsigned int size_t;
	typedef int (*func) (int, char **);

#endif