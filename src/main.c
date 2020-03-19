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

#include "system.h"
#include "pcb/pcb.h"

#include "test.h" /* test fase */

extern struct list_head *ready_queue;

void main(){
	// Inizialializzazione del sistema
	//----------------------------------------------------
	initAreas();
	//----------------------------------------------------

	initPcbs();
	INIT_LIST_HEAD(ready_queue);

	// TODO:
	//----------------------------------------------------
	test_init(); /* inserisce le funzioni di test nello scheduler come processi */

	// Halt
	//----------------------------------------------------
	HALT();
}
