#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"

#ifndef MLQ_SCHED
#define MLQ_SCHED
#endif


#define MAX_PRIO 140

int queue_empty(void);

void init_scheduler(void);
void finish_scheduler(void);

/* Get the next process from ready queue */
struct pcb_t * get_proc(int quantumn, int *slot_left);
/* Function to handle except 1 in get_proc() */
void minus_slot_except1(struct pcb_t * proc, int quantumn);
/* Put a process back to run queue */
void put_proc(struct pcb_t * proc);

/* Add a new process to ready queue */
void add_proc(struct pcb_t * proc);


#endif

