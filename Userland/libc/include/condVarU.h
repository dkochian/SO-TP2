#ifndef __CONDVAR_H__
    #define __CONDVAR_H__

	#include "../../../Common/common.h"

	typedef void* cond_u_t;

	cond_u_t cvInitialize();
	void cvWait(cond_u_t cv, mutex m);
	uint64_t cvSignal(cond_u_t cv);
	void cvBroadcast(cond_u_t cv);
	void cvDestroy(cond_u_t cv);

#endif