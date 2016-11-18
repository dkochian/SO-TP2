#ifndef _INC_SIMPLE_LIST_

	#define _INC_SIMPLE_LIST_

	#include "../../../include/common.h"

	typedef struct sList * sList;

	#include "process.h" //Do not put it over sList

	typedef struct process * element_t;

	bool isEmptyS(sList l);
	element_t getS(sList l);
	void destroyListS(sList l);
	void resetCursorS(sList l);
	element_t getFirstS(sList l);//Gets the first element and remove it
	element_t peekFirstS(sList l);//Gets the first element and sends it to the bottom
	bool addS(sList l, element_t item);
	bool removeS(sList l, element_t item);
	bool existsS(sList l, element_t item);
	bool setNextS(sList l, element_t item);
	sList buildListS(bool (*f) (element_t , element_t));
	int getSizeS(sList l);

	void printListS(sList l);
	void printSizeS(sList l);

#endif