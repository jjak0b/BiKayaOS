#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include "device.h"

#define TERMINAL_STATUS_TRANSMITTED     5
#define TERMINAL_STATUS_RECEIVED TERMINAL_STATUS_TRANSMITTED

#define TERMINAL_CMD_TRANSMIT       2    
#define TERMINAL_CMD_RECEIVE TERMINAL_CMD_TRANSMIT

#define CHAR_OFFSET        8
#define TERMINAL_STATUS_MASK   0xFF
#define TERMINAL_DATA_MASK   0xFF00

#define EOL '\n'

void terminal_puts(termreg_t *device, const char *str);

/**
 * @brief Riempie il buffer assegnato con un carattere presente in input sul terminale
 * @param str 
 * @return int 
 * 			0 se l'esecuzione è andata a buon fine
 * 			-1 altrimenti
 */
int terminal_getchar(termreg_t *device, char *str );

/**
 * @brief Riempie il buffer con una stringa C,
 * 			fornendone la lunghezza
 * 			restituisce un intero indicandone lo stato dell'esecuzione
 * @param str_buffer 
 * @param size_str 
 * @param length_str 
 * @return int 
 * 			0	se una stringa intera è stata memorizzata nel buffer
 * 			1	se avanza dell'input che il buffer non riesce a contenere -> richiamare nuovamente la funzione per continuare
 * 			-1	se è avvenuto un errore
 */
int terminal_gets(termreg_t *device, char *str_buffer, unsigned int size_str, unsigned int *length_str );
#endif