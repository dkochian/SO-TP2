#ifndef _INC_LIST_

	#define _INC_LIST_

	#include "../../../include/common.h"

	typedef struct list *list;

	#include "process.h" //Do not put it over list

	typedef struct process *element_t;

	list buildList(bool (*f) (element_t , element_t));
	void destroyList(list l);
	bool add(list l, element_t item);
	bool remove(list l, element_t item);
	bool exists(list l, element_t item);
	bool isEmpty(list l);
	element_t getFirst(list l);//Gets the first element and remove it
	element_t peekFirst(list l);//Gets the first element and sends it to the bottom

#endif