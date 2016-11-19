#include "include/processList.h"
#include "../system/include/mmu.h"
#include "../include/common.h"
#include "../../include/mutex.h"

#include "../../drivers/include/video.h"

typedef struct node {
	element_t	item;
	struct node *next;
} node;

struct list {
	mutex* m;
	node *head;
	node *tail;
	node *cursor;
	size_t size;
	bool (*cmp) (element_t , element_t);
};

list buildList(bool (*f) (element_t , element_t)) {
	list l = (list) k_malloc(sizeof(struct list));

	if(l == NULL)
		return NULL;

	l->m = initLock();
	if(l->m == NULL) {
		k_free(l);
		return NULL;
	}
	lock(l->m);
	l->cmp = f;
	l->head = NULL;
	l->tail = NULL;
	l->cursor = NULL;
	l->size = 0;
	unlock(l->m);
	return l;
}

void destroyList(list l) {
	mutex* tempLock = l->m;
	lock(tempLock);
	node *current = l->head;
	node *tmp = NULL;

	while(current != NULL) {
		tmp = current;
		current = current->next;
		k_free(tmp);
	}

	k_free(l);
	unlock(tempLock); 
	destroyLock(tempLock);
}

bool add(list l, element_t item) {
	node *n = (node *) k_malloc(sizeof(node));

	if(n == NULL)
		return false;

	n->item = item;
	n->next = NULL;
	lock(l->m);
	if(l->head == NULL) {
		l->head = n;
		l->tail = n;
	}
	else {
		l->tail->next = n;
		l->tail = n;
	}

	l->size++;
	unlock(l->m);
	return true;
}

bool remove(list l, element_t item) {
	bool found = false;
	node *current;
	node *prev;
	lock(l->m);
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
	unlock(l->m);
	return found;
}

bool exists(list l, element_t item) {
	node *current;
	lock(l->m);
	current = l->head;
	while(current != NULL) {
		if(l->cmp(current->item, item) == true) {
			unlock(l->m);
			return true;
		}

		current = current->next;
	}
	unlock(l->m);
	return false;
}

bool isEmpty(list l) {
	return (l->size == 0);
}

element_t getFirst(list l) {
	lock(l->m);
	node *tmp = l->head;
	element_t item = tmp->item;
	l->head = l->head->next;

	k_free(tmp);
	l->size --;
	unlock(l->m);
	return item;
}

element_t peekFirst(list l) {
	lock(l->m);
	if(l->head == l->tail) {
		element_t out = l->head->item;
		unlock(l->m);
		return out;
	}

	node *tmp = l->head;
	element_t item = tmp->item;

	l->head = l->head->next;
	l->tail->next = tmp;
	l->tail = tmp;
	
	tmp->next = NULL;
	unlock(l->m);
	return item;
}

bool setNext(list l, element_t item) {
	node *current;
	node *prev;
	lock(l->m);
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

	if(current == NULL) {
		unlock(l->m);
		return false;
	}

	current->next = l->head->next;
	l->head->next = current;
	unlock(l->m);
	return true;
}

element_t get(list l) {
	lock(l->m);
	if(l->cursor == NULL)
		l->cursor = l->head;
	else
		l->cursor = l->cursor->next;

	if(l->cursor == NULL) {
		unlock(l->m);
		return NULL;
	}		
	element_t out = l->cursor->item;
	unlock(l->m);
	return out;
}

void resetCursor(list l) {
	lock(l->m);
	l->cursor = NULL;
	unlock(l->m);
}

int getSize(list l) {
	lock(l->m);
	int out = l->size;
	unlock(l->m);
	return out;
}

void printList(list l) {
	int index = 1;
	lock(l->m);
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
	unlock(l->m);
}

void printSize(list l) {
	print("list size: ", -1);
	printDec(getSize(l), -1);
	printNewline();
}