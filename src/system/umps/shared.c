#include <system/shared/shared.h>

#include <umps/cp0.h>

void EnableInterrupts(state_t *state, int b_flag){
    state->status = b_flag ? (state->status|STATUS_IEc) : (state->status&~STATUS_IEc);
}

void EnableKernelMode(state_t *state, int b_flag){
    state->status = b_flag ? (state->status|STATUS_KUc) : (state->status&~STATUS_KUc);
}

void EnableVirtualMemory(state_t *state, int b_flag){
    state->status = b_flag ? (state->status|STATUS_VMc) : (state->status&~STATUS_VMc);
}

void SetPC(state_t *state, memaddr value){
    state->pc_epc = value;
}

void SetSP(state_t *state, memaddr value){
    state->reg_sp = value;
}

void SetLR(state_t *state, memaddr value) {
//    state->lr = value;
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
