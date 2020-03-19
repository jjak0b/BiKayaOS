#include <scheduler.h>

static scheduler_t *scheduler;

int scheduler_init() {
	static scheduler_t scheduler_struct; /* Tutte le funzioni di questo Header fanno riferimento implicito a questa struttura */
	mkEmptyProcQ( &scheduler_struct.ready_queue );
	scheduler = &scheduler_struct;
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

int scheduler_AddProcess( pcb_t *p ) {
	insertProcQ( &scheduler->ready_queue, p );
}