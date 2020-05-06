#ifndef H_SCHEDULER
#define H_SCHEDULER

#include <pcb.h>
#include <utilities/types.h>

#define TIME_SLICE 3000 /* Time slice in microsecondi di CPU dedicato ad ogni processo */

/*
    Ciclo di vita dello scheduler:
        (Per # si intende un evento che genera l'interruzione dell'esecuzione )
        (Per @ si intende una possibile gestione dell'evento)
    -> scheduler_init()
    -> scheduler_main()
        -> 	scheduler_schedule() -> # -> scheduler_UpdateContext() -> V
               ^                                                      |		
               |                                                      @ ----> scheduler_StateToTerminate()
               -------------------------------scheduler_StateToReady<-@
                                                        ^             :
                                                        |             :
                                            scheduler_StateToWaiting<-@ 


*/
struct scheduler_t {

	/* coda dei processi in stato ready */
	struct list_head ready_queue;
	
	/* puntatore al descrittore del processo in esecuzione */
	pcb_t* running_p;

    /* booleano indicante se il processo attuale non può continuare lo scheduling (TRUE) oppure continuare mantenere il processo schedulato (FALSE) */
    int b_force_switch;

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
 * @brief Aggiorna lo stato del processo tracciato in esecuzione, con quello fornito
 * 
 * @param state 
 */
void scheduler_UpdateContext( state_t* state );

/**
 * @brief 	Avvia l'esecuzione dell'algoritmo dello scheduler che in base alle circostanze può:
 *          - continuare l'esecuzione del processo attuale;
 *          - sospendere il processo attuale e scegliere ed avviare il prossimo processo nella ready queue per un TIME_SLICE, impostando tale valore al Interrupt Timer
 * 			Se la coda dei processi ready non è vuota,
 * 				Sceglie il processo dalla ready queue con priorità più alta
 * 				Imposta l' interrupt timer a TIME_SLICE
 * 				e carica lo stato del processo sul processore
 * 				Dopo questo la funzione non dovrebbe restituire poichè il controllo dell'esecuzione è passato al processo
 * 				Se ciò avviene è dovuto ad un errore di LDST.
 * @PreCondition Prima di chiamare questa funzione, è necessario chiamare scheduler_UpdateContext() per aggiornare il contesto del processo attuale
 * 				 
 * @param b_force_switch se TRUE lo scheduler è forzato a cambiare il processo da eseguire
 * @return int 
 * 			* manda Il sistema in HALT se la coda dei processi è vuota
 * 			* -1 se non è stato caricato lo stato di un nuovo processo, senza cambiare il controllo di esecuzione ( malfunzionamento LDST )
 */
int scheduler_schedule( int b_force_switch );

/**
 * @brief   Ripristina la priorità originale del processo che era in esecuzione
 * 			Disassocia il processo in esecuzione dallo scheduler
 * @PreCondition Prima di chiamare questa funzione, è necessario chiamare scheduler_UpdateContext() per aggiornare il contesto del processo attuale
 * @PostCondition Dopo questo stato è necessario richiamare scheduler_schedule per procedere con la schedulazione dei processi
 * 
 * @param state stato del processo che era in esecuzione
 * @return int 
 * 			* 1 se non c'è alcun processo tracciato dallo scheduler in esecuzione
 * 			* 0 altrimenti se l'inserimento in ready queue è avvenuto correttamente
 */
int scheduler_StateToReady();

/**
 * @brief aggiunge il processo corrente alla ASL con associato la chiave fornita
 * 
 * @PreCondition Prima di chiamare questa funzione, è necessario chiamare scheduler_UpdateContext() per aggiornare il contesto del processo attuale
 * @PostCondition Dopo questo stato è necessario richiamare scheduler_schedule per procedere con la schedulazione dei processi
 * @param semKey chiave da associare al semaforo
 * @return int 
 * 			* -1 se nessun processo è attualmente assegnato come processo corrente
 * 			* 0 se l'operazione è stata effettuata correttamente
 * 			* 1 se non è stato possibile aggiungere il processo corrente alla ASL (impossibile allocare semaforo)
 */
int scheduler_StateToWaiting( int* semKey );

/**
 * @brief Dealloca il descrittore del processo che era in esecuzione, rimuovendo eventualmente la sua progenie
 * 
 * @PreCondition Prima di chiamare questa funzione, è necessario chiamare scheduler_UpdateContext() per aggiornare il contesto del processo attuale
 * @PostCondition Dopo questo stato è necessario richiamare scheduler_schedule per procedere con la schedulazione dei processi
 * @return int 
 * 			* 1 se non c'è alcun processo tracciato dallo scheduler in esecuzione
 * 			* 0 altrimenti se è avvenuto tutto correttamente
 * @param b_flag_terminate_progeny 	Se TRUE rimuove e dealloca dalla ready queue il descrittore del processo in esecuzione e tutta la sua progenie,
 * 									Se FALSE rimuove e dealloca solo il descrittore in esecuzione, ma tutti i suoi figli non avranno padre e ogni figlio non avrà fratelli
 * 										cioè saranno indipendenti tra loro
 */
int scheduler_StateToTerminate( int b_flag_terminate_progeny  );

/**
 * @brief Restituisce il puntatore dell'attuale pcb_t in esecuzione
 * 
 * @return state_t* 
 */
pcb_t *scheduler_GetRunningProcess();

scheduler_t *scheduler_Get();

/**
 * @brief Inserisce un descrittore di processo nella ready queue
 * @PreCondition Da utilizzare solo se p non è presente nella ready queue
 * @param p descrittore del processo
 */
void scheduler_AddProcess( pcb_t *p );

/**
 * @brief Rimuove il descrittore di processo dalla ready queue ed eventualmente disassocia il processo attuale, se è lo stesso pcb fornito
 * 
 * @param p pcb da rimuovere dallo scheduler
 * @return int 
 *          TRUE se è stato rimosso con successo
 *          FALSE se non è presente nella ready queue
 */
int scheduler_RemoveProcess( pcb_t *p );

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

#endif
