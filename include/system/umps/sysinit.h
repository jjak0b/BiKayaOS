#include "types_bikaya.h"

/**
 * @brief Inizializza il registro di status:
 * - mascherando gli interrupt
 * - disabilitando la virtual memory
 * - settando la kernel mode
 * - abilitando il timer del coprocessor 0
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
 * state: puntatore ad una new area del ROM Reserved Frame
 * devregarea: puntatore alla Bus register area 
 */
void initSysCallArea(state_t *area, devregarea_t *devregarea);

/**
 * @brief Inizializza la Trap New Area
 * @param 
 * state: puntatore ad una new area del ROM Reserved Frame
 * devregarea: puntatore alla Bus register area 
 */
void initTrapArea(state_t *area, devregarea_t *devregarea);

/**
 * @brief Inizializza la TLB New Area
 * @param 
 * state: puntatore ad una new area del ROM Reserved Frame
 * devregarea: puntatore alla Bus register area 
 */
void initTLBArea(state_t *area, devregarea_t *devregarea);

/**
 * @brief Inizializza la Interrupt New Area
 * @param 
 * state: puntatore ad una new area del ROM Reserved Frame
 * devregarea: puntatore alla Bus register area 
 */
void initInterruptArea(state_t *area, devregarea_t *devregarea);