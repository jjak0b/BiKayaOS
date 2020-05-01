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

#define RESET              0
#define ACK                1
//-------------------------------------------------------Terminal
/*Terminal Device Status Codes*/
#define TERM_NOT_INSTALLED  0
#define TERM_READY          1
#define TERM_ILLEGAL_OP     2
#define TERM_BUSY           3
#define RECV_TRASM_ERR      4
#define CHAR_RECV_TRASM     5

#define IS_TERM_READY(status) ((status&0xFF)==TERM_READY)
#define IS_TERM_IN_ERROR(status) (\
   (status&0xFF)==TERM_NOT_INSTALLED || \
   (status&0xFF)==TERM_ILLEGAL_OP || \
   (status&0xFF)==RECV_TRASM_ERR)
//---------------------------------------------------------------

//------------------------------------------------------Other Dev
#define DEV_NOT_INSTALLED  0
#define DEV_READY          1
#define ILLEGAL_OP         2
#define DEV_BUSY           3
#define ERR_CODE_4         4
#define ERR_CODE_5         5
#define ERR_CODE_6         6
#define ERR_CODE_7         7
#define ERR_CODE_128       128

#define IS_DEV_IN_ERROR(status) (\
   (status&0xFF)==DEV_NOT_INSTALLED || (status&0xFF)==ILLEGAL_OP || \
   (status&0xFF)==ERR_CODE_4 || (status&0xFF)==ERR_CODE_5 \
   (status&0xFF)==ERR_CODE_6 || (status&0xFF)==ERR_CODE_7 \
   (status&0xFF)==ERR_CODE_128)

//---------------------------------------------------------------



//
#define CAUSE_IP_GET(cause, line) ((cause&CAUSE_IP(line)) != 0)

//
#define IS_IRQ_RAISED_FROM_I(line,i) ((i>=0 && i<8 && CDEV_BITMAP_ADDR(line)&(1U<<i)) != 0)

/* We are 8 type of interrupt lines. But, only 5
* lines are related to devices.
*/
#define DEV_ONLY (N_IL-3)
#define SEM_DEV_N ((DEV_ONLY+1)*N_DEV_PER_IL)

//
#define GET_SEM_OFFSET(dev_reg,line) (line!=IL_TERMINAL ? (0) : (IS_TERM_READY(dev_reg->term->transm_status)? (0) : (1)))
//
#define GET_SEM_INDEX(dev_reg,line,device) (N_DEV_PER_IL*(line-DEV_ONLY+GET_SEM_OFFSET(dev_reg,line))+device)
//
#define GET_DEV_STATUS(dev_reg, line) (line!=IL_TERMINAL ?(dev_reg->dtp->status):(GET_SEM_OFFSET(dev_reg,line) ?(dev_reg->term->recv_status):(dev_reg->term->trasm_status)) 


