#ifndef H_HANDLER
#define H_HANDLER

#include <utilities/types.h>

#ifdef TARGET_UARM
#include <uarm/uARMconst.h>
#endif

/*Syscall CODE*/
#define TERMINATEPROCESS 3

/*Dichiarazioni*/
void Handler_SysCall(void);
void Handler_Trap( word arg0, word arg1, word arg2, word arg3 );
void Handler_TLB( word arg0, word arg1, word arg2, word arg3 );
void Handler_Interrupt( word arg0, word arg1, word arg2, word arg3 );

void handle_syscall(state_t *request);
void sys3_terminate(void);
#endif