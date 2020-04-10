/***********************************scheduler.c****************************************
*                   - Bikaya OS - Phase 1.5 - Scheduler -
*   
*	Welcome to BiKaya OS!
*	This module handle all 
*	functions about scheduler.
*	See "scheduler/scheduler.h" for further infos 
*	about the following functions.	
*
*	To test kernel, just read README.md 
*	Enjoy using BiKaya OS. :)
*
*	Copyright (c) 2020 lso20az15. All rights reserved.
*	This work is licensed under the terms of the MIT license.
*	For a copy, see LICENSE.
* 	 
*	@credit: 
*   Stefano De Santis, Cristiano Guidotti, Iacopo Porcedda, Jacopo Rimediotti
*/

#include <scheduler.h>
#include <system/shared/shared.h>
#include <utilities/types.h>
#include <pcb/utils.h>
#include <system/const.h>

HIDDEN scheduler_t *scheduler;

void scheduler_init() {
	HIDDEN scheduler_t scheduler_struct; /* Tutte le funzioni di questo Header fanno riferimento implicito a questa struttura */
	mkEmptyProcQ( &scheduler_struct.ready_queue );
	scheduler_struct.running_p = NULL;
	scheduler = &scheduler_struct;
}

int scheduler_main() {
	return scheduler_StateToRunning();
}

void scheduler_DoAging() {
	struct list_head *iter;
	pcb_t *dummy;
	if( !emptyProcQ( &scheduler->ready_queue ) ) {
		list_for_each(iter, &scheduler->ready_queue ) {
			dummy = container_of(iter, pcb_t, p_next);
			dummy->priority += 1;
		}
	}
}

int scheduler_StateToRunning() {
	if( emptyProcQ( &scheduler->ready_queue ) ) { 
		HALT();
	}
	scheduler->running_p = removeProcQ( &scheduler->ready_queue );
	
	/* Rendendo questa funzione una macro diminuisce il numero di tick richiesti per la sua chiamata */
	SET_INTERVAL_TIMER( TIME_SLICE );

	LDST( &scheduler->running_p->p_s ); /* mette in esecuzione il processo */
	return -1;
}

int scheduler_StateToReady( state_t* state ) {
	if( scheduler->running_p == NULL ) {
		return 1;
	}

	scheduler_DoAging(); /* Incrementa priorità per evitare starvation dei processi */

	moveState( state, &scheduler->running_p->p_s ); /* aggiorno lo stato del pcb con lo stato fornito */

	/* ripristino priorità ed inserisco nella coda */
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
	if( scheduler->running_p == NULL ) {
		return 1;
	}

	if( b_flag_terminate_progeny ) {
		/* rimuove la progenie dalla ready queue, poi dealloca e disassocia puntatore */
		scheduler_RemoveProgeny( scheduler->running_p );
	}
	else {
		pcb_SetChildrenParent( scheduler->running_p, NULL );
		freePcb( scheduler->running_p );
		scheduler->running_p = NULL;
	}

	return 0;
}

int scheduler_RemoveProgeny( pcb_t* p ) {
	if( p == NULL ) {
		return 1;
	}
	else if( scheduler->running_p == p ) {
		scheduler->running_p = NULL; /* Rimuovo il tracciante di questo descrittore attivo */
	}

	pcb_RemoveProgenyQ( p, &scheduler->ready_queue );

	return 0;
}

/* WIP */
int scheduler_CreateProcess( memaddr func, int priority ) {
	pcb_t* pcb = allocPcb();
	if( pcb == NULL ) {
		return -1;
	}
	SetPC( &pcb->p_s, (memaddr)func );
	/* TODO: SetSP( ); SP dovrebbe essere assegnato dinamicamente in base al gestore della memoria */
	/* TODO: flag status, ecc ... */
	/* i processi user dovrebbero essere sempre in user mode, quindi lo lasciamo a FALSE in attesa di più informazioni */
	EnableKernelMode( &pcb->p_s, FALSE );
	EnableInterrupts( &pcb->p_s, TRUE );

	scheduler_AddProcess( pcb );
	return 0;
}

void scheduler_AddProcess( pcb_t *p ) {
	insertProcQ( &scheduler->ready_queue, p );
}
