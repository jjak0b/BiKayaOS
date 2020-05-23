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
    pcb_t *test_pcb     = allocPcb();
    state_t *state  = &(test_pcb->p_s);
    
    //--------------Initialize status register
    SetStatus(state, STATUS_NULL);
    EnableInterrupts(state, TRUE);
    EnableVirtualMemory(state, FALSE);
    EnableKernelMode(state, TRUE);
    //----------------------------------------

    //---------------------Set SP and priority 
    SetSP(state, (memaddr)RAM_TOP-FRAMESIZE);
    test_pcb->original_priority = DEFAULT_PRIORITY;
    test_pcb->priority          = test_pcb->original_priority;
    //---------------------------------------

    //--------------------------------------Set PC 
    SetPC(state, (memaddr)test);
    //--------------------------------------------

    scheduler_AddProcess(test_pcb); /*Add process to ready queue*/
}
