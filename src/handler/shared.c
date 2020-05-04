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
#include <asl/asl.h>
#include <pcb/pcb.h>
#include <system/shared/device/device.h>

HIDDEN state_t specPassup[3]; /* stati del processore dedicati a handler di livello superiore specifici */
HIDDEN byte bitmap_specPassup; /* bitmap con i flag settati sulle i-esime posizioni se la specPassup[ i ] è assegnata */

void SpecPassup_init() {
    bitmap_specPassup = 0;
}

int IsSetSpecPassup( int type ) {
    byte mask_specPassup_type = 1 << type;
    return (bitmap_specPassup & mask_specPassup_type) == mask_specPassup_type;
}

int SetSpecPassup( int type, state_t *area ) {
    if( area != NULL && ( type >= 0 && type <= 2 ) && !IsSetSpecPassup( type ) ) {
        /* copia la area corrispondente e imposta l'i-esmi bit della bitmap relativo al tipo fornito*/
        moveState(area, &specPassup[ type ]);
        bitmap_specPassup |= (1 << type);
        return TRUE;
    }
    return FALSE;
}

state_t *GetSpecPassup( int type ) {
    if( IsSetSpecPassup( type ) ) {
        return &specPassup[ type ];
    }
    return NULL;
}

word Syscaller( state_t *state, word sysNo, word param1, word param2, word param3, word *returnValue ) {
    int b_hasReturnValue = FALSE;
    switch( sysNo ) {
        case GETCPUTIME:
            Sys1_GetCPUTime( state, (word*)param1, (word*)param2, (word*)param3 );
            break;
        case CREATEPROCESS:
            b_hasReturnValue = TRUE;
            *returnValue = (int) Sys2_CreateProcess( (state_t*)param1, (int)param2, (pcb_t**)param3 );
            break;
        case TERMINATEPROCESS:
            b_hasReturnValue = TRUE;
            *returnValue = (int) Sys3_TerminateProcess( (void*) param1 );
            break;
        case VERHOGEN:
            Sys4_Verhogen( (int*)param1 );
            break;
        case PASSEREN:
            Sys5_Passeren( (int*)param1 );
            break;
        case WAITIO:
            Sys6_DoIO( param1, (word*)param2, (int)param3 );
            break;
        case SPECPASSUP:
            b_hasReturnValue = TRUE;
            *returnValue = (int) Sys7_SpecPassup( state, (int)param1, (state_t*)param2, (state_t*)param3 );
            break;
        case GETPID:
            b_hasReturnValue = TRUE;
            *returnValue = (int) Sys8_GetPID( (void**)param1, (void**)param2 );
            break;
        default: {
            state_t *area = GetSpecPassup( SYS_SPECPASSUP_TYPE_SYSBK );
            if( area != NULL ) {
                LDST( area );
            }
            else {
                b_hasReturnValue = TRUE;
                *returnValue = -1;
                scheduler_StateToTerminate( FALSE );
            }
            break;
        }
    }

    return b_hasReturnValue;
}

void Sys1_GetCPUTime( state_t* currState, word *user, word *kernel, word *wallclock ) {
    // TODO
}

int Sys2_CreateProcess( state_t *child_state, int child_priority, pcb_t **child_pid ) {
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

int Sys3_TerminateProcess( pcb_t *pid ) {
    scheduler_t *s = getScheduler();

    if ( pid ) {
        if ( !(outProcQ( &s->ready_queue, pid )) )
            return (-1); /* se il PCB non esiste ritorna errore */
    }
    else
        pid = scheduler_GetRunningProcess();

    /* ogni processo figlio viene rimosso da un'eventuale coda di blocco su semaforo */
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

    /* se il processo da terminare è quello corrente, si passa il lavoro allo scheduler;
    altrimenti il relativo pcb viene tolto dalla ready queue e inserito nella lista free */
    if ( pid == scheduler_GetRunningProcess() ) {
        scheduler_UpdateContext( &pid->p_s );
        scheduler_StateToTerminate( TRUE );
        scheduler_schedule( TRUE );
    }
    else {
        freePcb( outProcQ( &s->ready_queue, pid ) );
        return 0;
    }
}

void Sys4_Verhogen( int* semaddr ) {
    pcb_t *blocked = removeBlocked( semaddr );
    if( blocked != NULL ) {
        /* dovremmo ripristinare la priorità ? */
        scheduler_AddProcess( blocked );
    }
    else {
        (*semaddr) ++;
    }
}

void Sys5_Passeren( int* semaddr ) {
    if( *semaddr <= 0 ) {
        int b_result = scheduler_StateToWaiting( semaddr );    
        // nel caso ritornasse 1 non può accadere perchè ad ogni processo può essere nella ASL al massimo 1 volta
        // infatti se non fosse disponibile un semd, non esisterebbe neanche questo processo
        if( b_result == 1 ) scheduler_StateToTerminate( FALSE );
        // sia in caso ritorni 0 o -1 lo scheduler deve proseguire
    }
    else {
        (*semaddr) --;
    }
}

void Sys6_DoIO( word command, word *devregAddr, int subdevice ) {
    devreg_t * devreg = (devreg_t*)devregAddr;
    int devLine, devNo;
    device_GetInfo( devreg, &devLine, &devNo );

    int *semKey = device_GetSem( devLine, devNo, subdevice );
    if( *semKey > 0 ) {
        --(*semKey);
    }
    if( devLine == IL_TERMINAL ) {
        if( subdevice ) {
            devreg->term.transm_command = command;
        }
        else {
            devreg->term.recv_command = command;
        }
    }
    else {
        devreg->dtp.command = command;
    }
    scheduler_StateToWaiting( semKey );
}

int Sys7_SpecPassup( state_t* currState, int type, state_t *old_area, state_t *new_area ) {
    if( !SetSpecPassup( type, new_area ) ) {
       moveState( currState, old_area );
       return 0;
    }

    scheduler_StateToTerminate( FALSE );
    return -1;
}

int Sys8_GetPID( void **pid, void **ppid ) {
    // TODO
    return -1;
}
