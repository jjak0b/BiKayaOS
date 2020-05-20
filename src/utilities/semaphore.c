#include <utilities/semaphore.h>
#include <scheduler/scheduler.h>
#include <asl/asl.h>
#include <pcb/pcb.h>

int semaphore_P( int *semkey, pcb_t * p ) {
    int b_error = 0;
    if( --(*semkey) < 0 ) {
        b_error = scheduler_StateToWaiting( p, semkey );
        if( b_error ) ++(*semkey);
    }
    return b_error;
}

pcb_t *semaphore_V( int *semkey ) {
    pcb_t *p = NULL;
    if( ++(*semkey) <= 0 ) {
        p = removeBlocked( semkey );
        if( p != NULL ){
            p->p_semkey = NULL;
            scheduler_AddProcess( p );
        }
    }
    return p;
}