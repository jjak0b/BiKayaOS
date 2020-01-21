#include "pcb.h"
#include "types_bikaya.h"
#include "const.h"

static struct list_head pcbFree_h;

void initPcbs(void){
    static pcb_t pcbFree_table[ MAXPROC ];
    int i;
    for( i = 0; i < MAXPROC; i++ ){
        pcb_t *item = &( pcbFree_table[i] );
        item->p_parent = NULL;
        INIT_LIST_HEAD( &item->p_next );
        INIT_LIST_HEAD( &item->p_child );
        INIT_LIST_HEAD( &item->p_sib );
        item->priority = 0;
        item->p_semkey = NULL;
        item->p_s = {0};

        list_add( &item->p_next, &pcbFree_h );
    }
}

void freePcb(pcb_t *p){
    list_add( &p->p_next, &pcbFree_h );
}

pcb_t *allocPcb(void){
    pcb_t *new = NULL;
    if( !list_empty( &pcbFree_h ) ){ /* Se non è libera, allora rimuovo il primo elemento in testa */
        const pcb_t empty = {0};
        pcb_t *new = container_of( &pcbFree_h.next, pcb_t, p_next);
        list_del( &pcbFree_h.next );
        *new = empty;
    }
    return new;
}

void mkEmptyProcQ(struct list_head *head){
    INIT_LIST_HEAD(head);
}

int emptyProcQ(struct list_head *head){
    return list_empty( head );
}

void insertProcQ(struct list_head *head, pcb_t *p){
    if( list_empty( head ) ){
        list_add( &p->p_next, head );
    }
    else{
        int priority_tail = container_of( head->next, pcb_t, p_next )->priority;
        int priority_head = container_of( head->prev, pcb_t, p_next )->priority;

        /* aggiungo l'elemento a uno dei due estremi della lista in base alla priorità degli estremi*/
        if( p->priority >= priority_head  ){
            list_add( &p->p_next, head );
        }
        else if( p->priority <= priority_tail ){
            list_add_tail( &p->p_next, head );
        }
        else{ /* ricerco la posizione in cui inserirlo stimando un minor numero di iterazioni possibile per la ricerca */
            struct list_head* it; /* iteratore della lista */
            pcb_t *item; /*pcb che contiene l'iteratore*/

            if( p->priority >= (priority_head / 2) && (priority_head / 2) >= priority_tail ){ 
                list_for_each( it, head ){
                    item = container_of( it, pcb_t, p_next );
                    if( p->priority >= item->priority ){
                        list_add( &p->p_next, it );
                        break;
                    }
                }
            }
            else{
                list_for_each_prev( it, head ){
                    item = container_of( it, pcb_t, p_next );
                    if( p->priority >= item->priority ){
                        list_add( &p->p_next, it );
                        break;
                    }
                }
            }
        }
    }
}

pcb_t *headProcQ(struct list_head *head){
    if( emptyProcQ( head ) ){
        return NULL;
    }
    else{
        return container_of( head->next, pcb_t, p_next );
    }
}

pcb_t *removeProcQ(struct list_head *head){
    pcb_t *item = headProcQ( head );
    if( item != NULL ){
        list_del( head->next );
    }
    return item;
}

pcb_t *outProcQ(struct list_head *head, pcb_t *p){
    pcb_t* item = NULL;
    if( !emptyProcQ( head ) ){
        struct list_head *it;
        list_for_each( it, head ){
            if( it == &p->p_next ){
                item = container_of( it, pcb_t, p_next );
                list_del( it );
                break;
            }
        }
    }
    return item;
}
