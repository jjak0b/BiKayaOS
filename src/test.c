/***********************************test.c****************************************
*                   - Bikaya OS - Phase 1.5 - Test -
*    
*	Welcome to BiKaya OS!
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

#include <system/system.h>
#include <utilities/types.h>

#include <scheduler/scheduler.h>
#include <pcb/pcb.h>

#include <system/shared/shared.h>
#include <test.h>

void test_init(){
    pcb_t* tests[ 3 ];
    int i;

    for(i = 0; i < 3; i++){ // il nostro n prende i valori 1,2,3 (cioè i+1)
        tests[i] = allocPcb();

        SetStatus( &( tests[i]->p_s), STATUS_NULL );

        EnableInterrupts(&(tests[i]->p_s), TRUE);
        EnableVirtualMemory(&(tests[i]->p_s), FALSE);
        EnableKernelMode(&(tests[i]->p_s), TRUE);

        SetSP(&(tests[i]->p_s), (memaddr)RAM_TOP-(FRAMESIZE * (i+1)));
        
        tests[i]->original_priority = i;
        tests[i]->priority          = tests[i]->original_priority;
    }

    SetPC(&(tests[ 0 ]->p_s), (memaddr)test1);
    SetPC(&(tests[ 1 ]->p_s), (memaddr)test2);
    SetPC(&(tests[ 2 ]->p_s), (memaddr)test3);

    for(i = 0; i < 3; i++){
        scheduler_AddProcess(tests[ i ]);
    }
}
