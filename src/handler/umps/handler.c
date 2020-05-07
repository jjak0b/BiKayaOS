/***********************************handler.c****************************************
*                   - Bikaya OS - Phase 1.5 - Handler -
*    
*	Welcome to BiKaya OS!
*	This module takes care of handle all 
*	exceptions for UMPS architecture.
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

// Syscall-Breakpoint Handler
//---------------------------------------------------------------
void Handler_SysCall(void){
    state_t *request    = (state_t *) SYSBK_OLDAREA;            /*Caller CPU state*/
    word cause          = CAUSE_GET_EXCCODE(request->cause);    /*Content of cause register*/
    
    request->pc_epc += WORD_SIZE; //jump to next instruction
    scheduler_UpdateContext( request ); // aggiorno il contesto del processo tracciato    
    switch(cause){
        case EXC_SYS:
            handle_syscall(request);
            break;
        case EXC_BP:
            handle_breakpoint(request);
            break;
        default: 
            PANIC();
            break;
    }
    scheduler_schedule( FALSE ); /* fin qui può essere accaduto di tutto al processo attuale, quindi "decide" lo scheduler */
}

void handle_syscall(state_t *request){
    word statusReq  = request->status;

    if((statusReq&KERNELMODE_OFF)!=STATUS_NULL){    /*Richiesta non soddisfacibile*/
        PANIC();
    }

    Syscaller( request, request->reg_a0, request->reg_a1, request->reg_a2, request->reg_a3, &request->reg_v0 );
}

void handle_breakpoint(state_t *request) {
    state_t *area = GetSpecPassup( SYS_SPECPASSUP_TYPE_SYSBK );
    if( area != NULL ) {
        LDST( area );
    }
}
//----------------------------------------------------------------

// Trap Handler
//----------------------------------------------------------------
void Handler_Trap(void) {
    state_t *area = GetSpecPassup( SYS_SPECPASSUP_TYPE_PGMTRAP );
    if( area != NULL ) {
        LDST( area );
    }
    PANIC(); /*Questa eccezione è disabilitata!*/
}
//----------------------------------------------------------------

// TLB Handler
//----------------------------------------------------------------
void Handler_TLB(void) {
    state_t *area = GetSpecPassup( SYS_SPECPASSUP_TYPE_TLB );
    if( area != NULL ) {
        LDST( area );
    }
    PANIC(); /*Questa eccezione è disabilitata!*/
}
//----------------------------------------------------------------

// Interrupt Handler
//----------------------------------------------------------------
void Handler_Interrupt(void) {
    state_t *request    = (state_t *) INT_OLDAREA;
	word cause          = request->cause;
    word exc_cause      = CAUSE_GET_EXCCODE(request->cause);

    if(exc_cause != EXC_INT){
        PANIC(); /*REQ ERROR*/
    }

    //----------------------------------------------Inter-processor
    if(CAUSE_IP_GET(cause, IL_IPI)){
        PANIC(); /*NOT SUPPORTED*/
    }
    //-------------------------------------------------------------
    
    scheduler_UpdateContext(request); /* update context */
    
    //--------------------------Processor Local Timer/Interval Timer
    if(CAUSE_IP_GET(cause, IL_CPUTIMER)){
        scheduler_schedule(FALSE);
    }
    if(CAUSE_IP_GET(cause, IL_TIMER)){
        scheduler_schedule(TRUE);
    }
    //-------------------------------------------------------------

    /*With this comment, we mean that the underlying management code
    * is related to interrupts raised on lines> = 3. For this reason,
    * it  is  also  necessary  to  identify  the specific device that 
    * generated the interrupt.
    */
    int line;
    int dev;
    if((line = get_interrupting_line(cause))<0 || (dev = get_interrupting_device(line))<0){
        PANIC(); /*something bad happens*/
    }

    devreg_t *dev_reg       = (devreg_t *) DEV_REG_ADDR(line, dev);
    unsigned int dev_status = GET_DEV_STATUS(dev_reg,line); /*status of device*/

   if(line==IL_TERMINAL){
       handle_irq_terminal(dev_reg);
   }else{
       handle_irq_other_dev(dev_reg);
   }
    
    int *sem = device_GetSem( line, dev, GET_SEM_OFFSET(dev_reg, line) ); /*sem associated with device*/
    if(++(*sem) <= 0){ /*V on this sem*/
        pcb_t *p = removeBlocked(sem);
        if(p!=NULL){
            p->p_s.reg_v0 = dev_status;
            /*add process to ready queue*/
            scheduler_AddProcess(p);
        }
    }
    scheduler_schedule(FALSE);
}
