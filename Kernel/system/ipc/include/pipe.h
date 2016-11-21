
#ifndef __INC_PIPE__
	#define __INC_PIPE__

	#include "../../../include/common.h"

	#define MAX_PIPES	32

	bool pipeInit();
	uint64_t pipeBuild();
	void pipeDestroy(uint64_t id);
	char pipeRead(uint64_t id);
	bool pipeWrite(uint64_t id, char c);

#endif