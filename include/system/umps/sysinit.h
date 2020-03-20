#ifndef H_SYSINIT
#define H_SYSINIT

#include "utilities/types.h"

/**
 * @brief Inizializza il registro di status:
 * - mascherando gli interrupt
 * - disabilitando la virtual memory
 * - settando la kernel mode
 * @param 
 * state: puntatore ad una new area del ROM Reserved Frame
 */
void initStatusFlag(state_t *state);

/**
 * @brief Inizializza le new areas del ROM Reserved Frame.
*/
void initAreas(void);

/**
 * @brief Inizializza la SysCall New Area
 * @param 
 * ram_top_addr: indirizzo di RAM_TOP 
 */
void initSysCallArea(word ram_top_addr);

/**
 * @brief Inizializza la Trap New Area
 * @param 
 * ram_top_addr: indirizzo di RAM_TOP 
 */
void initTrapArea(word ram_top_addr);

/**
 * @brief Inizializza la TLB New Area
 * @param 
 * ram_top_addr: indirizzo di RAM_TOP 
 */
void initTLBArea(word ram_top_addr);

/**
 * @brief Inizializza la Interrupt New Area
 * @param 
 * ram_top_addr: indirizzo di RAM_TOP 
 */
void initInterruptArea(word ram_top_addr);

#endif