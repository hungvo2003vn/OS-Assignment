
#include "queue.h"
#include "sched.h"
#include <pthread.h>

#include <stdlib.h>
#include <stdio.h>
static struct queue_t ready_queue;
static struct queue_t run_queue;
static pthread_mutex_t queue_lock;
//static int sig = 0;
#ifdef MLQ_SCHED
static struct queue_t mlq_ready_queue[MAX_PRIO];
#endif

int queue_empty(void) {
#ifdef MLQ_SCHED
	unsigned long prio;
	for (prio = 0; prio < MAX_PRIO; prio++)
		if(!empty(&mlq_ready_queue[prio])) 
			return -1;
#endif
	return (empty(&ready_queue) && empty(&run_queue));
}

void init_scheduler(void) {
#ifdef MLQ_SCHED
    int i ;

	for (i = 0; i < MAX_PRIO; i++)
	{
		mlq_ready_queue[i].size = 0;
		mlq_ready_queue[i].slot = MAX_PRIO - i;
	}
#endif
	ready_queue.size = 0;
	run_queue.size = 0;
	pthread_mutex_init(&queue_lock, NULL);
}

#ifdef MLQ_SCHED
/* 
 *  Stateful design for routine calling
 *  based on the priority and our MLQ policy
 *  We implement stateful here using transition technique
 *  State representation   prio = 0 .. MAX_PRIO, curr_slot = 0..(MAX_PRIO - prio)
 */
struct pcb_t * get_mlq_proc(int quantumn, int *slot_left) {
	struct pcb_t * proc = NULL;
	//the level to choose proc from
	uint32_t prio_out = 0;

	while (1)
	{
		//get process satisfies:  
		//the queue not empty && slot != 0
		if (!empty(&mlq_ready_queue[prio_out])){
			if (mlq_ready_queue[prio_out].slot != 0)
			{
				pthread_mutex_lock(&queue_lock);
				proc = dequeue(&mlq_ready_queue[prio_out]);
				//exception 1: size doesn't division by quantumn
				//handle in minus_slot_except1()
				mlq_ready_queue[prio_out].slot = mlq_ready_queue[prio_out].slot - quantumn;
				//exception 2: out-of-resource, unfinished process
				if (mlq_ready_queue[prio_out].slot < 0)	
				{
					//now slot_left is utilised
					*slot_left = mlq_ready_queue[prio_out].slot + quantumn;
					mlq_ready_queue[prio_out].slot = 0;
				}
				pthread_mutex_unlock(&queue_lock);
				
				return proc; //Sucessfully getproc
			}
		}
		//when we touch the end of MLQ
		if ((++prio_out) == MAX_PRIO)
		{
			prio_out = 0;
			//reset our slots
			int i;
			for (i = 0; i < MAX_PRIO; i++)
			{
				mlq_ready_queue[i].slot = MAX_PRIO - i;
			}
			break;
		}	
	}
	
	return NULL; //Empty-handed collection
}
void minus_slot_except1(struct pcb_t * proc, int quantumn){
	pthread_mutex_lock(&queue_lock);
	if (proc->code->size % quantumn != 0)
	{
		mlq_ready_queue[proc->prio].slot = mlq_ready_queue[proc->prio].slot + quantumn - proc->code->size % quantumn;
	}
	pthread_mutex_unlock(&queue_lock);
}

void put_mlq_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&mlq_ready_queue[proc->prio], proc);
	pthread_mutex_unlock(&queue_lock);
}

void add_mlq_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&mlq_ready_queue[proc->prio], proc);
	pthread_mutex_unlock(&queue_lock);	
}

struct pcb_t * get_proc(int quantumn, int *slot_left) {
	return get_mlq_proc(quantumn, slot_left);
}

void put_proc(struct pcb_t * proc) {
	return put_mlq_proc(proc);
}

void add_proc(struct pcb_t * proc) {
	return add_mlq_proc(proc);
}
#else
struct pcb_t * get_proc(void) {
	struct pcb_t * proc = NULL;
	/*TODO: get a process from [ready_queue].
	 * Remember to use lock to protect the queue.
	 * */
	pthread_mutex_lock(&queue_lock);

	if (empty(&ready_queue)){
		while (!empty(&run_queue))
		{
			//move run_queue process to ready_queue
			enqueue(&ready_queue, dequeue(&run_queue));
		}
	}
	if (!empty(&ready_queue))
		//get the proc from ready_queue
		proc = dequeue(&ready_queue);

	pthread_mutex_unlock(&queue_lock);
	
	return proc;
}

void put_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&run_queue, proc);
	pthread_mutex_unlock(&queue_lock);
}

void add_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&ready_queue, proc);
	pthread_mutex_unlock(&queue_lock);	
}
#endif
