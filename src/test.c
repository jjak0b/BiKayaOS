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

void test_init(void){
    pcb_t* tests[3];    /*PCB used for test*/
    state_t *state;     /*Pointer to a processor state area*/

    int i;
    for(i = 0; i < 3; i++){ /*Alloc PCB and initialize process*/
        tests[i]    = allocPcb();
        state       = &(tests[i]->p_s);
        
        //--------------Initialize status register
        SetStatus(state, STATUS_NULL);
        EnableInterrupts(state, TRUE);
        EnableVirtualMemory(state, FALSE);
        EnableKernelMode(state, TRUE);
        //----------------------------------------

        //---------------------Set SP and priority 
        SetSP(state, (memaddr)RAM_TOP-(FRAMESIZE * (i+1)));
        tests[i]->original_priority = i;
        tests[i]->priority          = tests[i]->original_priority;
        //---------------------------------------
    }

    //--------------------------------------Set PC 
    SetPC(&(tests[ 0 ]->p_s), (memaddr)test1);
    SetPC(&(tests[ 1 ]->p_s), (memaddr)test2);
    SetPC(&(tests[ 2 ]->p_s), (memaddr)test3);
    //--------------------------------------------

    for(i = 0; i < 3; i++){
        scheduler_AddProcess(tests[i]); /*Add process to ready queue*/
    }
}
