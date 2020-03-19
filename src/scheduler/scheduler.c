#include <scheduler.h>

extern struct list_head *ready_queue;

int scheduler_init() {
	initPcbs();
	/* TODO */
}

int scheduler_main() {
	/* TODO */
}

int scheduler_CreateProcess( function_t func, int priority ) {
	pcb_t* pcb = allocPcb();
	if( pcb == NULL ) {
		return -1;
	}
	/* TODO */
}

int scheduler_AddProcess( pcb_t *p ){
	insertProcQ( ready_queue, p );
}