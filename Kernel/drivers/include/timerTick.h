#ifndef _INC_TIMERTICK_
	#define _INC_TIMERTICK_

	#define TICK_TO_SWICH_PROCESS	5// Should be 2 or 1 (we must change the clock I guess)

	#include <stdint.h>

	uint64_t timerTickHandler(uint64_t s_frame);
	void resetTickCounter();

#endif