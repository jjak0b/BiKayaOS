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
#include <system/shared/registers.h>

word Syscaller( state_t *state, word sysNo, word param1, word param2, word param3, word *returnValue ) {
    int b_hasReturnValue = FALSE;
    switch( sysNo ) {
        case GETCPUTIME:
            Sys1_GetCPUTime( (unsigned int*)param1, (unsigned int*)param2, (unsigned int*)param3 );
            break;
        case CREATEPROCESS:
            b_hasReturnValue = TRUE;
            *returnValue = (int) Sys2_CreateProcess( (state_t*)param1, (int)param2, (pcb_t**)param3 );
            break;
        case TERMINATEPROCESS:
            b_hasReturnValue = TRUE;
            *returnValue = (int) Sys3_TerminateProcess( (pcb_t*) param1 );
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
            Sys8_GetPID( (pcb_t**)param1, (pcb_t**)param2 );
            break;
        default: {
            if( handle_specPassUp( state, SYS_SPECPASSUP_TYPE_SYSBK ) ){
                *returnValue = -1;
                b_hasReturnValue = TRUE;
                scheduler_StateToTerminate( NULL, FALSE );
            }
            break;
        }
    }

    return b_hasReturnValue;
}

int Sys1_GetCPUTime( unsigned int *user, unsigned int *kernel, unsigned int *wallclock ) {
    /* prima viene effettuato il controllo validità puntatori */
    if ( user && kernel && wallclock ) {
        scheduler_UpdateProcessRunningTime( TRUE );
        pcb_t *p = scheduler_GetRunningProcess();
        if (p == NULL)
            return (-1);
        *user = p->user_timelapse;
        *kernel = p->kernel_timelapse;
        scheduler_StartProcessChronometer();
        *wallclock = p->chrono_start_tod - p->first_activation_tod;
        return 0;
    }
    else
        return (-1);    
}

int Sys2_CreateProcess( state_t *child_state, int child_priority, pcb_t **child_pid ) {
    /* prima di eseguire viene controllata la validità dei puntatori forniti */
    if ( child_state != NULL ) {
        pcb_t *child = allocPcb(); /* il puntatore sarà nullo in caso non vi siano PCB disponibili */
        if ( child != NULL ) {
            moveState( child_state, &child->p_s );
            child->priority = child_priority;
            child->original_priority = child_priority;
            insertChild( scheduler_GetRunningProcess(), child );
            scheduler_AddProcess( child );
            if( child_pid != NULL )
                *child_pid = child;

            return 0;
        } 
    }
    return -1;
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
    if( *semKey >= 0 ){
        if( devLine == IL_TERMINAL ) {
            if( !subdevice ) {
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

    int b_error = semaphore_P( semKey, pid );
    if( b_error ) {
        scheduler_StateToTerminate( pid, FALSE );
    }
}

int Sys7_SpecPassup( state_t* currState, int type, state_t *old_area, state_t *new_area ) {
    pcb_t* p = scheduler_GetRunningProcess();
    if( p != NULL && (0 <= type && type < 3) ) {
        if( p->specPassup[ type ][ SYS_SPECPASSUP_AREA_NEW ] == NULL ) {
            p->specPassup[ type ][ SYS_SPECPASSUP_AREA_NEW ] = new_area;
            p->specPassup[ type ][ SYS_SPECPASSUP_AREA_OLD ] = old_area;
            return 0;
        }
        else {
            scheduler_StateToTerminate( p, TRUE );
        }
    }
    return -1;
}

void Sys8_GetPID( pcb_t **pid, pcb_t **ppid ) {
    /* la procedura ritorna errore in caso di puntatori non validi */
    pcb_t *p = scheduler_GetRunningProcess();
    if ( pid != NULL )
        *pid = p;

    if ( p != NULL && ppid != NULL )
        *ppid = p->p_parent;
}

int handle_specPassUp( state_t *request, int type ) {
    if( (0 <= type && type < 3) ) {
        pcb_t *p = scheduler_GetRunningProcess();
        state_t *new_area = p->specPassup[ type ][ SYS_SPECPASSUP_AREA_NEW ];
        state_t *old_area = p->specPassup[ type ][ SYS_SPECPASSUP_AREA_OLD ];
        if( p != NULL && new_area != NULL ) {
            if( old_area != NULL ) {
                moveState( request, old_area );
            }
            LDST( new_area );
        }
    }
    return -1;
}

// Interrupt Handler
//----------------------------------------------------------------
void Handle_Interrupt() {
    state_t *request    = (state_t *) INT_OLDAREA;
	word cause          = request->reg_cause;
    word exc_cause      = CAUSE_EXCCODE_GET( cause );

	if(exc_cause != EXC_INTERRUPT){
		PANIC(); /*REQ ERROR*/
	}

	//----------------Inter-processor irq/Processor Local Timer irq
	if(CAUSE_IP_GET(cause, IL_IPI) || CAUSE_IP_GET(cause, IL_CPUTIMER)){
		PANIC(); /*NOT USED*/
	}
	//-------------------------------------------------------------

	scheduler_UpdateContext(request); /* update context */
	int irq_served      = FALSE;
	int force_switch    = FALSE;
	//-------------------------------------------Interval Timer irq
	if(CAUSE_IP_GET(cause, IL_TIMER)){
		force_switch = irq_served = TRUE;
	}
	//-------------------------------------------------------------

	/*With this comment, we mean that the underlying management code
    * is related to interrupts raised on lines> = 3. For this reason,
    * it  is  also  necessary  to  identify  the specific device that 
    * generated the interrupt.
    */
	unsigned int dev_line;
	unsigned int dev;
	for(dev_line=0; dev_line<N_EXT_IL; dev_line++){
        for(dev=0; dev<N_DEV_PER_IL; dev++){
            if(IS_IRQ_RAISED_FROM_I(dev_line, dev)){
				handle_irq(dev_line+3, dev);
				irq_served = TRUE;
			}
		}
	}
	if(!irq_served){
		PANIC(); /*irq but no interrupt served?*/
	}

	scheduler_schedule(force_switch);
}

void handle_irq(unsigned int line, unsigned int dev){
    devreg_t *dev_reg   = (devreg_t *) DEV_REG_ADDR(line, dev);
    (line==IL_TERMINAL) ? handle_irq_terminal(line,dev,dev_reg) : handle_irq_other_dev(line,dev,dev_reg);
}

void handle_irq_terminal(unsigned int line, unsigned int dev, devreg_t *dev_reg){
    word trasm  = dev_reg->term.transm_status;
    word recv   = dev_reg->term.recv_status;
    if(!IS_TERM_READY(trasm)){/* gestione del terminale di trasmissione */
        dev_reg->term.transm_command = IS_TERM_IN_ERROR(trasm) ? DEV_CMD_RESET : DEV_CMD_ACK; /*notify to device*/
        notifyEvent(line,dev,0,dev_reg,trasm);
    }
    if(!IS_TERM_READY(recv)){/* gestione del terminale di ricezione */
        dev_reg->term.recv_command = IS_TERM_IN_ERROR(recv) ? DEV_CMD_RESET : DEV_CMD_ACK; /*notify to device*/
        notifyEvent(line,dev,1,dev_reg,recv);
    }
}

void handle_irq_other_dev(unsigned int line, unsigned int dev, devreg_t *dev_reg){
    word dev_s = dev_reg->dtp.status;
    dev_reg->dtp.command = IS_DEV_IN_ERROR(dev_s) ? DEV_CMD_RESET : DEV_CMD_ACK; /*notify to device*/
    notifyEvent(line,dev,0,dev_reg,dev_s);
}

void notifyEvent(unsigned int line, unsigned int dev, unsigned int subdev_trasm, devreg_t *dev_reg, word status){
    int *sem = device_GetSem(line, dev, subdev_trasm); /*sem associated with device*/
    pcb_t *p;
    if((p = semaphore_V(sem))==NULL){
        return;
    }
    p->p_s.reg_return_0 = status; /*notify to process*/
    
    /*see next process in queue*/
    if((p=headBlocked(sem))==NULL){
        return;
    }
    word new_command = p->p_s.reg_param_1;
    switch(line){
        case IL_TERMINAL:
            (!subdev_trasm) ? (dev_reg->term.transm_command = new_command) : (dev_reg->term.recv_command = new_command);
            break;
        default:
            dev_reg->dtp.command = new_command;
    }
}
//----------------------------------------------------------------

void Handle_Trap(void){
    state_t *request    = (state_t *) PGMTRAP_OLDAREA;
    if( handle_specPassUp( request, SYS_SPECPASSUP_TYPE_PGMTRAP ) ){
        scheduler_StateToTerminate( NULL, FALSE );
        scheduler_schedule( TRUE );
    }
    PANIC();
}

void Handle_TLB(void){
    state_t *request    = (state_t *) TLB_OLDAREA;
    if( handle_specPassUp( request, SYS_SPECPASSUP_TYPE_TLB ) ){
        scheduler_StateToTerminate( NULL, FALSE );
        scheduler_schedule( TRUE );
    }
    PANIC();
}

void Handle_breakpoint() {
    state_t *request    = (state_t *) SYSBK_OLDAREA;
    handle_specPassUp( request, SYS_SPECPASSUP_TYPE_SYSBK );
}