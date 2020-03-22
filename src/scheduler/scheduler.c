#include <scheduler.h>
#include <system/shared/shared.h>
#include <utilities/types.h>
#include <pcb/utils.h>

static scheduler_t *scheduler; // non dovrebbe essere definito static

void scheduler_init() {
	static scheduler_t scheduler_struct; /* Tutte le funzioni di questo Header fanno riferimento implicito a questa struttura */
	mkEmptyProcQ( &scheduler_struct.ready_queue );
	scheduler_struct.running_p = NULL;
	scheduler = &scheduler_struct;
}

int scheduler_main() {
	return scheduler_StateToRunning();
}

void scheduler_DoAging() {
	struct list_head *it;
	pcb_t *dummy;
	if( !emptyProcQ( &scheduler->ready_queue ) ) {
		list_for_each(it, &scheduler->ready_queue ) {
			dummy = headProcQ( it );
			dummy->priority += 1;
		}
	}
}

int scheduler_StateToRunning(){
	if( emptyProcQ( &scheduler->ready_queue ) ) {
		return 1;
	}
	scheduler->running_p = removeProcQ( &scheduler->ready_queue );

	setTIMER( TIME_SLICE );
	LDST( &scheduler->running_p->p_s );
	return -1;
}

int scheduler_StateToReady( state_t* state ) {
	if( scheduler->running_p == NULL ){
		return 1;
	}

	scheduler_DoAging(); /* Incrementa priorità per evitare starvation dei processi */

	moveState( state, &scheduler->running_p->p_s ); /* aggiorno lo stato del pcb con lo stato del processore fornito */

	/* ripristino priorita ed inserisco nella coda */
	scheduler->running_p->priority = scheduler->running_p->original_priority;
	scheduler_AddProcess( scheduler->running_p );
	scheduler->running_p = NULL;

	return 0;
}

int scheduler_StateToWaiting() {
	/* TODO */
	return 0;
}

int scheduler_StateToTerminate( int b_flag_terminate_progeny ) {
	if( scheduler->running_p == NULL ){
		return 1;
	}

	if( b_flag_terminate_progeny ) {
		scheduler_RemoveProgeny( scheduler->running_p );
	}
	else {
		pcb_SetChildrenParent( scheduler->running_p, NULL );
		freePcb( scheduler->running_p );
		scheduler->running_p = NULL;
	}

	return 0;
}

int scheduler_RemoveProgeny( pcb_t* p ){
	if( p == NULL ){
		return 1;
	}
	else if( scheduler->running_p == p ){
		scheduler->running_p = NULL; /* Rimuovo il tracciante di questo descrittore attivo */
	}

	pcb_RemoveProgenyQ( p, &scheduler->ready_queue );

	return 0;
}

int scheduler_CreateProcess( function_t func, int priority ) {
	pcb_t* pcb = allocPcb();
	if( pcb == NULL ) {
		return -1;
	}
	SetPC( &pcb->p_s, (memaddr)func );
	/* TODO: SetSP( ); SP dovrebbe essere dinamicamente in base al gestore della memoria */
	/* TODO: flag status, ecc ... */
	EnableKernelMode( &pcb->p_s, FALSE );

	scheduler_AddProcess( pcb );
	return 0;
}

void scheduler_AddProcess( pcb_t *p ) {
	insertProcQ( &scheduler->ready_queue, p );
}