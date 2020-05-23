#include <pcb/pcb.h>
#include <pcb/utils.h>

void pcb_SetChildrenParent( pcb_t* old_parent, pcb_t* new_parent ) {

    while( !emptyChild( old_parent ) ) {
        pcb_t *child = removeChild( old_parent );
        if( new_parent != NULL ){
            insertChild( new_parent, child );
        }
    }
}

void pcb_init( pcb_t *new, int b_shouldInitState ){
    if(new != NULL) {
        //inizializzazione pcb
        INIT_LIST_HEAD(&(new->p_child));
        INIT_LIST_HEAD(&(new->p_sib));
        INIT_LIST_HEAD(&(new->p_next));
        new->p_parent = NULL;
        new->p_semkey = NULL;
        new->priority = 0;
        new->original_priority = 0;
        new->first_activation_tod = 0;
        new->chrono_start_tod = 0;
        new->kernel_timelapse = 0;
        new->user_timelapse = 0;
        int i, j;
        for( i = 0; i < 3; i++ )
            for( j = 0; j < 2; j++ )
                new->specPassup[ i ][ j ] = NULL;

        if( !b_shouldInitState ) return;

#ifdef TARGET_UMPS
        new->p_s.entry_hi = 0;
        new->p_s.cause = 0;
        new->p_s.status = 0;
        new->p_s.pc_epc = 0;
        new->p_s.hi = 0;
        new->p_s.lo = 0;
        for( i = 0; i < STATE_GPR_LEN; i++ ){
            new->p_s.gpr[ i ] = 0;
        }
#endif
#ifdef TARGET_UARM
        new->p_s.a1 = 0;
        new->p_s.a2 = 0;
        new->p_s.a3 = 0;
        new->p_s.a4 = 0;
        new->p_s.v1 = 0;
        new->p_s.v2 = 0;
        new->p_s.v3 = 0;
        new->p_s.v4 = 0;
        new->p_s.v5 = 0;
        new->p_s.v6 = 0;
        new->p_s.sl = 0;
        new->p_s.fp = 0;
        new->p_s.ip = 0;
        new->p_s.sp = 0;
        new->p_s.lr = 0;
        new->p_s.pc = 0;
        new->p_s.cpsr = 0;
        new->p_s.CP15_Control = 0;
        new->p_s.CP15_EntryHi = 0;
        new->p_s.CP15_Cause = 0;
        new->p_s.TOD_Hi = 0;
        new->p_s.TOD_Low = 0;
#endif
    }
}