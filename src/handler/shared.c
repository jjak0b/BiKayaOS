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
#include <utilities/semaphore.h>
#include <shared/device/terminal.h>

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
            *returnValue = (int) Sys8_GetPID( (pcb_t**)param1, (pcb_t**)param2 );
            break;
        default: {
            state_t *area = GetSpecPassup( SYS_SPECPASSUP_TYPE_SYSBK );
            if( area != NULL ) {
                LDST( area );
            }
            else {
                b_hasReturnValue = TRUE;
                *returnValue = -1;
                scheduler_StateToTerminate( NULL, FALSE );
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
    /* prima di eseguire viene controllata la validità dei puntatori forniti */
    if ( child_state && child_pid ) {
        *child_pid = allocPcb(); /* il puntatore sarà nullo in caso non vi siano PCB disponibili */

        if ( *child_pid ) {
            moveState( child_state, &(*child_pid)->p_s );
            (*child_pid)->priority = child_priority;
            (*child_pid)->original_priority = child_priority;
            insertChild( scheduler_GetRunningProcess(), *child_pid );
            scheduler_AddProcess( *child_pid );
            return 0;
        }
        else
            return (-1);
    }
    else
        return (-1);
}

int Sys3_TerminateProcess( pcb_t *pid ) {
    int b_error = scheduler_StateToTerminate( pid, TRUE );
    return ( b_error ? -1 : 0 );
}

void Sys4_Verhogen( int* semaddr ) {
    semaphore_V( semaddr );
    /* dovremmo ripristinare la priorità ? */
}

void Sys5_Passeren( int* semaddr ) {
    pcb_t *pid = scheduler_GetRunningProcess();
    int status = semaphore_P( semaddr, pid );
    // nel caso ritornasse 1 non può accadere perchè ad ogni processo può essere nella ASL al massimo 1 volta
    // infatti se non fosse disponibile un semd, non esisterebbe neanche questo processo
    if( status == 1 ) {
        scheduler_StateToTerminate( pid, FALSE );
    }
}

void Sys6_DoIO( word command, word *devregAddr, int subdevice ) {
    devreg_t * devreg = (devreg_t*)devregAddr;
    int devLine, devNo;
    device_GetInfo( devreg, &devLine, &devNo );

    int *semKey = device_GetSem( devLine, devNo, subdevice );
    pcb_t *pid = scheduler_GetRunningProcess();
    // un semaforo di un device è sempre almeno inizializzato a 0, e quindi è sempre sospeso
    int b_error = semaphore_P( semKey, pid );
    if( b_error ) {
        scheduler_StateToTerminate( pid, FALSE );
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
}

int Sys7_SpecPassup( state_t* currState, int type, state_t *old_area, state_t *new_area ) {
    if( !SetSpecPassup( type, new_area ) ) {
       moveState( currState, old_area );
       return 0;
    }

    scheduler_StateToTerminate( NULL, FALSE );
    return -1;
}

int Sys8_GetPID( pcb_t **pid, pcb_t **ppid ) {
    /* la procedura ritorna errore in caso di puntatori non validi */
    if ( pid && ppid ) {
        *pid = scheduler_GetRunningProcess();

        if ( *pid )
             *ppid = (*pid)->p_parent;
        else
            return (-1);
        
        return 0;
    }

    return (-1);
}

int get_interrupting_line(word cause){
	//-------------------------------------------------Disk Devices
	if ( CAUSE_IP_GET(cause, INT_DISK) ) {
		return INT_DISK;
    }
	//-------------------------------------------------Tape Devices
    if ( CAUSE_IP_GET(cause, INT_TAPE) ) {
		return INT_TAPE;
    }
	//-------------------------------------------------Unused Devices
    if ( CAUSE_IP_GET(cause, INT_UNUSED) ) {
		return INT_UNUSED;
    }
	//-------------------------------------------------Printer Devices
    if( CAUSE_IP_GET(cause, INT_PRINTER) ) {
        return INT_PRINTER;
    }
	//-------------------------------------------------Terminal Devices
    if( CAUSE_IP_GET(cause, INT_TERMINAL) ) {
		return INT_TERMINAL;
    }
    return -1; /*error*/
}

int get_interrupting_device(int line){
	 for(int i=0;i<N_DEV_PER_IL;i++){
        if(IS_IRQ_RAISED_FROM_I(line,i)){
            return i;
        }
    }
    return -1; /*error*/
}

void handle_irq_terminal(devreg_t *dev_reg){
    if(IS_TERM_READY(dev_reg->term.transm_status)){  
        /* gestione del terminale di ricezione */
        dev_reg->term.recv_command = IS_TERM_IN_ERROR(dev_reg->term.recv_status) ? DEV_CMD_RESET : DEV_CMD_ACK;
        return;
    }
    /* gestione del terminale di trasmissione */
    dev_reg->term.transm_command = IS_TERM_IN_ERROR(dev_reg->term.transm_status) ? DEV_CMD_RESET : DEV_CMD_ACK;
}

void handle_irq_other_dev(devreg_t *dev_reg){
    dev_reg->dtp.command = IS_DEV_IN_ERROR(dev_reg->dtp.status) ? DEV_CMD_RESET : DEV_CMD_ACK;
}
