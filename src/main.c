/***********************************main.c****************************************
*                   - Bikaya OS - Phase 1.5 - Main -
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

#ifdef TARGET_UMPS
#include <system/umps/sysinit.h>
#endif
#ifdef TARGET_UARM
#include <system/uarm/sysinit.h>
#endif

#include <system/system.h>
#include <pcb/pcb.h>
#include <scheduler/scheduler.h>

#include <test.h>

int main(){
	// Inizialializzazione del sistema
	//----------------------------------------------------
	initAreas();
	//----------------------------------------------------
	initPcbs();
	scheduler_init();
	
	//Avvio test
	//----------------------------------------------------
	test_init();
	scheduler_main();
	//----------------------------------------------------
	return 0;
}
