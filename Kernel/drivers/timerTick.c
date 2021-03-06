#include "include/timerTick.h"
#include "../system/scheduler/include/scheduler.h"
#include "include/video.h"

static int tickCount = 0;

uint64_t timerTickHandler(uint64_t s_frame) {
	tickCount++;

	if(tickCount >= TICK_TO_SWICH_PROCESS) {
		tickCount = 0;
		return contextSwitch(s_frame);
	}
	return 0;
}

uint64_t yieldHandler(uint64_t s_frame) {
	tickCount = 0;
	return contextSwitch(s_frame);
}