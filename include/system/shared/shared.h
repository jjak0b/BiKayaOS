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
#include <utilities/types.h>

#define TIME_SLICE 0xFFFF0000 /* TODO: da sistemare a valore effettivo */ /* Time slice di CPU dedicato ad ogni processo */

void EnableInterrupts( state_t *state, int b_flag );

void EnableKernelMode( state_t *state, int b_flag );

void EnableVirtualMemory( state_t *state, int b_flag );

void SetPC( state_t *state, memaddr value );

void SetSP( state_t *state, memaddr value );

void SetLR( state_t *state, memaddr value );

/**
 * @brief Copia before in after
 * 
 * @param before il processor state da copiare
 * @param after il processor state target
 */
void moveState(state_t *before, state_t *after);

#endif