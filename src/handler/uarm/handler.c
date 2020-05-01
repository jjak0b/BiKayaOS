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

// Syscall-Breakpoint Handler
//---------------------------------------------------------------
void Handler_SysCall(void){
    state_t *request    = (state_t *) SYSBK_OLDAREA;                /*Caller CPU state*/
    word cause          = CAUSE_EXCCODE_GET(request->CP15_Cause);   /*Content of cause register*/

    switch(cause){
        case EXC_SYSCALL:
            handle_syscall(request);
            break;
        case EXC_BREAKPOINT:
        default: 
            PANIC();
            break;
    }
}

void handle_syscall(state_t *request){
    word sysReq     = request->a1;
    word statusReq  = request->cpsr;

    if(!(sysReq>0 && sysReq<12)){    // sysCall Code non valido
        PANIC();
    }

    if((statusReq & STATUS_SYS_MODE) != STATUS_SYS_MODE){ //richiesta non soddisfacibile
        PANIC(); // in futuro sarà da gestire come eccezione (trap) 
    }

    switch(sysReq){
        case TERMINATEPROCESS:
            sys3_terminate();
            break;
        default:
            PANIC();
    }
}
//----------------------------------------------------------------

// Trap Handler
//----------------------------------------------------------------
void Handler_Trap(void){
	tprint( "ERROR: Trap currently unsupported\n" );
    PANIC();
}
//----------------------------------------------------------------

// TLB Handler
//----------------------------------------------------------------
void Handler_TLB(void){
    tprint( "ERROR: TLB currently unsupported\n" );
    PANIC();
}
//----------------------------------------------------------------

// Interrupt Handler
//----------------------------------------------------------------
void Handler_Interrupt(void) {	
	state_t *request    = (state_t *) INT_OLDAREA;
    word cause          = request->CP15_Cause;
    word excode         = CAUSE_EXCCODE_GET(request->CP15_Cause);

    request->pc -= WORD_SIZE;
	
    if(excode != EXC_INTERRUPT){
        PANIC();
    }

    if(CAUSE_IP_GET(cause, INT_TIMER)){
        // Interval Timer
        scheduler_StateToReady(request);
        scheduler_StateToRunning(); 
        return;
    }
    if(CAUSE_IP_GET(cause, INT_DISK)){
        // Disk Devices
        return;
    }
    if(CAUSE_IP_GET(cause, INT_TAPE)){
        // Tape Devices
        return;
    }
    if (CAUSE_IP_GET(cause, INT_UNUSED)){
        // Unused
        return;
    }
    if(CAUSE_IP_GET(cause, INT_PRINTER)){
        // Printer Devices
        return;
    }
    if(CAUSE_IP_GET(cause, INT_TERMINAL)){
        // Terminal Devices
        return;
    }

    // unhandled interrupt
    PANIC();
}