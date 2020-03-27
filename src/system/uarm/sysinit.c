/***********************************sysinit.c****************************************
*                   - Bikaya OS - Phase 1.5 - sysinit -
*    
*	Welcome to BiKaya OS!
*	This module takes care of initialize all of 
*	OLD Memory Areas for UARM architecture.
*	See "system/uarm/sysinit.h" for further infos 
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

#include <system/uarm/sysinit.h>
#include <handler/handler.h>

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