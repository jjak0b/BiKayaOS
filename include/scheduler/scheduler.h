#ifndef H_SCHEDULER
#define H_SCHEDULER

#include <pcb.h>
#include <utilities/types.h>

struct scheduler_t {

	/* coda dei processi in stato ready */
	struct list_head ready_queue;
	
	/* puntatore al descrittore del processo in esecuzione */
	pcb_t* running_p;

};
typedef struct scheduler_t scheduler_t;

/**
 * @brief Inizializza lo scheduler
 * 
 */
void scheduler_init();

/**
 * @brief Avvia lo scheduler
 * @PreCondition Deve essere richiamato dopo scheduler_init()
 * 
 * @return int 
 */
int scheduler_main();

/**
 * @brief 	Se la coda dei processi ready non è vuota,
 * 				Sceglie il processo dalla ready queue con priorità più alta
 * 				Imposta Il timer del processore a TIME_SLICE
 * 				e carica lo stato del processo sul processore
 * 				Dopo questo la funzione non dovrebbe restituire poichè il controllo dell'esecuzione è passato al processo
 * 				Se ciò avviene è dovuto ad un errore di LDST.
 * @return int 
 * 			* manda Il sistema in HALT se la coda dei processi è vuota
 * 			* -1 se non è stato caricato lo stato di un nuovo processo, senza cambiare il controllo di esecuzione ( malfunzionamento LDST )
 */
int scheduler_StateToRunning();

/**
 * @brief 	Assegna lo stato del processore fornito al processo corrente;
 * 			effettua un azione di aging per tutti gli elementi nella ready queue;
 * 			ripristina la priorità originale del processo che era in esecuzione
 * 			Disassocia il processo in esecuzione dallo scheduler
 * 
 * @param state stato del processo che era in esecuzione
 * @return int 
 * 			* 1 se non c'è alcun processo tracciato dallo scheduler in esecuzione
 * 			* 0 altrimenti se l'inserimento in ready queue è avvenuto correttamente
 */
int scheduler_StateToReady( state_t* state );

int scheduler_StateToWaiting();

/**
 * @brief Dealloca il descrittore del processo che era in esecuzione
 * 
 * @return int 
 * 			* 1 se non c'è alcun processo tracciato dallo scheduler in esecuzione
 * 			* 0 altrimenti se è avvenuto tutto correttamente
 * @param b_flag_terminate_progeny 	Se TRUE rimuove e dealloca dalla ready queue il descrittore del processo in esecuzione e tutta la sua progenie,
 * 									Se FALSE rimuove e dealloca solo il descrittore in esecuzione, ma tutti i suoi figli non avranno padre e ogni figlio non avrà fratelli
 * 										cioè saranno indipendenti tra loro
 */
int scheduler_StateToTerminate( int b_flag_terminate_progeny  );

/**
 * @brief Crea un processo, aggiungendolo al ciclo di vita dello scheduler
 * Quando il processo sarà scelto, inizierà l'esecuzine a partire dall'indirizzo di funzione fornito
 * 
 * @param func 
 * @return int 
 */
int scheduler_CreateProcess( function_t func, int priority );

/**
 * @brief Inserisce un descrittore di processo nella ready queue
 * 
 * @param p descrittore del processo
 */
void scheduler_AddProcess( pcb_t *p );

/**
 * @brief 	wrapper di pcb_RemoveProgenyQ con passata la ready queue
 * @PostCondition 	Se p è il processo in esecuzione allora viene deassociato nella struttura dello scheduler.
 * 					Non avviene alcuna rimozione nella lista dei fratelli e del padre di p.
 * 
 * @param p descrittore del processo da cui partire a rimuovere la progenie
 * @return int 
 * 			* 1 Se p == NULL
 * 			* 0 altrimenti
 */
int scheduler_RemoveProgeny( pcb_t* p );

#endif