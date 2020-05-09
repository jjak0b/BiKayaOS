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