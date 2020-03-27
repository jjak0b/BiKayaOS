#ifndef H_SYSINIT
#define H_SYSINIT

#include <system/system.h>
#include <utilities/types.h>

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
 */
void initSysCallArea(void);

/**
 * @brief Inizializza la Trap New Area
 */
void initTrapArea(void);

/**
 * @brief Inizializza la TLB New Area
 */
void initTLBArea(void);

/**
 * @brief Inizializza la Interrupt New Area
 */
void initInterruptArea(void);

#endif