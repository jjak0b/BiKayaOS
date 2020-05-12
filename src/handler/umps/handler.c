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
#include <utilities/semaphore.h>

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
            break; // (from sds) non necessario; è l'ultimo case
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