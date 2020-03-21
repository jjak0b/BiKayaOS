#include <pcb/pcb.h>

/**
 * @brief   rimuove tutti i figli di old_parent, aggiungendoli a quelli di new_parent
 * @PostCondition   Se new_parent == NULL, allora i figli di old_parent saranno senza fratelli e senza padre
 * @param old_parent genitore di cui si vuole rimuovere i figli
 * @param new_parent il nuovo genitore dei figli
 */     
void pcb_SetChildrenParent( pcb_t* old_parent, pcb_t* new_parent );

/**
 * @brief   Dealloca dopo aver rimosso il descrittore fornito e tutta la sua progenie dalla queue della sentinella fornita
 * @PostCondition   Non avviene alcuna rimozione nella lista dei fratelli e del padre di p.
 * 
 * @param p descrittore del processo da cui partire a rimuovere la progenie
 * @param head queue in cui deve essere rimosso p e tutta la sua progenie
 */
void pcb_RemoveProgenyQ( pcb_t* p, struct list_head *head );