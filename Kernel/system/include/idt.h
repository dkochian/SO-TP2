#ifndef _INC_IDT_
	#define _INC_IDT_

	#include "atom.h"
	#include "../include/common.h"

	#define INTERRUPT_GATE		0xE
	#define PRESENT				0x80 //(10000000b)
	#define SEGMENT         	0xEF //(11101111b)
	#define DPL					0x0  //(00000000b)
	#define DEFAULT_ATTRIBUTES	((INTERRUPT_GATE | PRESENT | DPL) & SEGMENT)

	typedef struct __attribute__((packed)) {
		int16_t offsetLo;		//low 16b
		int16_t selector;		//code segment
		int8_t z1;			//always 0
		int8_t attributes;	//See below
		int16_t offsetMi;		//mid 16b
		int32_t offsetHi;		//high 32b
		int32_t z2;			//always 0
	} IDTEntry;

	/*	Arg	Size(B)	extra
		P 	  1 	  1 if it's used, otherwise 0
		DPL   2 	  Privilege level
		S     1 	  0 for interrupt gates
		Type  4 	  gate type
	*/

	void buildIDT();
		
	extern void _yieldHandler();
	extern void _sysCallHandler();
	extern void _keyboardHandler();
	extern void _timerTickHandler();
	extern void _write_port(int16_t, int16_t);

#endif