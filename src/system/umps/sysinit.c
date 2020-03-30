/***********************************sysinit.c****************************************
*                   - Bikaya OS - Phase 1.5 - sysinit -
*    
*	Welcome to BiKaya OS!
*	This module takes care of initialize all of 
*	OLD Memory Areas for UMPS architecture.
*	See "system/umps/sysinit.h" for further infos 
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

#include <system/umps/sysinit.h>
#include <system/shared/shared.h>

#include <handler/handler.h>

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
	SetSP(area, RAM_TOP);

	initStatusFlag(area);
}

void initTrapArea(void){
	state_t *area = (state_t *) PGMTRAP_NEWAREA;
	moveState((state_t *) SYSBK_NEWAREA, area);

	SetPC(area,(memaddr)Handler_Trap);
	SetSP(area,RAM_TOP);

	initStatusFlag(area);
}

void initTLBArea(void){
	state_t *area = (state_t *) TLB_NEWAREA;
	moveState((state_t *) SYSBK_NEWAREA, area);

	SetPC(area,(memaddr)Handler_TLB);
	SetSP(area,RAM_TOP);

	initStatusFlag(area);
}

void initInterruptArea(void){
	state_t *area = (state_t *) INT_NEWAREA;
	moveState((state_t *) SYSBK_NEWAREA, area);

	SetPC(area,(memaddr)Handler_Interrupt);
	SetSP(area,RAM_TOP);

	initStatusFlag(area);
}

void initStatusFlag(state_t *state){
	state->status = STATUS_NULL;
}
