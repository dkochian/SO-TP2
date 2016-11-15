#include "include/processList.h"
#include "../system/include/mmu.h"
#include "../include/common.h"

#include "../../drivers/include/video.h"

typedef struct node {
	element_t	item;
	struct node *next;
} node;

struct list {
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

	l->cmp = f;
	l->head = NULL;
	l->tail = NULL;
	l->cursor = NULL;
	l->size = 0;

	return l;
}

void destroyList(list l) {
	node *current = l->head;
	node *tmp = NULL;

	while(current != NULL) {
		tmp = current;
		current = current->next;
		k_free(tmp);
	}

	k_free(l);
}

bool add(list l, element_t item) {
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

bool remove(list l, element_t item) {
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

bool exists(list l, element_t item) {
	node *current;

	current = l->head;
	while(current != NULL) {
		if(l->cmp(current->item, item) == true)
			return true;

		current = current->next;
	}

	return false;
}

bool isEmpty(list l) {
	return (l->size == 0);
}

element_t getFirst(list l) {
	node *tmp = l->head;
	element_t item = tmp->item;
	l->head = l->head->next;

	k_free(tmp);
	l->size --;

	return item;
}

element_t peekFirst(list l) {
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

bool setNext(list l, element_t item) {
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

element_t get(list l) {
	if(l->cursor == NULL)
		l->cursor = l->head;
	else
		l->cursor = l->cursor->next;

	if(l->cursor == NULL)
		return NULL;

	return l->cursor->item;
}

void resetCursor(list l) {
	l->cursor = NULL;
}