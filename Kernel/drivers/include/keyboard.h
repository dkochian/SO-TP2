#ifndef __KB__H__
	#define __KB__H__
	#include "../../include/common.h"

	#define DO_NOTHING		0
	#define ESCAPE			1
	#define ESCAPE_RL		129
	#define CAPS			58
	#define SHIFTL  		42
	#define SHIFTR	 		54
	#define SHIFTL_RL 		170
	#define SHIFTR_RL 		182
	
	

	typedef struct {
		uint8_t
			shiftOn,
			capsOn;
		int
			writeIndex,
			readIndex;
		char
			buffer[KB_SIZE];
	} kbStatus;

	void keyboardInit();
	void keyboardHandler(unsigned char key);
	char getKey(char write);

#endif
