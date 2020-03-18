#include <system/shared/shared.h>

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