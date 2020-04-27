#ifndef H_HANDLER_SHARED
#define H_HANDLER_SHARED

#include <system/system.h>

/**
 * @brief Chiamante di system call che dati dei parametri forniti, chiama la specifica system call con i parametri dati
 * @PostCondition valgono tutte le PostCondition delle system call
 *                inoltre,  se è gestita internamente saranno passati solo i paramentri necessari alla system call,
 *                          altrimenti se definita una SPECPASSUP saranno passati tutti i paramentri forniti
 * 
 * @param sysno codice identificativo della system call
 * @param param1 
 * @param param2 
 * @param param3 
 * @param returnValueAddr puntatore nella quale sarà assegnato il valore di ritorno ( se != NULL )
 * @return word se == 0 la system call chiamata non ha restituito alcun valore, 1 altrimenti
 */
word Syscaller( word sysno, word param1, word param2, word param3, word *returnValueAddr );

void Sys1_CetCPUTime( word *user, word *kernel, word *wallclock );

int Sys2_CreateProcess( state_t *state, int priority, void **cpid );

/**
 * @brief Implementazione della syscall TERMINATE_PROCESS.
 * Termina il processo corrente e tutta la sua progenie.
 * Infine, richiama lo scheduler.
 */
int Sys3_TerminateProcess( void *pid );

void Sys4_Verhogen( int* semaddr );

void Sys5_Passeren( int* semaddr );

int Sys6_DoIO( word command, word *devreg, int subdevice );

int Sys7_SpecPassup( int type, state_t *old_area, state_t *new_area );

int Sys8_GetPID( void **pid, void **ppid );
#endif