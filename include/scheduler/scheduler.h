#ifndef H_SCHEDULER
#define H_SCHEDULER

#include <pcb.h>
#include <utilities/types.h>

struct scheduler_t {

	/* coda dei processi in stato ready */
	struct list_head ready_queue;

};
typedef struct scheduler_t scheduler_t;

/**
 * @brief Inizializza lo scheduler
 * 
 * @return int 
 */
int scheduler_init();

/**
 * @brief Avvia lo scheduler
 * 
 * @return int 
 */
int scheduler_main();

/**
 * @brief Crea un processo, aggiungendolo al ciclo di vita dello scheduler
 * Quando il processo sarà scelto, inizierà l'esecuzine a partire dall'indirizzo di funzione fornito
 * 
 * @param func 
 * @return int 
 */
int scheduler_CreateProcess( function_t func, int priority );


int scheduler_AddProcess( pcb_t *p );

#endif