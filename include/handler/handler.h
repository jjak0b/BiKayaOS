#ifndef H_HANDLER
#define H_HANDLER

#include <shared/types.h>

void Handler_SysCall( word sysNum, word arg1, word arg2, word arg3 );
void Handler_Trap( word arg0, word arg1, word arg2, word arg3 );
void Handler_TLB( word arg0, word arg1, word arg2, word arg3 );
void Handler_Interrupt( word arg0, word arg1, word arg2, word arg3 );

 #endif