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
#include <asl/asl.h>

HIDDEN scheduler_t *scheduler;

void scheduler_init() {
	HIDDEN scheduler_t scheduler_struct; /* Tutte le funzioni di questo Header fanno riferimento implicito a questa struttura */
	mkEmptyProcQ( &scheduler_struct.ready_queue );
	scheduler_struct.running_p = NULL;
	scheduler = &scheduler_struct;
}

int scheduler_main() {
	return scheduler_schedule( TRUE );
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

int scheduler_schedule( int b_force_switch ) {
	if( emptyProcQ( &scheduler->ready_queue ) ) { 
		HALT();
	}

	int b_should_switch = b_force_switch || scheduler->b_force_switch || scheduler->running_p == NULL;
	
	if( b_should_switch ) {
		scheduler_DoAging(); /* Incrementa priorità per evitare starvation dei processi */

		if( scheduler->running_p != NULL ) {
			scheduler_StateToReady(); /* rimette nella ready, ripristiando priorità */
		}
		scheduler->running_p = removeProcQ( &scheduler->ready_queue );
		/* Rendendo questa funzione una macro diminuisce il numero di tick richiesti per la sua chiamata */
		SET_INTERVAL_TIMER( TIME_SLICE );
	}
	else {
		// TODO: Re-impostare time slice rimanente oppure ignorare ?
	}

	LDST( &scheduler->running_p->p_s ); /* mette in esecuzione il processo */
	return -1;
}

void scheduler_UpdateContext( state_t* state ) {
	if( scheduler->running_p != NULL )
		moveState( state, &scheduler->running_p->p_s );
}

int scheduler_StateToReady() {
	/* ripristino priorità ed inserisco nella coda */
	if( scheduler->running_p != NULL ) {
		scheduler->running_p->priority = scheduler->running_p->original_priority;
		scheduler_AddProcess( scheduler->running_p );
		scheduler->running_p = NULL;
		scheduler->b_force_switch = TRUE;
		return 0;
	}
	scheduler->b_force_switch = TRUE;
	return 1;
	
}

int scheduler_StateToWaiting( int* semKey ) {
	pcb_t* p = scheduler->running_p;
	if( p == NULL ) {
		return -1;
	}

	int b_result = insertBlocked( semKey, p );
	if( !b_result ) {
		scheduler->running_p = NULL;
		scheduler->b_force_switch = TRUE;
		return 0;
	}
	else {
		return 1;
	}
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
		scheduler->b_force_switch = TRUE;
	}

	return 0;
}

int scheduler_RemoveProgeny( pcb_t* p ) {
	if( p == NULL ) {
		return 1;
	}
	else if( scheduler->running_p == p ) {
		scheduler->running_p = NULL; /* Rimuovo il tracciante di questo descrittore attivo */
		scheduler->b_force_switch = TRUE;
	}

	pcb_RemoveProgenyQ( p, &scheduler->ready_queue );

	return 0;
}

pcb_t *scheduler_GetRunningProcess() {
	return scheduler->running_p;
}

scheduler_t *scheduler_Get() {
	return scheduler;
}

void scheduler_AddProcess( pcb_t *p ) {
	insertProcQ( &scheduler->ready_queue, p );
}

int scheduler_FindReadyProc( pcb_t *p ) {
	struct list_head *iter;
	pcb_t *dummy;

	list_for_each( iter, &scheduler->ready_queue ) {
			dummy = container_of( iter, pcb_t, p_next );
			if ( dummy == p )
				return 1;
	}

	return 0;
}
