#ifndef _UTIL_SEMAPHORE_
#define _UTIL_SEMAPHORE_

#include <pcb/pcb.h>

/**
 * @brief Decrementa il valore del semaforo, se diventa < 0, allora il processo indicato viene sospeso;
 *        cioè rimosso dalla ready queue dello scheduler e posto in attesa sulla coda del semaforo specificato
 * @PreCondition semkey != NULL 
 * 
 * @param semkey 
 * @param p 
 * @return int 
 *          FALSE se è stato sospeso correttamente
 *          TRUE se p == NULL oppure se non è stato possibile allocare un semd per il processo specificato
 */
int semaphore_P( int *semkey, pcb_t * p );

/**
 * @brief Incrementa il valore del semaforo, se il suo valore diventa <= 0, allora il primo processo in coda
 *        associato al semaforo specificato è risvegliato; cioè aggiunto alla ready queue
 * @PreCondition semkey != NULL 
 * 
 * @param semkey 
 * @return pcb_t* il puntatore al pcb risvegliato
 */
pcb_t *semaphore_V( int *semkey );

#endif