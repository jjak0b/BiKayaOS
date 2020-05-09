#include "umps/arch.h"
#include "umps/cp0.h"

/* Addresses for new and old areas (where new and old processor states are
   stored on exceptions) */
#define DEV_REG_AREA 0x10000000
#define EXCV_BASE 0x20000000
#define STATE_T_SIZE 140
#define INT_OLDAREA EXCV_BASE
#define INT_NEWAREA (EXCV_BASE + STATE_T_SIZE)
#define TLB_OLDAREA (EXCV_BASE + (2 * STATE_T_SIZE))
#define TLB_NEWAREA (EXCV_BASE + (3 * STATE_T_SIZE))
#define PGMTRAP_OLDAREA (EXCV_BASE + (4 * STATE_T_SIZE))
#define PGMTRAP_NEWAREA (EXCV_BASE + (5 * STATE_T_SIZE))
#define SYSBK_OLDAREA (EXCV_BASE + (6 * STATE_T_SIZE))
#define SYSBK_NEWAREA (EXCV_BASE + (7 * STATE_T_SIZE))

#define RAMBASE    *((unsigned int *)BUS_REG_RAM_BASE)
#define RAMSIZE    *((unsigned int *)BUS_REG_RAM_SIZE)
#define RAM_TOP     (RAMBASE + RAMSIZE)


#define STATUS_NULL          0x00000000
#define INTERRUPT_ON          (STATUS_IEp)
#define INTERRUPT_OFF         (~INTERRUPT_ON)
#define KERNELMODE_OFF        (STATUS_KUp)
#define KERNELMODE_ON         (~KERNELMODE_OFF)
#define VM_ON                 (STATUS_VMp)
#define VM_OFF                (~VM_ON)
#define INTERVAL_TIMER_ON     (STATUS_IM(2))
#define INTERVAL_TIMER_OFF    (~INTERVAL_TIMER_ON)

#define CAUSE_IP_GET(cause, line) ((cause&CAUSE_IP(line)) != 0)