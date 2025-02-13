#ifndef H_HANDLER
#define H_HANDLER

// Interrupt Handler functions and define
//-------------------------------------------------------
void Handler_Interrupt(void);

/**
 * @brief Gestore SYSCALL/BP. Richiama il gestore 
 * specifico per SYSCALL/BP se il registro di causa
 * dell'area SYS/BP è valorizzato con uno dei due valori.
 */
void Handler_SysCall(void);

/**
 * @brief Gestore specifico per SYSCALL.
 * @param request Puntatore alla SYS/BP New Area
 */
void handle_syscall(state_t *request);

/**
 * @brief Gestore specifico per BREAKPOINT.
 * @param request Puntatore alla SYS/BP New Area
 */
void handle_breakpoint(state_t *request);
//-------------------------------------------------------

// Trap Handler functions and define
//-------------------------------------------------------
void Handler_Trap(void);
//-------------------------------------------------------

// TLB Handler functions and define
//-------------------------------------------------------
void Handler_TLB(void);
//-------------------------------------------------------

#endif