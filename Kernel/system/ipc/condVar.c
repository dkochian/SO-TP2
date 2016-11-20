//http://www.cs.wustl.edu/~schmidt/win32-cv-1.html

#include "../include/mmu.h"
#include "include/condVar.h"
#include "../../utils/include/list.h"
#include "../scheduler/include/scheduler.h"


cond_t cvInitialize() {
    cond_t cv = k_malloc(sizeof(cond_t));
	cv->cv_list = listBuild(&equal);
	if (cv->cv_list){
		k_free(cv);
		return NULL;
	}
    return cv;
}

void cvWait(cond_t cv, mutex cv_mutex) {	

	process *cProcess = getCurrentProcess();
	unlock(cv_mutex);
	listAdd(cv->cv_list, cProcess);

	blockProcess(cProcess->id);  
	_yield();
	
	lock(cv_mutex);
}

uint64_t cvSignal(cond_t cv) {
	process *rProcess;
	if (!listIsEmpty(cv->cv_list)) {
		rProcess = listGetFirst(cv->cv_list);
		listRemove(cv->cv_list, rProcess);
		unBlockProcess(rProcess->id);
	}

	return rProcess->id;
}

void cvBroadcast(cond_t cv) {
	while (!listIsEmpty(cv->cv_list)) {
		process *rProcess = listGetFirst(cv->cv_list);
		listRemove(cv->cv_list, rProcess);
		unBlockProcess(rProcess->id);
	}
}

void cvDestroy(cond_t cv) {
	cvBroadcast(cv);
	listDestroy(cv->cv_list);
	k_free(cv);
}
