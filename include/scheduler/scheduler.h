#ifndef H_SCHEDULER
#define H_SCHEDULER

#include <pcb.h>
#include <utilities/types.h>

#define TIME_SLICE 3000 /* Time slice in microsecondi di CPU dedicato ad ogni processo */

/*
    Ciclo di vita dello scheduler:
        (Per # si intende un evento che genera l'interruzione dell'esecuzione )

    -> scheduler_init()
    -> scheduler_main()
        -> 	scheduler_StateToRunning() -> # -> scheduler_StateToTerminate()
                    ^                     |		
                    |                     |
			scheduler_StateToready() <----|

*/
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
 * @brief Avvia lo scheduler, da richiamare solo per il primo avvio dello scheduler
 * @PreCondition Deve essere richiamato dopo scheduler_init() dopo aver inserito almeno il primo processo
 * 
 * @return int 
 */
int scheduler_main();

/**
 * @brief 	Avvia l'esecuzione del prossimo processo nella ready queue per un TIME_SLICE, impostando tale valore al Interrupt Timer
 * 			Se la coda dei processi ready non è vuota,
 * 				Sceglie il processo dalla ready queue con priorità più alta
 * 				Imposta l' interrupt timer a TIME_SLICE
 * 				e carica lo stato del processo sul processore
 * 				Dopo questo la funzione non dovrebbe restituire poichè il controllo dell'esecuzione è passato al processo
 * 				Se ciò avviene è dovuto ad un errore di LDST.
 * @PreCondition Prima di chiamare questa funzione, è necessario chiamare scheduler_StateToReady o scheduler_StateToTerminate
 * 				 per aggiornare il contesto dell'ultimo processo eseguito
 * @return int 
 * 			* manda Il sistema in HALT se la coda dei processi è vuota
 * 			* -1 se non è stato caricato lo stato di un nuovo processo, senza cambiare il controllo di esecuzione ( malfunzionamento LDST )
 */
int scheduler_StateToRunning();

/**
 * @brief 	Aggiorna lo stato del processo fornito al processo corrente;
 * 			effettua un azione di aging per tutti gli elementi nella ready queue;
 * 			ripristina la priorità originale del processo che era in esecuzione
 * 			Disassocia il processo in esecuzione dallo scheduler
 * @PostCondition Dopo questo stato è necessario richiamare scheduler_StateToRunning per procedere con la schedulazione dei processi
 * 
 * @param state stato del processo che era in esecuzione
 * @return int 
 * 			* 1 se non c'è alcun processo tracciato dallo scheduler in esecuzione
 * 			* 0 altrimenti se l'inserimento in ready queue è avvenuto correttamente
 */
int scheduler_StateToReady( state_t* state );

/* WIP UNCOMPLETE */
int scheduler_StateToWaiting();

/**
 * @brief Dealloca il descrittore del processo che era in esecuzione, rimuovendo eventualmente la sua progenie
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
 * @brief Inserisce un descrittore di processo nella ready queue
 * @PreCondition Da utilizzare solo se p non è presente nella ready queue
 * @param p descrittore del processo
 */
void scheduler_AddProcess( pcb_t *p );

/**
 * @brief 	wrapper di pcb_RemoveProgenyQ con passata la ready queue dello scheduler
 * @PostCondition 	Se p è il processo in esecuzione allora viene deassociato nella struttura dello scheduler e deallocato.
 * 					Non avviene alcuna rimozione nella lista dei fratelli e del padre di p.
 * 
 * @param p descrittore del processo da cui partire a rimuovere la progenie
 * @return int 
 * 			* 1 Se p == NULL
 * 			* 0 altrimenti
 */
int scheduler_RemoveProgeny( pcb_t* p );

/*
descrizione da aggiungere
*/
int scheduler_FindReadyProc( pcb_t *p )

#endif
