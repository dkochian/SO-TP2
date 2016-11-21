#ifndef __CONDVAR_H__
    #define __CONDVAR_H__

	#include "mutex.h"
	#include "../../../utils/include/list.h"
	#include "../../../../Common/common.h"

	typedef struct cond_t *list;

	cond_t cvInitialize();
	void cvWait(cond_t cv, mutex cv_mutex);
	void cvSignal(cond_t cv);
	void cvBroadcast(cond_t cv);
	void cvDestroy(cond_t cv);

#endif