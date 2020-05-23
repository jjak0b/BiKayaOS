#ifndef PCB_UTILS_H
#define PCB_UTILS_H
#include <pcb/pcb.h>

/**
 * @brief   rimuove tutti i figli di old_parent, aggiungendoli a quelli di new_parent
 * @PostCondition   Se new_parent == NULL, allora i figli di old_parent saranno senza fratelli e senza padre
 * @param old_parent genitore di cui si vuole rimuovere i figli
 * @param new_parent il nuovo genitore dei figli
 */     
void pcb_SetChildrenParent( pcb_t* old_parent, pcb_t* new_parent );

/**
 * @brief Inizializza un singolo pcb con valori NULL/0 e liste vuote
 * @param p 
 * @param b_shouldInitState se FALSE si limiterà alla inizializzazione dei campi, eccetto lo state_t,
 *                          altrimenti inizializzerà anche i campi dello state_t in base all'architettura
 */
void pcb_init( pcb_t *p, int b_shouldInitState );

#endif