#include <pcb/pcb.h>

/**
 * @brief   rimuove tutti i figli di old_parent, aggiungendoli a quelli di new_parent
 * @PostCondition   Se new_parent == NULL, allora i figli di old_parent saranno senza fratelli e senza padre
 * @param old_parent genitore di cui si vuole rimuovere i figli
 * @param new_parent il nuovo genitore dei figli
 */     
void pcb_SetChildrenParent( pcb_t* old_parent, pcb_t* new_parent );
