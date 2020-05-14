#ifndef H_HANDLER_SHARED
#define H_HANDLER_SHARED

#include <system/system.h>

#define SYSNO2INDEX( n )( (n)-1 )

#define SYS_SPECPASSUP_AREA_OLD 0
#define SYS_SPECPASSUP_AREA_NEW 1
#define SYS_SPECPASSUP_TYPE_SYSBK 0
#define SYS_SPECPASSUP_TYPE_TLB 1
#define SYS_SPECPASSUP_TYPE_PGMTRAP 2

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

/**
 * @brief Aggiorna il calcolo delle tempistiche e assegna i valori ricavati ai puntatori
 * Alla fine fa ripartire il cronometro
 * @param user 
 * @param kernel 
 * @param wallclock 
 * @return int 
 */
int Sys1_GetCPUTime( unsigned int *user, unsigned int *kernel, unsigned int *wallclock );

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
 */
void Sys8_GetPID( pcb_t **pid, pcb_t **ppid );

/**
 * @brief Carica sul processore lo stato associato alla specPassup del type fornito, memorizzando nella sua old area, lo stato request
 * 
 * @param request 
 * @param type 
 * @return int 
 *          -1 se il type fornito non è valido, oppure se non è stato associato un handler superiore al tipo fornito
 *          Altrimenti non dovrebbe ritornare perchè il controllo dell'esecuzione cambia con quello dell handler associata
 */
int handle_specPassUp( state_t *request, int type );

// Interrupt Handler functions and define
//-------------------------------------------------------
void Handle_Interrupt( void );

void Handle_Trap( void );

void Handle_TLB( void );

void Handle_breakpoint( void );

/**
 * @brief Gestore per interrupt device. Richiama i metodi sottostanti in base
 * al parametro line
 */
void handle_irq(unsigned int line, unsigned int dev);

/**
 * @brief Gestione per i terminali di ricezione e trasmissione. Se è presente un codice di errore nello status del terminale 
 * scrive nel command register il comando RESET, altrimenti scrive il comando ACK
 * @param line linea su cui si è verificato l'interrupt
 * @param dev device su cui si è verificato l'interrupt
 * @param puntatore al device register
 */
void handle_irq_terminal(unsigned int line, unsigned int dev, devreg_t *dev_reg);

/**
 * @brief Gestione per tutti i device che non siano un terminale. Se è presente un codice di errore nello status del device 
 * scrive nel command register il comando RESET, altrimenti scrive il comando ACK
 * @param line linea su cui si è verificato l'interrupt
 * @param dev device su cui si è verificato l'interrupt
 * @param puntatore al device register
 */
void handle_irq_other_dev(unsigned int line, unsigned int dev, devreg_t *dev_reg);

/**
 * @brief Notifica successo/non successo di un operazione di IO al processo che l'ha richiesto. Se esiste un secondo processo in
 * coda sul semaforo del device, fa partire la sua richiesta
 * @param line linea su cui si è verificato l'interrupt
 * @param dev device su cui si è verificato l'interrupt
 * @param subdev_trasm 1 se si tratta del terminale di trasmissione, 0 in tutti gli altri casi
 * @param dev_reg puntatore al device register
 * @param status stato di risposta del device
 */
void notifyEvent(unsigned int line, unsigned int dev, unsigned int subdev_trasm, devreg_t *dev_reg, word status);
#endif
