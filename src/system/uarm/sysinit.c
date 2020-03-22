#include <system/shared/shared.h>

#include <system/uarm/sysinit.h>
#include <handler/uarm/handler.h>

void initAreas( void ){
	initSysCallArea();
	initTrapArea();
	initTLBArea();
	initInterruptArea();
}

void initStatusFlag(state_t *state) {
	state->cpsr &= STATUS_NULL; /* Disable all ( Interrupts and others modes ) */
    state->cpsr |= STATUS_SYS_MODE; /* Enable Kernel Mode */
    state->CP15_Control = CP15_DISABLE_VM( state->CP15_Control ); /* Disable Virtual memory translation */
}

void initSysCallArea(){
    state_t* area = (state_t*)SYSBK_NEWAREA;
    initStatusFlag( area );
    area->sp = RAM_TOP;
    area->pc = (memaddr)Handler_SysCall;
}

void initTrapArea(){
    state_t* area = (state_t*)PGMTRAP_NEWAREA;
    initStatusFlag( area );
    area->sp = RAM_TOP;
    area->pc = (memaddr)Handler_Trap;
}

void initTLBArea(){
    state_t* area = (state_t*)TLB_NEWAREA;
    initStatusFlag( area );
    area->sp = RAM_TOP;
    area->pc = (memaddr)Handler_TLB;
}

void initInterruptArea(){
    state_t* area = (state_t*)INT_NEWAREA;
    initStatusFlag( area );
    area->sp = RAM_TOP;
    area->pc = (memaddr)Handler_Interrupt;
}