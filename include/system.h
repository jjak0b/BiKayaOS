#ifdef TARGET_UMPS
#include "umps/libumps.h"
#include "umps/arch.h"
#include "umps/types.h"
#endif
#ifdef TARGET_UARM
#include "uarm/libuarm.h"
#include "uarm/arch.h"
#include "uarm/uARMtypes.h"
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef unsigned char byte;
typedef unsigned int word;
typedef enum bool {
	false = 0,
	true = 1
} bool;
