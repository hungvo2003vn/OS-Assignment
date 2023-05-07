#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t * q) {
    if (q == NULL) return 1;
	return (q->size == 0);
}

void enqueue(struct queue_t * q, struct pcb_t * proc) {
        /* TODO: put a new process to queue [q] */

    if (q->size == MAX_QUEUE_SIZE) return;

	q->proc[q->size++] = proc;
}

struct pcb_t * dequeue(struct queue_t * q) {
	/* TODO: return a pcb whose prioprity is the highest
		* in the queue [q] and remember to remove it from q
		* */

    if (q->size == 0) return NULL;

	int max_prio = 0;
    int j;

	for (j = 1; j < q->size; j++)
	{
		if (q->proc[j]->priority > q->proc[max_prio]->priority)
			max_prio = j;
	}

	struct pcb_t *pcb_out = q->proc[max_prio];

	for (j = max_prio; j < q->size - 1; j++)
		q->proc[j] = q->proc[j+1];
	

	q->proc[q->size - 1] = NULL;
	q->size--;

	return pcb_out;
}

