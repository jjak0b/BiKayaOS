#ifndef H_SYSTEM_SHARED
#define H_SYSTEM_SHARED
/**
 * @brief INTERFACCIA comune ad entrambe le architetture
 * 
 * Se più operazioni sono condivise da entrambe le architetture
 * Inserire qui la relativa operazione
 * 
 * E scrivere l'implemenetazione in system/<ARCHITETTURA>/shared.c
 */

#include <system/system.h>

/**
 * @brief Copia before in after
 * 
 * @param before il processor state da copiare
 * @param after il processor state target
 */
void moveState(state_t *before, state_t *after);

#endif