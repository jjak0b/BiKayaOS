#include "test.h"

#include "utilities/types.h"
#include "system/shared/shared.h"

#include "scheduler/scheduler.h"
#include "pcb/pcb.h"

void test_init() {
    int priority = 10;
    pcb_t* tests[ 3 ];
    int i;
    for( i = 0; i < 3; i++ ) {
        pcb_t *dummy = allocPcb();
        EnableInterrupts( &dummy->p_s, TRUE );
        EnableVirtualMemory( &dummy->p_s, FALSE );
        EnableKernelMode( &dummy->p_s, TRUE );

        #ifdef TARGET_UARM
            SetSP( &dummy->p_s, RAM_TOP - (FRAME_SIZE * i ) );
        #endif
        #ifdef TARGET_UMPS
            SetSP( &dummy->p_s, NULL ); /* TODO */
        #endif
        dummy->original_priority = priority;
        dummy->priority = priority;

        tests[ i ] = dummy;
    }

    SetPC( &tests[ 0 ]->p_s, test1 );
    SetPC( &tests[ 1 ]->p_s, test2 );
    SetPC( &tests[ 2 ]->p_s, test3 );

    for( i = 0; i < 3; i++ ) {
        scheduler_AddProcess( tests[ i ] );
    }
}