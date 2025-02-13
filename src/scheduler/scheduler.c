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
#include <system/shared/device/device.h>
#include <system/shared/registers.h>

HIDDEN scheduler_t *scheduler;

/**
 * @brief Rimuove il processo come processo corrente, altrimenti dalla wait queue, altrimenti ancora se non è presente lo rimuove dalla ready queue
 * @PostCondition Se viene rimosso dalla wait queue di un semaforo, incrementa il valore del semaforo stesso, quindi il suo p_semkey diventa NULL
 * 
 * @param p il processo da rimuovere
 * @return int
 *          * FALSE se è p esiste in qualche coda ed è stato rimosso correttamente
 *          * TRUE se p == NULL oppure se p non è stato trovato
 */
int scheduler_RemoveProcessFromAnyQ( pcb_t *p );

void idle_entry(){ while( TRUE ) WAIT(); }

void scheduler_init() {
	HIDDEN scheduler_t scheduler_struct; /* Tutte le funzioni di questo Header fanno riferimento implicito a questa struttura */
	mkEmptyProcQ( &scheduler_struct.ready_queue );
	scheduler_struct.running_p = NULL;

	scheduler_struct.b_has_idle = FALSE;
	pcb_t *idlePcb = &scheduler_struct.idlePcb;
	pcb_init( idlePcb, TRUE );
	idlePcb->original_priority = DEFAULT_PRIORITY;
	idlePcb->priority = idlePcb->original_priority;
	SetStatus( &idlePcb->p_s, STATUS_NULL);
	EnableInterrupts( &idlePcb->p_s, TRUE);
	EnableKernelMode( &idlePcb->p_s, TRUE);
	EnableVirtualMemory( &idlePcb->p_s, FALSE);
	SetPC( &idlePcb->p_s, (memaddr)idle_entry );
	SetSP( &idlePcb->p_s, RAM_TOP );
	
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

	if( scheduler->b_has_idle ) {
		scheduler_RemoveProcessFromAnyQ( &scheduler->idlePcb );
		scheduler->b_has_idle = FALSE;
	}
	int b_should_switch = b_force_switch || scheduler->b_force_switch || scheduler->running_p == NULL;
	
	scheduler->b_force_switch = FALSE; /* resetto */

	if( scheduler->running_p == NULL && emptyProcQ( &scheduler->ready_queue ) ) { 
		/* tutti i processi potrebbero essere in attesa su una coda di qualche semaforo */

		/* se non ci sono processi in attesa su qualche semaforo dei device,
		   allora o tutti i processi sono terminati oppure può essere deadlock */
		if( !device_IsAnyProcessWaiting( NULL ) ) {
			HALT();
		}
		/* altrimenti metto nella ready queue un processo che fa solo WAIT, mettendo il sitema in stato di idle  */
		else {
			scheduler->b_force_switch = TRUE; /* preparo già per il prossimo scheduling, così in caso si sia riempita la ready queue, procede con cambiare processo */
			b_should_switch = TRUE;
			
			scheduler_AddProcess( &scheduler->idlePcb );
			scheduler->b_has_idle = TRUE;
			scheduler_StartProcessChronometer(); // giusto per non ottenere valori strani nelle tempistiche
		}
	}
	
	/* se il processo corrente è ancora tracciato, ne salvo il kernel time */
	scheduler_UpdateProcessRunningTime( TRUE );

	if( b_should_switch ) {
		scheduler_DoAging(); /* Incrementa priorità per evitare starvation dei processi */

		if( scheduler->running_p != NULL ) {
			scheduler_StateToReady(); /* rimette nella ready, ripristiando priorità */
		}
		scheduler->running_p = removeProcQ( &scheduler->ready_queue );
		scheduler_StartProcessChronometer();
		/* Rendendo questa funzione una macro diminuisce il numero di tick richiesti per la sua chiamata */
		SET_INTERVAL_TIMER( TIME_SLICE );
	}
	else {
		scheduler_StartProcessChronometer();
		// TODO: Re-impostare time slice rimanente oppure ignorare ?
	}

	LDST( &scheduler->running_p->p_s ); /* mette in esecuzione il processo */
	return -1;
}

void scheduler_UpdateContext( state_t* state ) {
	if( scheduler->running_p != NULL ){
		moveState( state, &scheduler->running_p->p_s );

		scheduler_UpdateProcessRunningTime( FALSE );
		scheduler_StartProcessChronometer();
	}
}

int scheduler_StateToReady() {
	/* ripristino priorità ed inserisco nella coda */
	if( scheduler->running_p != NULL ) {
		scheduler->running_p->priority = scheduler->running_p->original_priority;
		scheduler_AddProcess( scheduler->running_p );
		scheduler_UpdateProcessRunningTime( TRUE );
		scheduler->running_p = NULL;
		scheduler->b_force_switch = TRUE;
		return 0;
	}

	scheduler->b_force_switch = TRUE;
	return 1;
	
}

int scheduler_StateToWaiting( pcb_t *p, int* semKey ) {
	if( p == NULL )
		p = scheduler->running_p;
	if( p == NULL ) return -1;

	if( p == scheduler->running_p )
		scheduler_StateToReady();
	scheduler_RemoveProcess( p );
	int b_result = insertBlocked( semKey, p );
	return b_result;
}

int scheduler_RemoveProcessFromAnyQ( pcb_t *p ) {
	pcb_t *removed = NULL;

	if( p == scheduler->running_p ) {
		scheduler->running_p = NULL;
		scheduler->b_force_switch = TRUE;
		removed = p;
	}
	/* Rimuovo dalla wait queue V() */
	else if( p->p_semkey != NULL) {
		removed = outBlocked( p );
		if( removed != NULL ) {
			++(*p->p_semkey);
			removed->p_semkey = NULL;
		}
	}
	/* Rimuovo dalla ready queue */
	else if( removed == NULL ) {
		removed = outProcQ( &(scheduler->ready_queue), p );
	}
	return removed == NULL;
}

int scheduler_StateToTerminate( pcb_t *pid, int b_flag_terminate_progeny ) {
	if( pid == NULL ) {
		pid = scheduler->running_p;
	}
	
	if( pid == NULL ) {
		return 1;
	}
	/* Lo scollego dal padre */
	outChild( pid );
	int b_error = FALSE;
	if( b_flag_terminate_progeny ) {
		b_error = scheduler_RemoveProgeny( pid );
	}
	else {
		b_error = scheduler_RemoveProcessFromAnyQ( pid );
		if( !b_error ){
			pcb_SetChildrenParent( pid, NULL );
			if( pid != &scheduler->idlePcb )
				freePcb( pid );
		}
	}

	return b_error;
}

int scheduler_RemoveProgeny( pcb_t* p ) {
	/*
	 * asl::outChildBlocked sarebbe inefficiente - 
	 * dovrei processare se stessa, e visitare nuovamente l'intero albero di processi per fare V() e rimuovere ogni PCB
	 */

	if( p == NULL )
        return 1;
	
	LIST_HEAD( pcb_stack );
	pcb_t
		*parent = p;

    /*
	 * Ricorsivamente per ogni processo figlio dell intero albero:
     * Ogni processo rimuove da una coda i figli e li aggiunge ad uno stack di lavoro
     * e poi elimina il padre appena elaborato dallo stack per deallocarlo
     */
	
	if( scheduler_RemoveProcessFromAnyQ( parent ) ){
		return -1;
	}
	list_add( &p->p_next, &pcb_stack );
	
	while( !list_empty( &pcb_stack ) ) {
		struct list_head *it_parent = list_next( &pcb_stack );
		parent = container_of( it_parent, pcb_t, p_next );

		/* Aggiungo i figli sullo stack */
		while( !emptyChild( parent ) ){
			pcb_t* child = removeChild( parent );

			/* Devo elaborarlo qui altrimenti poco dopo lo "stacco" dalla lista in cui risiede */
			scheduler_RemoveProcessFromAnyQ( child );

			list_add( &child->p_next, &pcb_stack );
		}

		/* Elimino il genitore */
		list_del( &parent->p_next );
		freePcb( parent );
	}
	return 0;
}

pcb_t *scheduler_GetRunningProcess() {
	return scheduler->running_p;
}

void scheduler_AddProcess( pcb_t *p ) {
	insertProcQ( &scheduler->ready_queue, p );
}

int scheduler_RemoveProcess( pcb_t *p ) {
	if( scheduler->running_p == p ) { /* rimuove quello attuale */
		scheduler->running_p = NULL;
		scheduler->b_force_switch = TRUE;
		return TRUE;
	}
	else { /* rimuove dalla coda */
		return NULL != outProcQ( &(scheduler->ready_queue), p );
	}
}

void scheduler_StartProcessChronometer() {
	pcb_t *p = scheduler->running_p; /* rende più leggibile il codice */

	if( p == NULL ) return;

	unsigned int *todlo = (unsigned int *) BUS_REG_TOD_LO;

	p->chrono_start_tod = *todlo; /* il processo inizia la sua esecuzione in kernel mode */

	/* se il processo non era mai stato attivato, setta il TOD di prima attivazione */
	if ( p->first_activation_tod == 0 )
		p->first_activation_tod = p->chrono_start_tod;
}

void scheduler_UpdateProcessRunningTime( int b_isKernelTime ) {
	pcb_t *p = scheduler->running_p; /* rende più leggibile il codice */

	if( p == NULL ) return;

	unsigned int *todlo = (unsigned int *) BUS_REG_TOD_LO;

	if ( b_isKernelTime )
		p->kernel_timelapse += *todlo - p->chrono_start_tod;
	else
		p->user_timelapse += *todlo - p->chrono_start_tod;
}