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
#include <system/const_bikaya.h>

void test_init(void){
    state_t *state;     /*Pointer to a processor state area*/
    pcb_t *test;
    /*Alloc PCB and initialize process*/
    test = allocPcb();
    state = &(test->p_s);
    
    //--------------Initialize status register
    SetStatus(state, STATUS_NULL);
    EnableInterrupts(state, TRUE);
    EnableVirtualMemory(state, FALSE);
    EnableKernelMode(state, TRUE);
    //----------------------------------------

    //---------------------Set SP and priority 
    SetSP(state, (memaddr)RAM_TOP-FRAMESIZE);
    test->original_priority = DEFAULT_PRIORITY;
    test->priority          = test->original_priority;
    //---------------------------------------

    //--------------------------------------Set PC 
    SetPC(&(test->p_s), (memaddr)test);
    //--------------------------------------------

    scheduler_AddProcess(test); /*Add process to ready queue*/
}
