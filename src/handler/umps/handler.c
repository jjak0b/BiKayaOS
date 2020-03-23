#include <system/shared/shared.h>
#include <handler/umps/handler.h>

#include <scheduler/scheduler.h>


// Syscall-Breakpoint Handler
//---------------------------------------------------------------
void Handler_SysCall(void){
    state_t *request    = (state_t *) SYSBK_OLDAREA;            /*Caller CPU state*/
    word cause          = CAUSE_GET_EXCCODE(request->cause);    /*Content of cause register*/
    
    switch(cause){
        case EXC_SYS:
            handle_syscall(request);
            break;
        case EXC_BP:
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

    if((statusReq&STATUS_KUc)!=0){ /*richiesta non soddisfacibile*/
        PANIC(); /* in futuro sarà da gestire come eccezione (trap) */
    }

    request->pc_epc += sizeof(word); //jump to next instruction

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
	word cause          = CAUSE_GET_EXCCODE(request->cause);
    
    if (cause != EXC_INT) {
        PANIC();
    }

    if (CDEV_BITMAP_ADDR(IL_IPI)) {
        // Inter-processor interrupts
        return;
    }
    if (CDEV_BITMAP_ADDR(IL_CPUTIMER)) {
        // Processor Local Timer
        return;
    }
    if (CDEV_BITMAP_ADDR(IL_TIMER)) {
        // Interval Timer
        scheduler_StateToReady( request );
        scheduler_StateToRunning(); 
        return;
    }
    if (CDEV_BITMAP_ADDR(IL_DISK)) {
        // Disk Devices
        return;
    }
    if (CDEV_BITMAP_ADDR(IL_TAPE)) {
        // Tape Devices
        return;
    }
    if (CDEV_BITMAP_ADDR(IL_ETHERNET)) {
        // Network Devices
        return;
    }
    if (CDEV_BITMAP_ADDR(IL_PRINTER)) {
        // Printer Devices
        return;
    }
    if (CDEV_BITMAP_ADDR(IL_TERMINAL)) {
        // Terminal Devices
        return;
    }

}