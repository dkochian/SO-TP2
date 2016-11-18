#include "include/simpleProcessList.h"
#include "../system/include/mmu.h"
#include "../include/common.h"

#include "../../drivers/include/video.h"

typedef struct node {
	element_t	item;
	struct node *next;
} node;

struct sList {
	node *head;
	node *tail;
	node *cursor;
	size_t size;
	bool (*cmp) (element_t , element_t);
};

sList buildListS(bool (*f) (element_t , element_t)) {
	sList l = (sList) k_malloc(sizeof(struct sList));

	if(l == NULL)
		return NULL;

	l->cmp = f;
	l->head = NULL;
	l->tail = NULL;
	l->cursor = NULL;
	l->size = 0;

	return l;
}

void destroyListS(sList l) {
	node *current = l->head;
	node *tmp = NULL;

	while(current != NULL) {
		tmp = current;
		current = current->next;
		k_free(tmp);
	}

	k_free(l);
}

bool addS(sList l, element_t item) {
	node *n = (node *) k_malloc(sizeof(node));

	if(n == NULL)
		return false;

	n->item = item;
	n->next = NULL;

	if(l->head == NULL) {
		l->head = n;
		l->tail = n;
	}
	else {
		l->tail->next = n;
		l->tail = n;
	}

	l->size++;

	return true;
}

bool removeS(sList l, element_t item) {
	bool found = false;
	node *current;
	node *prev;

	current = l->head;
	prev = NULL;
	while(current != NULL) {
		if(l->cmp(current->item, item) == true) {
			if(prev == NULL)
				l->head = current->next;
			else
				prev->next = current->next;

			if(current == l->tail)
				l->tail = prev;

			k_free(current);

			found = true;
			l->size--;
			break;
		}

		prev = current;
		current = current->next;
	}

	return found;
}

bool existsS(sList l, element_t item) {
	node *current;

	current = l->head;
	while(current != NULL) {
		if(l->cmp(current->item, item) == true)
			return true;

		current = current->next;
	}

	return false;
}

bool isEmptyS(sList l) {
	return (l->size == 0);
}

element_t getFirstS(sList l) {
	node *tmp = l->head;
	element_t item = tmp->item;
	l->head = l->head->next;

	//k_free(tmp);
	l->size --;

	return item;
}

element_t peekFirstS(sList l) {
	if(l->head == l->tail)
		return l->head->item;

	node *tmp = l->head;
	element_t item = tmp->item;

	l->head = l->head->next;
	l->tail->next = tmp;
	l->tail = tmp;
	
	tmp->next = NULL;

	return item;
}

bool setNextS(sList l, element_t item) {
	node *current;
	node *prev;

	current = l->head;
	prev = NULL;

	while(current != NULL) {
		if(l->cmp(current->item, item) == true) {
			if(prev != NULL)
				prev->next = current->next;

			break;
		}

		prev = current;
		current = current->next;
	}

	if(current == NULL)
		return false;

	current->next = l->head->next;
	l->head->next = current;

	return true;
}

element_t getS(sList l) {
	if(l->cursor == NULL)
		l->cursor = l->head;
	else
		l->cursor = l->cursor->next;

	if(l->cursor == NULL)
		return NULL;

	return l->cursor->item;
}

void resetCursorS(sList l) {
	l->cursor = NULL;
}

int getSizeS(sList l) {
	return l->size;
}

void printListS(sList l) {
	int index = 1;
	node *current = l->head;

	while(current != NULL) {
		printDec(index++, -1);
		print(": ", -1);
		print(current->item->name, -1);
		print(" (id: ", -1);
		printDec(current->item->id, -1);
		print(")", -1);
		printNewline();

		current = current->next;
	}
}

void printSizeS(sList l) {
	print("sList size: ", -1);
	printDec(l->size, -1);
	printNewline();
}