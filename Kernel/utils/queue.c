#include "include/queue.h"
#include "../include/common.h"
#include "../system/include/mmu.h"

#include "../drivers/include/video.h"

typedef struct node {
	int size;
	element_t item;
	struct node *next;
	bool (*cmp) (element_t , element_t);
} node;

struct queue {
	node *head;
	node *tail;
	bool (*cmp) (element_t , element_t);
};

queue queueBuild(bool (*f) (element_t , element_t)) {
	queue q = (queue) k_malloc(sizeof(struct queue));

	if(q == NULL)
		return NULL;

	q->head = NULL;
	q->tail = NULL;
	q->cmp = f;

	return q;
}

void queueDestroy(queue q) {
    if(q->head != NULL) {
		node *current = q->head;
		node *aux = NULL;

		while(current != NULL) {
			aux = current;
			current = current->next;
			k_free(aux);
		}
	}

	k_free(q);
}

bool queuePush(queue q, element_t item) {
	node *n = k_malloc(sizeof(node));

	if(n == NULL)
		return false;

	n->item = item;
	n->next = NULL;

	if(q->head == NULL)
		q->head = n;
	else
		q->tail->next = n;
	
	q->tail = n;

	return true;
}

element_t queuePop(queue q) {
	node *aux = q->head;
	element_t item = aux->item;
	q->head = q->head->next;

	k_free(aux);
	
	return item;
}

bool queueIsEmpty(queue q) {
	return (q->head == NULL);
}

bool queueExists(queue q, element_t item) {
	node *current = q->head;

	while(current != NULL) {
		if(q->cmp(current->item, item) == true)
			return true;
		current = current->next;
	}

	return false;
}

void queuePrint(queue q) {
	node *current = q->head;

	print("---", -1);
	printNewLine();
	while(current != NULL) {
		print(current->item->name, -1);
		printNewLine();
		current = current->next;
	}
	print("---", -1);
	printNewLine();
}