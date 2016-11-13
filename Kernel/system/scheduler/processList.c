#include "include/processList.h"
#include "../system/include/mmu.h"

#include "../../drivers/include/video.h"

typedef struct node {
	element_t	item;
	struct node *prev;
} node;

struct list {
	node *head;
	node *tail;
	int size;
	bool (*cmp) (element_t , element_t);
};

list buildList(bool (*f) (element_t , element_t)) {
	list l = (list) k_malloc(sizeof(struct list));

	if(l == NULL)
		return NULL;

	l->cmp = f;

	return l;
}

void destroyList(list l) {
	node *current = l->head;
	node *tmp = NULL;

	while(current != NULL) {
		tmp = current;
		current = current->prev;
		k_free(tmp);
	}

	k_free(l);
}

bool add(list l, element_t item) {
	node *n = (node *) k_malloc(sizeof(node));

	if(n == NULL)
		return false;

	n->item = item;
	n->prev = NULL;

	if(l->head == NULL) {
		l->head = n;
		l->tail = n;
	}
	else {
		l->tail->prev = n;
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
				l->head = current->prev;
			else
				prev->prev = current->prev;

			if(current == l->tail)
				l->tail = prev;

			k_free(current);

			found = true;
			l->size--;
			break;
		}

		prev = current;
		current = current->prev;
	}

	return found;
}

bool exists(list l, element_t item) {
	node *current;

	current = l->head;
	while(current != NULL) {
		if(l->cmp(current->item, item) == true)
			return true;

		current = current->prev;
	}

	return false;
}

bool isEmpty(list l) {
	return (l->size == 0);
}

element_t getFirst(list l) {
	node *tmp = l->head;
	element_t item = tmp->item;
	l->head = l->head->prev;

	k_free(tmp);
	l->size --;

	return item;
}

element_t peekFirst(list l) {
	if(l->head == l->tail)
		return l->head->item;

	node *tmp = l->head;
	element_t item = tmp->item;

	l->head = l->head->prev;
	l->tail->prev = tmp;
	l->tail = tmp;
	
	tmp->prev = NULL;

	return item;
}