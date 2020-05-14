#ifndef __SYS_SHARED_ARCH__
#define __SYS_SHARED_ARCH__

#ifdef TARGET_UARM
#define reg_pc pc
#define reg_param_0 a1
#define reg_param_1 a2
#define reg_param_2 a3
#define reg_param_3 a4
#define reg_return_0 v1
#define reg_cause CP15_Cause
#define reg_tod_hi TOD_Hi
#define reg_tod_low TOD_Low
#endif

#ifdef TARGET_UMPS
/* richiede inclusione di umps/types.h */
#define reg_pc pc_epc
#define reg_param_0 reg_a0
#define reg_param_1 reg_a1
#define reg_param_2 reg_a2
#define reg_param_3 reg_a3
#define reg_return_0 reg_v0
#define reg_cause cause
#define reg_tod_hi reg_HI
#define reg_tod_low reg_LO
#endif

#endif