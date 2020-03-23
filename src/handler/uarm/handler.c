#include <system/shared/shared.h>
#include <handler/uarm/handler.h>

#include <scheduler/scheduler.h>

// Syscall-Breakpoint Handler
//---------------------------------------------------------------
void Handler_SysCall(void){
    state_t *request    = (state_t *) SYSBK_OLDAREA;            /*Caller CPU state*/
    word cause          = CAUSE_EXCCODE_GET(request->CP15_Cause);    /*Content of cause register*/
    
    switch(cause){
        case EXC_SYSCALL:
            handle_syscall(request);
            break;
        case EXC_BREAKPOINT:
        default: 
            PANIC();
    }
}

void handle_syscall(state_t *request){
    word sysReq     = request->a1;
    word statusReq  = request->cpsr;

    if(!(sysReq>0 && sysReq<12)){    /*sysCall Code non valido*/
        PANIC();
    }

    if((statusReq&STATUS_SYS_MODE)!=0){ /*richiesta non soddisfacibile*/
        PANIC(); /* in futuro sarà da gestire come eccezione (trap) */
    }

    request->pc += WORD_SIZE; //jump to next instruction

    switch(sysReq){
        case TERMINATEPROCESS:
            sys3_terminate();
            break;
        default:
            PANIC();
    }
}

void sys3_terminate(void){
    scheduler_StateToTerminate(1); /*MEGLIO DEFINIRE COSTANTE*/
    scheduler_main();
}
//----------------------------------------------------------------

// Trap Handler
//----------------------------------------------------------------
void Handler_Trap( word arg0, word arg1, word arg2, word arg3 ) {
	
}
//----------------------------------------------------------------

// TLB Handler
//----------------------------------------------------------------
void Handler_TLB( word arg0, word arg1, word arg2, word arg3 ) {

}
//----------------------------------------------------------------

// Interrupt Handler
//----------------------------------------------------------------
void Handler_Interrupt() {
	
	state_t *request    = (state_t *) INT_OLDAREA;
    word cause          = CAUSE_EXCCODE_GET(request->CP15_Cause);
    request->pc -= WORD_SIZE;
	
    if (cause != EXC_INTERRUPT) {
        PANIC();
    }

    if CAUSE_IP_GET(cause, INT_TIMER) {
        // Interval Timer
        scheduler_StateToReady( request );
        scheduler_StateToRunning(); 
        return;
    }
    if CAUSE_IP_GET(cause, INT_DISK) {
        // Disk Devices
        return;
    }
    if CAUSE_IP_GET(cause, INT_TAPE) {
        // Tape Devices
        return;
    }
    if CAUSE_IP_GET(cause, INT_UNUSED) {
        // Unused
        return;
    }
    if CAUSE_IP_GET(cause, INT_PRINTER) {
        // Printer Devices
        return;
    }
    if CAUSE_IP_GET(cause, INT_TERMINAL) {
        // Terminal Devices
        return;
    }

}