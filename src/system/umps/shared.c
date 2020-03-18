#include <system/shared/shared.h>

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