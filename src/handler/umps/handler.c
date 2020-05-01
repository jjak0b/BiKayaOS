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

extern int semdev[SEM_DEV_N]; // la vera struttura dati sarà nell init.c

// Syscall-Breakpoint Handler
//---------------------------------------------------------------
void Handler_SysCall(void){
    state_t *request    = (state_t *) SYSBK_OLDAREA;            /*Caller CPU state*/
    word cause          = CAUSE_GET_EXCCODE(request->cause);    /*Content of cause register*/
    
    request->pc_epc += WORD_SIZE; //jump to next instruction

    switch(cause){
        case EXC_SYS:
            handle_syscall(request);
            break;
        case EXC_BP:
            LDST(request); /*nothing to do.. (for now)*/
        default: 
            PANIC();
    }
}

void handle_syscall(state_t *request){
    word sysReq     = request->reg_a0;
    word statusReq  = request->status;

    if(!(sysReq>0 && sysReq<12)){                   /*Syscall non valida*/
        PANIC();
    }

    if((statusReq&KERNELMODE_OFF)!=STATUS_NULL){    /*Richiesta non soddisfacibile*/
        PANIC();
    }

    switch(sysReq){
        case TERMINATEPROCESS:
            sys3_terminate();
        default: /*non dovremmo essere qui!*/
            PANIC();
    }
}
//----------------------------------------------------------------

// Trap Handler
//----------------------------------------------------------------
void Handler_Trap(void) {
    PANIC(); /*Questa eccezione è disabilitata!*/
}
//----------------------------------------------------------------

// TLB Handler
//----------------------------------------------------------------
void Handler_TLB(void) {
    PANIC(); /*Questa eccezione è disabilitata!*/
}
//----------------------------------------------------------------

// Interrupt Handler
//----------------------------------------------------------------
void Handler_Interrupt(void) {
    state_t *request    = (state_t *) INT_OLDAREA;
    word exc_cause      = CAUSE_GET_EXCCODE(request->cause);

    if(exc_cause != EXC_INT){
        PANIC(); /*REQ ERROR*/
    }

    //----------------------------------------------Inter-processor
    if(CAUSE_IP_GET(request->cause,IL_IPI)){
        PANIC(); /*NOT SUPPORTED*/
    }
    //--------------------------Processor Local Timer/Interval Timer
    if(CAUSE_IP_GET(request->cause,IL_CPUTIMER)||CAUSE_IP_GET(request->cause,IL_TIMER)){
        exit(request);
    }
    //-------------------------------------------------------------

    /*With this comment, we mean that the underlying management code
    * is related to interrupts raised on lines> = 3. For this reason,
    * it  is  also  necessary  to  identify  the specific device that 
    * generated the interrupt.
    */
    int line;
    int dev;
    if((line = get_interrupting_line(request))<0 || (dev = get_interrupting_device(line))<0){
        PANIC(); /*something bad happens*/
    }

    devreg_t *dev_reg       = (devreg_t *) DEV_REG_ADDR(line, dev);
    unsigned int dev_status = GET_DEV_STATUS(dev_reg,line); /*status of device*/

   if(line==IL_TERMINAL){
       handle_irq_terminal(dev_reg);
   }else{
       handle_irq_other_dev(dev_reg);
   }
    
    int *sem = &(semdev[GET_SEM_INDEX(dev_reg,line,dev)]); /*sem associated with device*/
    if(++(*sem) <= 0){ /*V on this sem*/
        pcb_t *p = removeBlocked(sem);
        if(p!=NULL){
            p->p_s.reg_v0 = dev_status;
            /*add process to ready queue*/
            scheduler_AddProcess(p);
        }
    }
    exit(request);
}

int get_interrupting_line(state_t *request){
    //-------------------------------------------------Disk Devices
    if (CAUSE_IP_GET(request->cause,IL_DISK)){
        return IL_DISK;
    }
    //-------------------------------------------------Tape Devices
    if (CAUSE_IP_GET(request->cause,IL_TAPE)){
        return IL_TAPE;
    }
    //----------------------------------------------Network Devices
    if (CAUSE_IP_GET(request->cause,IL_ETHERNET)){
        return IL_ETHERNET;
    }
    //----------------------------------------------Printer Devices
    if (CAUSE_IP_GET(request->cause,IL_PRINTER)){
        return IL_PRINTER;
    }
    //---------------------------------------------Terminal Devices
    if (CAUSE_IP_GET(request->cause,IL_TERMINAL)){
        return IL_TERMINAL;
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
    if(IS_TERM_READY(dev_reg->term->transm_status)){  
        /* gestione del terminale di ricezione */
        dev_reg->term->recv_command = IS_TERM_IN_ERROR(dev_reg->term->recv_status) ? RESET : ACK;
        return;
    }
    /* gestione del terminale di trasmissione */
    dev_reg->term->transm_command = IS_TERM_IN_ERROR(dev_reg->term->transm_status) ? RESET : ACK;
}

void handle_irq_other_dev(devreg_t *dev_reg){
    dev_reg->dtp->command = IS_DEV_IN_ERROR(dev_reg->dtp->status) ? RESET : ACK;
}

void exit(state_t *req){
    scheduler_StateToReady(req);    /*Add req to ready queue*/
    scheduler_StateToRunning();     /*Schedule*/    
}
//------------------------------------------------------------------
