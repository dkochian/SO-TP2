#include "include/syscall.h"
#include "include/semaphore.h"


sem_u_t semOpen(char * name, int value) {
	return (sem_u_t) sysSemOpen(name,value);
}

void semClose(sem_u_t sem) {
	sysSemClose(sem);
}

void semWait(sem_u_t sem) {
	sysSemWait(sem);
}

void semPost(sem_u_t sem) {
	sysSemPost(sem);
}