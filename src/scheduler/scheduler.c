#include <scheduler.h>


int scheduler_init() {
	initPcbs();
	/* TODO */
}

int scheduler_main() {
	/* TODO */
}

int scheduler_CreateProcess( function_t func ) {
	pcb_t* pcb = allocPcb();
	if( pcb == NULL ) {
		return -1;
	}
	/* TODO */
}