/***********************************handler.c****************************************
*                   - Bikaya OS - Phase 1.5 - Handler -
*    
*	Welcome to BiKaya OS!
*	This module takes care of handle all 
*	exceptions for UARM architecture.
*	See "handler/handler.h" for further infos 
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
#include <handler/handler.h>
#include <handler/shared.h>
#include <scheduler/scheduler.h>
#include <asl/asl.h>

#include <shared/device/device.h>
#include <shared/device/terminal.h>

int get_interrupting_line(state_t *request);
int get_interrupting_device(int line);
void handle_irq_terminal(devreg_t *dev_reg);
void handle_irq_other_dev(devreg_t *dev_reg);

// Syscall-Breakpoint Handler
//---------------------------------------------------------------
void Handler_SysCall(void) {
    state_t *request    = (state_t *) SYSBK_OLDAREA;                /*Caller CPU state*/
    word cause          = CAUSE_EXCCODE_GET(request->CP15_Cause);   /*Content of cause register*/

    scheduler_UpdateContext( request ); // aggiorna il contesto del processo tracciato
    switch(cause){
        case EXC_SYSCALL:
            handle_syscall(request);
            break;
        case EXC_BREAKPOINT:
            handle_breakpoint(request);
            break;
        default: 
            PANIC();
            break;
    }
    scheduler_schedule( FALSE ); /* fin qui può essere accaduto di tutto al processo attuale, quindi "decide" lo scheduler */
}

void handle_syscall(state_t *request){
    word statusReq  = request->cpsr;

    if((statusReq & STATUS_SYS_MODE) != STATUS_SYS_MODE){ //richiesta non soddisfacibile
        PANIC(); // in futuro sarà da gestire come eccezione (trap) 
    }

    Syscaller( request, request->a1, request->a2, request->a3, request->a4, &request->v1 );
}

void handle_breakpoint( state_t *request ) {
    state_t *area = GetSpecPassup( SYS_SPECPASSUP_TYPE_SYSBK );
    if( area != NULL ) {
        LDST( area );
    }
}
//----------------------------------------------------------------

// Trap Handler
//----------------------------------------------------------------
void Handler_Trap(void){
    state_t *area = GetSpecPassup( SYS_SPECPASSUP_TYPE_PGMTRAP );
    if( area != NULL ) {
        LDST( area );
    }
    PANIC();
}
//----------------------------------------------------------------

// TLB Handler
//----------------------------------------------------------------
void Handler_TLB(void){
    state_t *area = GetSpecPassup( SYS_SPECPASSUP_TYPE_TLB );
    if( area != NULL ) {
        LDST( area );
    }
    PANIC();
}
//----------------------------------------------------------------

// Interrupt Handler
//----------------------------------------------------------------
void Handler_Interrupt(void) {	
	state_t *request    = (state_t *) INT_OLDAREA;
    word exc_cause         = CAUSE_EXCCODE_GET(request->CP15_Cause);

    request->pc -= WORD_SIZE;
    scheduler_UpdateContext( request ); // aggiorna il contesto del processo tracciato	
    if (ex_cause != EXC_INTERRUPT) {
        PANIC(); /*REQ ERROR*/
    }
	//-----------------------------------------------Interval Timer
    if ( CAUSE_IP_GET(request->CP15_Cause, INT_TIMER) ) {
        scheduler_schedule(TRUE);
    }
	//-------------------------------------------------------------
    
	int line;
	int dev;	
	if((line = get_interrupting_line(request))<0 || (dev = get_interrupting_device(line))<0){
        PANIC(); /*something bad happens*/
    }

    devreg_t *dev_reg       = (devreg_t *) DEV_REG_ADDR(line, dev);
    unsigned int dev_status = GET_DEV_STATUS(dev_reg,line); /*status of device*/

	if(line==INT_TERMINAL){
       handle_irq_terminal(dev_reg);
  	}else{
       handle_irq_other_dev(dev_reg);
   	}

    int *sem = device_GetSem( line, dev, GET_SEM_OFFSET(dev_reg, line) ); /*sem associated with device*/
    if(++(*sem) <= 0){ /*V on this sem*/
        pcb_t *p = removeBlocked(sem);
        if(p!=NULL){
            p->p_s.a1 = dev_status;
            /*add process to ready queue*/
            scheduler_AddProcess(p);
        }
	}
    scheduler_schedule(FALSE); 
}


int get_interrupting_line(state_t *request){
	//-------------------------------------------------Disk Devices
	if ( CAUSE_IP_GET(request->CP15_Cause, INT_DISK) ) {
		return INT_DISK;
    }
	//-------------------------------------------------Tape Devices
    if ( CAUSE_IP_GET(request->CP15_Cause, INT_TAPE) ) {
		return INT_TAPE;
    }
	//-------------------------------------------------Unused Devices
    if ( CAUSE_IP_GET(request->CP15_Cause, INT_UNUSED) ) {
		return INT_UNUSED;
    }
	//-------------------------------------------------Printer Devices
    if( CAUSE_IP_GET(request->CP15_Cause, INT_PRINTER) ) {
        return INT_PRINTER;
    }
	//-------------------------------------------------Terminal Devices
    if( CAUSE_IP_GET(request->CP15_Cause, INT_TERMINAL) ) {
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
    dev_reg->dtp.command = IS_DEV_IN_ERROR( (dev_reg->dtp.status) ) ? DEV_CMD_RESET : DEV_CMD_ACK;
}
//------------------------------------------------------------------