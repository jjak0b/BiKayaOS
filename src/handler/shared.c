/***********************************shared.c****************************************
*                   - Bikaya OS - Phase 1.5 - Shared -
*    
*	Welcome to BiKaya OS!
*	This module handle all 
*	functions shared between architecture and related to handler module.
*	See "handler/shared.h" for further infos 
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

#include <system/shared/shared.h>
#include <handler/shared.h>
#include <scheduler/scheduler.h>

int sys2_create( state_t *child_state, int child_priority, void **child_pid ) {
  *child_pid = allocPCB(); /* il puntatore sarà nullo in caso non vi siano PCB disponibili */

  if ( *child_pid ) {
    (*child_pid)->p_s = *child_state;
    (*child_pid)->priority = child_priority;
    (*child_pid)->original_priority = child_priority;
    insertChild( scheduler_GetRunningProcess(), *child_pid );
    scheduler_AddProcess( *child_pid );
    return 0;
  }
  else
    return (-1);
}

int sys3_terminate( void *pid ) {
  if ( pid ) {
    if ( !(scheduler_FindReadyProc( pid )) )
      return (-1); /* se il PCB non esiste ritorna errore */
  }
  else
    pid = scheduler_GetRunningProcess();

  /* ogni processo figlio viene rimosso da un'eventuale queue di semaforo */
  struct list_head *child_iter;
  pcb_t *dummy;

  list_for_each( child_iter, &pid->p_child ) {
    dummy = container_of( child_iter, pcb_t, p_sib );
    int *key = dummy->p_semkey;
    if ( !(key) ) {
      list_del( &dummy->p_next ); /* rimozione del PCB dalla coda dei processi bloccati */

      /* aumento del valore del semaforo. se positivo si risveglia il primo in coda */
      *key += 1;
      if ( *key > 0 )
        scheduler_AddProcess( removeBlocked( key ) );
    }
  }

  scheduler_StateToTerminate( TRUE );
  scheduler_StateToRunning();

  return 0;
}
