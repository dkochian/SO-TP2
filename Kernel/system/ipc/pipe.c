#include "include/pipe.h"
#include "include/mutex.h"
#include "../include/mmu.h"
#include "include/semaphore.h"
#include "../../include/common.h"
#include "../../utils/include/lib.h"

#include "../../drivers/include/video.h"

typedef struct pipe {
	char *buffer;
	semaphore rSem;
	semaphore wSem;
	int readIndex;
	int writeIndex;
	int id;
} pipe;

static void freePipeId(uint64_t id);
static pipe *getPipeFromId(uint64_t id);
static bool getFreePipeId(pipe *p);

static mutex p_mutex;
static pipe *id_table[MAX_PIPES];

bool pipeInit() {
	p_mutex = lockBuild();

	if(p_mutex == NULL)
		return false;

	for(uint64_t i=0; i<MAX_PIPES; i++)
		id_table[i] = NULL;

	return true;
}

uint64_t pipeBuild() {
	pipe *p = k_malloc(sizeof(pipe));

	if(p == NULL)
		return INVALID_PIPE_ID;

	p->buffer = k_malloc(sizeof(char)*MAX_BUFFER);

	if(p->buffer == NULL) {
		k_free(p);
		return NULL;
	}

	p->rSem = semBuild(0);
	if(p->rSem == NULL) {
		k_free(p->buffer);
		k_free(p);
		return INVALID_PIPE_ID;
	}

	p->wSem = semBuild(MAX_BUFFER);
	if(p->wSem == NULL) {
		k_free(p->buffer);
		semDestroy(p->rSem);
		k_free(p);
		return INVALID_PIPE_ID;
	}

	if(getFreePipeId(p) == false) {
		k_free(p->buffer);
		semDestroy(p->rSem);
		semDestroy(p->wSem);
		k_free(p);
		return INVALID_PIPE_ID;
	}

	p->readIndex = 0;
	p->writeIndex = 0;

	memset(p->buffer, EMPTY, MAX_BUFFER);

	return p->id;
}

void pipeDestroy(uint64_t id) {
	pipe *p = getPipeFromId(id);
	if(p == NULL)
		return;

	freePipeId(p->id);
	k_free(p->buffer);
	semDestroy(p->rSem);
	semDestroy(p->wSem);
	k_free(p);
}

char pipeRead(uint64_t id) {
	pipe *p = getPipeFromId(id);
	if(p == NULL)
		return EMPTY;

	char c;
	semWait(p->rSem);
	print("[read] buffer: ", -1);
	print(p->buffer, -1);
	printNewLine();
	c = p->buffer[p->readIndex++];
	semPost(p->wSem);

	if(p->readIndex == MAX_BUFFER - 1)
		p->readIndex = 0;

	return c;
}

bool pipeWrite(uint64_t id, char c) {
	pipe *p = getPipeFromId(id);
	if(p == NULL)
		return false;

	semWait(p->wSem);
	p->buffer[p->writeIndex++] = c;
	print("[write] buffer: ", -1);
	print(p->buffer, -1);
	printNewLine();
	semPost(p->rSem);

	if(p->writeIndex < MAX_BUFFER - 1)
		p->writeIndex = 0;

	return true;
}

static pipe *getPipeFromId(uint64_t id) {
	pipe *p;
	lock(p_mutex);
	p = id_table[id];
	unlock(p_mutex);
	return p;
}

static bool getFreePipeId(pipe *p) {
	uint64_t i;
	bool found = false;
	lock(p_mutex);
	for(i=1; i<MAX_PIPES && found == false; i++) {
		if(id_table[i] == NULL) {
			p->id = i;
			id_table[i] = p;
			found = true;
		}
	}

	unlock(p_mutex);
	return found;
}

static void freePipeId(uint64_t id) {
	lock(p_mutex);
	id_table[id] = NULL;
	unlock(p_mutex);
}