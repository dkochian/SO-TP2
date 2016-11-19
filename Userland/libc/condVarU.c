#include "include/syscall.h"
#include "include/condVarU.h"


cond_u_t cvInitialize() {
	return (cond_u_t) syscvInit();
}

void cvWait(cond_u_t cv, mutex m) {
	syscvWait(cv, m);
}

uint64_t cvSignal(cond_u_t cv){
	return syscvSignal(cv);
}

void cvBroadcast(cond_u_t cv) {
	syscvBroadcast(cv);
}

void cvDestroy(cond_u_t cv) {
	syscvDestroy(cv);
}