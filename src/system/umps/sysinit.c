#include <system/umps/sysinit.h>
#include <system/shared/shared.h>

#include <umps/umpsconst.h>
#include <umps/libumps.h>

#include <handler/shared.h>

void initAreas(void){
	initSysCallArea();
	initTrapArea();
	initTLBArea();
	initInterruptArea();
}

void initSysCallArea(void){
	state_t *area = (state_t *) SYSBK_NEWAREA;
	STST(area);

	SetPC(area, (memaddr) Handler_SysCall);
	SetSP(area, RAMTOP);

	initStatusFlag(area);
}

void initTrapArea(void){
	state_t *area = (state_t *) PGMTRAP_NEWAREA;
	moveState((state_t *) SYSBK_NEWAREA, area);

	SetPC(area,(memaddr)Handler_Trap);
	SetSP(area,RAMTOP);

	initStatusFlag(area);
}

void initTLBArea(void){
	state_t *area = (state_t *) TLB_NEWAREA;
	moveState((state_t *) SYSBK_NEWAREA, area);

	SetPC(area,(memaddr)Handler_TLB);
	SetSP(area,RAMTOP);

	initStatusFlag(area);
}

void initInterruptArea(void){
	state_t *area = (state_t *) INT_NEWAREA;
	moveState((state_t *) SYSBK_NEWAREA, area);

	SetPC(area,(memaddr)Handler_Interrupt);
	SetSP(area,RAMTOP);

	initStatusFlag(area);
}

void initStatusFlag(state_t *state){
	EnableInterrupts(state, 0);
	EnableKernelMode(state, 0);
	EnableVirtualMemory(state, 0);
}
