#ifndef __TERMINAL_H__
#define __TERMINAL_H__

void term_puts(const char *str);

/**
 * @brief Riempie il buffer assegnato con un carattere presente in input sul terminale
 * @param str 
 * @return int 
 * 			0 se l'esecuzione è andata a buon fine
 * 			-1 altrimenti
 */
int term_getchar( char *str );

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
int term_gets(char *str_buffer, int size_str, int *length_str );
#endif