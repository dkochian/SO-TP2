#include "include/syscall.h"
#include "include/semaphore.h"


sem_u_t sem_open(char * name, int value) {
	return (sem_u_t) sysSemOpen(name,value);
}

void sem_close(sem_u_t sem) {
	sysSemClose(sem);
}

void sem_wait(sem_u_t sem) {
	sysSemWait(sem);
}

void sem_post(sem_u_t sem) {
	sysSemPost(sem);
}