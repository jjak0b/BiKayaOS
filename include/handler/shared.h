#ifndef H_HANDLER_SHARED
#define H_HANDLER_SHARED

/**
 * @brief Implementazione della syscall TERMINATE_PROCESS.
 * Termina il processo corrente e tutta la sua progenie.
 * Infine, richiama lo scheduler.
 */
void sys3_terminate(void);
#endif