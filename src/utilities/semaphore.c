#include <utilities/semaphore.h>
#include <scheduler/scheduler.h>
#include <asl/asl.h>
#include <pcb/pcb.h>

int semaphore_P( int *semkey, pcb_t * p ) {
    int b_error = FALSE;
    if( p == NULL )
        return !b_error;
    
    if( --(*semkey) < 0 ) {
        scheduler_RemoveProcess( p );
        b_error = insertBlocked( semkey, p );
    }
    return b_error;
}

pcb_t *semaphore_V( int *semkey ) {
    pcb_t *p = NULL;
    if( ++(*semkey) <= 0 ) {
        p = removeBlocked( semkey );
        if( p != NULL )
            scheduler_AddProcess( p );
    }
    return p;
}