#include <handler/handler.h>
#include <scheduler/scheduler.h>
#include <system/shared/shared.h>

void Handler_SysCall( word sysNum, word arg1, word arg2, word arg3 ) {
	
}

void Handler_Trap( word arg0, word arg1, word arg2, word arg3 ) {
	
}

void Handler_TLB( word arg0, word arg1, word arg2, word arg3 ) {

}

void Handler_Interrupt( word arg0, word arg1, word arg2, word arg3 ) {
	
    /* TIMER INT DETECTED */ 
    //if( getCAUSE() == IL_TIMER ) { /* TODO: provvisorio, per ora è SOLO per struttura concettuale */
    //    scheduler_StateToReady( (state_t *) INT_OLDAREA );
    //}
}