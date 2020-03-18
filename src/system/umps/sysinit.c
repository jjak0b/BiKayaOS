#include "system/umps/sysinit.h"

#include "umps/umpsconst.h"
#include "umps/cp0.h"
#include "umps/libumps.h"

#include "handler/handler.h"

void initAreas(void){
	devregarea_t *devregarea; // pointer to Bus register area
	devregarea = (devregarea_t *) DEV_REG_AREA;
	state_t *area = NULL;	// pointer to a processor state area					

	initSysCallArea(area, devregarea);
	initTrapArea(area, devregarea);
	initTLBArea(area, devregarea);
	initInterruptArea(area, devregarea);

	setTIMER(0); // set timer of coprocessor 0 (initialized with 0 value)
}

void initSysCallArea(state_t *area, devregarea_t *devregarea){
	area = (state_t *)SYSBK_NEWAREA;
	STST(area);

	area->pc_epc = (memaddr) sysHandler;
	area->reg_sp = devregarea->rambase + devregarea->ramsize; // RAMTOP
	initStatusFlag(area);
}

void initTrapArea(state_t *area, devregarea_t *devregarea){
	area = (state_t *)PGMTRAP_NEWAREA;
	moveState(area, (state_t *) PGMTRAP_NEWAREA);

	area->pc_epc = (memaddr) pgmTrapHandler;
	area->reg_sp = devregarea->rambase + devregarea->ramsize; // RAMTOP
	initStatusFlag(area);
}

void initTLBArea(state_t *area, devregarea_t *devregarea){
	area = (state_t *)TLB_NEWAREA;
	moveState(area, (state_t *) TLB_NEWAREA);

	area->pc_epc = (memaddr) TLBHandler ;
	area->reg_sp = devregarea->rambase + devregarea->ramsize; // RAMTOP
	initStatusFlag(area);
}

void initInterruptArea(state_t *area, devregarea_t *devregarea){
	area = (state_t *)INT_NEWAREA;
	moveState(area, (state_t *) INT_NEWAREA);

	area->pc_epc = (memaddr) intHandler;
	area->reg_sp = devregarea->rambase + devregarea->ramsize; // RAMTOP
	initStatusFlag(area);
}

void initStatusFlag(state_t *state){
	state->status &= 0;
	state->status |= STATUS_BEV;	//Bit 22 of processor state area; related to exception vector
	state->status |= STATUS_TE;		//Bit 27 of processor state area; related to local timer mask
	state->status |= STATUS_CU0;	//Bit 28 of processor state area; related to coprocessor usability
}

void moveState(state_t *before, state_t *after){
	after->entry_hi = before->entry_hi;
	after->cause = before->cause;
	after->status = before->status;
	after->pc_epc = before->pc_epc;
	after->hi = before->hi;
	after->lo = before->lo;

	for(int i=0; i<STATE_GPR_LEN+1;i++){
		after->gpr[i] = before->gpr[i];		
	}
}