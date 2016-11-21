//http://www.cs.wustl.edu/~schmidt/win32-cv-1.html

#include "../include/mmu.h"
#include "include/condVar.h"
#include "../../drivers/include/video.h"
#include "../scheduler/include/scheduler.h"


cond_t cvInitialize() {
	cond_t cv = listBuild(&equal);
	if (cv == NULL){
		return NULL;
	}

    return cv;
}

void cvWait(cond_t cv, mutex cv_mutex) {	

	process *cProcess = getCurrentProcess();
	listAdd(cv, cProcess);

	blockProcess(cProcess);  
	unlock(cv_mutex);
	_yield();
	
	lock(cv_mutex);
}

void cvSignal(cond_t cv) {
	process *rProcess;
	if (listIsEmpty(cv) == false) {
		rProcess = listGetFirst(cv);
		unBlockProcess(rProcess);
	}
}

void cvBroadcast(cond_t cv) {
	while (listIsEmpty(cv) == false) {
		process *rProcess = listGetFirst(cv);
		unBlockProcess(rProcess);
	}
}

void cvDestroy(cond_t cv) {
	cvBroadcast(cv);
	listDestroy(cv);
}
