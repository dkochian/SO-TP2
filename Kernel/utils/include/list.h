#ifndef _INC_LIST_

	#define _INC_LIST_

	#include "../../include/common.h"

	typedef struct list *list;

	#include "../../system/scheduler/include/process.h" //Do not put it over list

	typedef struct process *element_t;

	bool listIsEmpty(list l);
	element_t listGet(list l);
	void listDestroy(list l);
	void listResetCursor(list l);
	element_t listGetFirst(list l);//Gets the first element and remove it
	element_t listPeekFirst(list l);//Gets the first element and sends it to the bottom
	bool listAdd(list l, element_t item);
	bool listRemove(list l, element_t item);
	bool listExists(list l, element_t item);
	list listBuild(bool (*f) (element_t , element_t));
	int listGetSize(list l);
	void listPrint(list l);
	void listPrintSize(list l);
#endif