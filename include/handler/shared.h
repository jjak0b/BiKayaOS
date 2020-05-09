#ifndef H_HANDLER_SHARED
#define H_HANDLER_SHARED

#include <system/system.h>

#define SYSNO2INDEX( n )( (n)-1 )

#define SYS_SPECPASSUP_TYPE_SYSBK 0
#define SYS_SPECPASSUP_TYPE_TLB 1
#define SYS_SPECPASSUP_TYPE_PGMTRAP 2

#ifdef TARGET_UARM
#define reg_v0 a1
#define cause CP15_Cause
#define CAUSE_GET_EXCCODE(cause) CAUSE_EXCCODE_GET(cause)
#endif

void SpecPassup_init();

/**
 * @brief Indica se è stato definito un handler superiore del tipo fornito
 * 
 * @param type 
 * @return int boooleano
 */
int IsSetSpecPassup( int type );

/**
 * @brief Imposta l'area fornita come handler superiore del tipo fornito
 * 
 * @param type 
 * @param area 
 * @return int 
 *         - 0 se è l'area stata impostata correttamente
 *         - -1 se il tipo fornito non è gestito, area == NULL, oppure se è già stata assegnata un'area all'handler del tipo specifiato
 */
int SetSpecPassup( int type, state_t *area );

/**
 * @brief Restituisce il puntatore dello stato dell'handler superiore, associato al tipo specificato
 * 
 * @param type 
 * @return state_t* 
 *          - NULL se IsSetSpecPassup avrebbe restituito TRUE
 *          - l'area altrimenti
 */
state_t *GetSpecPassup( int type );

/**
 * @brief Chiamante di system call che dati dei parametri forniti, chiama la specifica system call con i parametri dati
 * @PostCondition valgono tutte le PostCondition delle system call
 *                inoltre,  se è gestita internamente saranno passati solo i paramentri necessari alla system call,
 *                          altrimenti se definita una SPECPASSUP, sarà caricato il contesto specificato sul processore
 *                Se la syscall ritorna un valore, il suo valore è assegnato a returnValueAddr;
 * @PreCondition returnValueAddr != NULL
 * @param sysno codice identificativo della system call
 * @param currState ultimo stato del processo in esecuzione
 * @param param1 
 * @param param2 
 * @param param3 
 * @param returnValueAddr puntatore nella quale sarà assegnato il valore di ritorno
 * @return word se == 0 la system call chiamata non ha restituito alcun valore, 1 altrimenti
 */
word Syscaller( state_t* currState, word sysno, word param1, word param2, word param3, word *returnValueAddr );

void Sys1_GetCPUTime( state_t* currState, word *user, word *kernel, word *wallclock );

/**
 * @brief Implementazione della syscall CHILD_PRIORITY
 * Crea un processo come figlio del chiamante
 * Ritorna errore se non ci sono più PCB disponibili
 * @param child_state 
 * @param child_priority 
 * @param child_pid 
 * @return int 
 */
int Sys2_CreateProcess( state_t *child_state, int child_priority, pcb_t **child_pid );

/**
 * @brief Implementazione della syscall TERMINATE_PROCESS.
 * Termina il processo corrente e tutta la sua progenie.
 * Infine, richiama lo scheduler.
 * @param pid 
 * @return int 
 */
int Sys3_TerminateProcess( pcb_t *pid );

void Sys4_Verhogen( int* semaddr );

void Sys5_Passeren( int* semaddr );

void Sys6_DoIO( word command, word *devreg, int subdevice );

int Sys7_SpecPassup( state_t* currState, int type, state_t *old_area, state_t *new_area );

/**
 * @brief assegna pid il puntatore al processo corrente, a ppid il puntatore al processo genitore
 * @param pid 
 * @param ppid 
 * @return int 
 */
int Sys8_GetPID( pcb_t **pid, pcb_t **ppid );

// Interrupt Handler functions and define
//-------------------------------------------------------
void Handler_Interrupt(void);

/**
 * @brief Gestore per interrupt device. Richiama i metodi sottostanti in base
 * al parametro line
 */
void handle_irq(unsigned int line, unsigned int dev);

/**
 * @brief Gestione per i terminali di ricezione e trasmissione. Se è presente un codice di errore nello status del terminale 
 * scrive nel command register il comando RESET, altrimenti scrive il comando ACK
 * @param puntatore al device register
 */
void handle_irq_terminal(devreg_t *dev_reg);

/**
 * @brief Gestione per tutti i device che non siano un terminale. Se è presente un codice di errore nello status del device 
 * scrive nel command register il comando RESET, altrimenti scrive il comando ACK
 * @param puntatore al device register
 */
void handle_irq_other_dev(devreg_t *dev_reg);

#endif
