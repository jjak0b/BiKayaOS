#include <system/umps/sysinit.h>
#include <system/shared/shared.h>

#include <umps/umpsconst.h>
#include <umps/cp0.h>
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

	area->pc_epc = (memaddr) Handler_SysCall;
	area->reg_sp = ram_top_addr;
	initStatusFlag(area);
}

void initTrapArea(word ram_top_addr){
	state_t *area = (state_t *) PGMTRAP_NEWAREA;
	moveState((state_t *) SYSBK_NEWAREA, area);

	area->pc_epc = (memaddr) Handler_Trap;
	area->reg_sp = ram_top_addr;
	initStatusFlag(area);
}

void initTLBArea(word ram_top_addr){
	state_t *area = (state_t *) TLB_NEWAREA;
	moveState((state_t *) SYSBK_NEWAREA, area);

	area->pc_epc = (memaddr) Handler_TLB ;
	area->reg_sp = ram_top_addr;
	initStatusFlag(area);
}

void initInterruptArea(word ram_top_addr){
	state_t *area = (state_t *) INT_NEWAREA;
	moveState((state_t *) SYSBK_NEWAREA, area);

	area->pc_epc = (memaddr) Handler_Interrupt;
	area->reg_sp = ram_top_addr;
	initStatusFlag(area);
}

void initStatusFlag(state_t *state){
	state->status &= 0;
	state->status |= STATUS_BEV;	//Bit 22 of processor state area; related to exception vector
	state->status |= STATUS_CU0;	//Bit 28 of processor state area; related to coprocessor usability
}
