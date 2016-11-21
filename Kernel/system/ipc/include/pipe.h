
#ifndef __INC_PIPE__
	#define __INC_PIPE__

	#include "../../../include/common.h"

	#define MAX_PIPES	32

	bool pipeInit();
	uint64_t pipeBuild();
	void pipeDestroy(uint64_t id);
	void pipeRead(uint64_t id, char *str);
	bool pipeWrite(uint64_t id, const char *str);

#endif