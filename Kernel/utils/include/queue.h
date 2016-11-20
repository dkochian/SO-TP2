#ifndef _INC_QUEUE_
	#define _INC_QUEUE_

	#include "../include/common.h"
	#include "../../system/scheduler/include/process.h"

	typedef struct queue *queue;
	typedef process *element_t;	

	queue queueBuild(bool (*f) (element_t , element_t));
	void queueDestroy(queue q);
	bool queuePush(queue q, element_t item);
	element_t queuePop(queue q);
	bool queueIsEmpty(queue q);
	bool queueExists(queue q, element_t item);

	void queuePrint(queue q);
	
#endif