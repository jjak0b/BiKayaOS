#include <system/umps/sysinit.h>
#include <system/shared/shared.h>

#include <umps/umpsconst.h>
//#include <umps/cp0.h>
#include <umps/libumps.h>

#include <handler/handler.h>

void initAreas(void){
	devregarea_t *devregarea	= (devregarea_t *) DEV_REG_AREA; // pointer to Bus register area
	word ram_top_addr			= devregarea->rambase + devregarea->ramsize;

	initSysCallArea(ram_top_addr);
	initTrapArea(ram_top_addr);
	initTLBArea(ram_top_addr);
	initInterruptArea(ram_top_addr);
}

void initSysCallArea(word ram_top_addr){
	state_t *area = (state_t *) SYSBK_NEWAREA;
	STST(area);

	SetPC(area, (memaddr) Handler_SysCall);
	SetSP(area, ram_top_addr);

	initStatusFlag(area);
}

void initTrapArea(word ram_top_addr){
	state_t *area = (state_t *) PGMTRAP_NEWAREA;
	moveState((state_t *) SYSBK_NEWAREA, area);

	SetPC(area,(memaddr)Handler_Trap);
	SetSP(area,ram_top_addr);

	initStatusFlag(area);
}

void initTLBArea(word ram_top_addr){
	state_t *area = (state_t *) TLB_NEWAREA;
	moveState((state_t *) SYSBK_NEWAREA, area);

	SetPC(area,(memaddr)Handler_TLB);
	SetSP(area,ram_top_addr);

	initStatusFlag(area);
}

void initInterruptArea(word ram_top_addr){
	state_t *area = (state_t *) INT_NEWAREA;
	moveState((state_t *) SYSBK_NEWAREA, area);

	SetPC(area,(memaddr)Handler_Interrupt);
	SetSP(area,ram_top_addr);

	initStatusFlag(area);
}

void initStatusFlag(state_t *state){
	//state->status &= 0;
	EnableInterrupts(state, 0);
	EnableKernelMode(state, 1);
	EnableVirtualMemory(state, 0);
	//state->status |= STATUS_BEV;	//Bit 22 of processor state area; related to exception vector
	//state->status |= STATUS_CU0;	//Bit 28 of processor state area; related to coprocessor usability
}
