/***********************************shared.c****************************************
*                   - Bikaya OS - Phase 1.5 - Shared -
*    
*	Welcome to BiKaya OS!
*	This module implements all 
*	functions that manage registers of struct state_t related to UARM architecture.
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

// TODO: da spostare eventualmente in header separato
#define STATUS_FASTINT_MODE 0x00000011

void EnableInterrupts( state_t *state, int b_flag ) {
    if( b_flag ){
        state->cpsr = STATUS_ENABLE_INT( state->cpsr );
        state->cpsr = STATUS_ENABLE_TIMER( state->cpsr ); /* fast interrupts */
    }
    else{
        state->cpsr = STATUS_DISABLE_TIMER( state->cpsr );
        state->cpsr = STATUS_DISABLE_INT( state->cpsr );
    }
}

void EnableKernelMode( state_t *state, int b_flag ) {
    if( b_flag ){
        state->cpsr |= STATUS_SYS_MODE;
    }
    else{
        state->cpsr &= ~STATUS_SYS_MODE;
        state->cpsr |= STATUS_USER_MODE;
    }
}

void EnableVirtualMemory( state_t *state, int b_flag ) {
    if( b_flag ){
        state->CP15_Control = CP15_ENABLE_VM( state->CP15_Control );
    }
    else {
        state->CP15_Control = CP15_DISABLE_VM( state->CP15_Control );
    }
}

void SetPC( state_t *state, memaddr value ) {
    state->pc = value;
}

void SetSP( state_t *state, memaddr value ) {
    state->sp = value;
}

void SetLR( state_t *state, memaddr value ) {
    state->lr = value;
}

void SetStatus( state_t *state, word value ) {
	state->cpsr = value;
}

word GetStatus( state_t *state ){
    return state->cpsr;
}

void moveState(state_t *before, state_t *after){
    after->a1 = before->a1;
    after->a2 = before->a2;
    after->a3 = before->a3;
    after->a4 = before->a4;
    after->v1 = before->v1;
    after->v2 = before->v2;
    after->v3 = before->v3;
    after->v4 = before->v4;
    after->v5 = before->v5;
    after->v6 = before->v6;
    after->sl = before->sl;
    after->fp = before->fp;
    after->ip = before->ip;
    after->sp = before->sp;
    after->lr = before->lr;
    after->pc = before->pc;
    after->cpsr = before->cpsr;
    after->CP15_Control = before->CP15_Control;
    after->CP15_EntryHi = before->CP15_EntryHi;
    after->CP15_Cause= before->CP15_Cause;
    after->TOD_Hi = before->TOD_Hi;
    after->TOD_Low = before->TOD_Low;
}
