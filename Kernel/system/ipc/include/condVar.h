#ifndef __CONDVAR_H__
    #define __CONDVAR_H__

	#include "mutex.h"
	#include "../../../../Common/common.h"

	struct cond_t {
		list cv_list;
	};

	typedef struct cond_t *cond_t;

	cond_t cvInitialize();
	void cvWait(cond_t cv, mutex cv_mutex);
	process *cvSignal(cond_t cv);
	void cvBroadcast(cond_t cv);
	void cvDestroy(cond_t cv);



#endif