/***********************************shared.c****************************************
*                   - Bikaya OS - Phase 1.5 - Shared -
*    
*	Welcome to BiKaya OS!
*	This module implements all 
*	functions that manage registers of struct state_t related to UMPS architecture.
*	See "system/shared/shared.h" for further infos 
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

void EnableInterrupts(state_t *state, int b_flag){
    state->status = b_flag ? (state->status|INTERRUPT_ON|INTERVAL_TIMER_ON) : (state->status&INTERRUPT_OFF&INTERVAL_TIMER_OFF);
}

void EnableKernelMode(state_t *state, int b_flag){
    state->status = b_flag ? (state->status&KERNELMODE_ON) : (state->status|KERNELMODE_OFF);
}

void EnableVirtualMemory(state_t *state, int b_flag){
    state->status = b_flag ? (state->status|VM_ON) : (state->status&VM_OFF);
}

void SetPC(state_t *state, memaddr value){
    state->pc_epc = value;
}

void SetSP(state_t *state, memaddr value){
    state->reg_sp = value;
}

void SetLR(state_t *state, memaddr value){
}

void SetStatus( state_t *state, word value ) {
	state->status = value;
}

word GetStatus( state_t *state ) {
    return state->status;
}

void moveState(state_t *before, state_t *after){
	after->entry_hi	= before->entry_hi;
	after->cause	= before->cause;
	after->status	= before->status;
	after->pc_epc	= before->pc_epc;
	after->hi		= before->hi;
	after->lo		= before->lo;

	for(int i=0; i<STATE_GPR_LEN+1;i++){
		after->gpr[i] = before->gpr[i];		
	}
}
