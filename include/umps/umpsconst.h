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
