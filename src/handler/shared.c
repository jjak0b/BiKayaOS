/***********************************shared.c****************************************
*                   - Bikaya OS - Phase 1.5 - Shared -
*    
*	Welcome to BiKaya OS!
*	This module handle all 
*	functions shared between architecture and related to handler module.
*	See "handler/shared.h" for further infos 
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
#include <handler/shared.h>
#include <scheduler/scheduler.h>
#include <asl/asl.h>
#include <pcb/pcb.h>

word Syscaller( state_t *state, word sysNo, word param1, word param2, word param3, word *returnValue ) {
    int b_hasReturnValue = FALSE;
    switch( sysNo ) {
        case GETCPUTIME:
            Sys1_GetCPUTime( state, (word*)param1, (word*)param2, (word*)param3 );
            break;
        case CREATEPROCESS:
            b_hasReturnValue = TRUE;
            *returnValue = (int) Sys2_CreateProcess( (state_t*)param1, (int)param2, (void**)param3 );
            break;
        case TERMINATEPROCESS:
            b_hasReturnValue = TRUE;
            *returnValue = (int) Sys3_TerminateProcess( (void*) param1 );
            break;
        case VERHOGEN:
            Sys4_Verhogen( (int*)param1 );
            break;
        case PASSEREN:
            Sys5_Passeren( state, (int*)param1 );
            break;
        case WAITIO:
            b_hasReturnValue = TRUE;
            *returnValue = (int) Sys6_DoIO( param1, (word*)param2, (int)param3 );
            break;
        case SPECPASSUP:
            b_hasReturnValue = TRUE;
            *returnValue = (int) Sys7_SpecPassup( (int)param1, (state_t*)param2, (state_t*)param3 );
            break;
        case GETPID:
            b_hasReturnValue = TRUE;
            *returnValue = (int) Sys8_GetPID( (void**)param1, (void**)param2 );
            break;
        default:
            // TODO: rendere gestibile da handler di SPECPASSUP
            break;
    }

    return b_hasReturnValue;
}

void Sys1_GetCPUTime( state_t* currState, word *user, word *kernel, word *wallclock ) {
    // TODO
}

int Sys2_CreateProcess( state_t *state, int priority, void **cpid ) {
    // TODO
    return -1;
}

int Sys3_TerminateProcess( void *pid ) {
    if( pid != NULL ) {
        // TODO: da cambiare gestione
        scheduler_StateToTerminate(TRUE);
        scheduler_StateToRunning();
    }
    else {
        // TODO
    }

    return -1;
}

void Sys4_Verhogen( int* semaddr ) {
    pcb_t *blocked = removeBlocked( semaddr );
    if( blocked != NULL ) {
        /* dovremmo ripristinare la priorità ? */
        scheduler_AddProcess( blocked );
    }
    else {
        (*semaddr) ++;
    }
}

void Sys5_Passeren( state_t* state, int* semaddr ) {
    if( *semaddr <= 0 ) {
        scheduler_StateToWaiting( state, semaddr );    
        // nel caso ritornasse 1 non può accadere perchè ad ogni processo può essere nella ASL al massimo 1 volta
        // infatti se non fosse disponibile un semd, non esisterebbe neanche questo processo

        // sia in caso ritorni 0 o -1 lo scheduler deve proseguire
        scheduler_StateToRunning();
    }
    else {
        (*semaddr) --;
    }
}

int Sys6_DoIO( word command, word *devreg, int subdevice ) {
    // TODO
    return -1;
}

int Sys7_SpecPassup( int type, state_t *old_area, state_t *new_area ) {
    // TODO
    return -1;
}

int Sys8_GetPID( void **pid, void **ppid ) {
    // TODO
    return -1;
}