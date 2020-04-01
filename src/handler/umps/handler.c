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
            LDST(request); /*nothing to do.. (4 now)*/
        default: 
            PANIC();
    }
}

void handle_syscall(state_t *request){
    word sysReq     = request->reg_a0;
    word statusReq  = request->status;

    if(!(sysReq>0 && sysReq<12)){    /*sysCall Code non valido*/
        PANIC();
    }

    if((statusReq&KERNELMODE_OFF)!=RESET_STATUS){ /*richiesta non soddisfacibile*/
        PANIC(); /* in futuro sarà da gestire come eccezione (trap) ?*/
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
    
    if (exc_cause != EXC_INT){/*req error*/
        PANIC(); 
    }
    //-------------------------------------------Handle interrupt
    if (CAUSE_IP_GET(request->cause,IL_IPI)){/*Future use*/
        PANIC();
    }
    if (CAUSE_IP_GET(request->cause,IL_CPUTIMER)){/*Future use*/
        // Processor Local Timer
        PANIC();
    }
    if (CAUSE_IP_GET(request->cause,IL_TIMER)){
        // Interval Timer
        scheduler_StateToReady( request ); /*Add req to ready queue*/
        scheduler_StateToRunning(); /*Schedule*/
        return;
    }
    if (CAUSE_IP_GET(request->cause,IL_DISK)){/*Future use*/
        // Disk Devices
        PANIC();
    }
    if (CAUSE_IP_GET(request->cause,IL_TAPE)){/*Future use*/
        // Tape Devices
        PANIC();
    }
    if (CAUSE_IP_GET(request->cause,IL_ETHERNET)){/*Future use*/
        // Network Devices
        PANIC();
    }
    if (CAUSE_IP_GET(request->cause,IL_PRINTER)){/*Future use*/
        // Printer Devices
        PANIC();
    }
    if (CAUSE_IP_GET(request->cause,IL_TERMINAL)){/*Future use*/
        // Terminal Devices
        PANIC();
    }
    //----------------------------------------------------------
    PANIC();/*interrupt not defined*/
}