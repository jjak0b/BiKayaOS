#include <pcb/pcb.h>
#include <pcb/utils.h>

void pcb_SetChildrenParent( pcb_t* old_parent, pcb_t* new_parent ) {

    while( !emptyChild( old_parent ) ) {
        pcb_t *child = removeChild( old_parent );
        if( new_parent != NULL ){
            insertChild( new_parent, child );
        }
    }
}

void pcb_RemoveProgenyQ( pcb_t* p, struct list_head *head ) {
    if( p == NULL )
        return;

    /**
     * Aggiunge ricorsivamente i figli di ogni processo ad uno stack di lavoro
     * e poi elimina il padre sia dalla queue
     * che dallo stack stesso, con conseguente deallocazione
     */
	LIST_HEAD( pcb_stack );
	list_add( &p->p_next, &pcb_stack );

	while( !list_empty( &pcb_stack ) ) {
		struct list_head *it_parent = list_next( &pcb_stack );
		pcb_t *parent = container_of( it_parent, pcb_t, p_next );
		while( !emptyChild( parent ) ){
			pcb_t* child = removeChild( parent );

            if( head != NULL ){
		        outProcQ( head, parent );
            }
            
			list_add( &child->p_next, &pcb_stack );
		}
		list_del( &parent->p_next );
		freePcb( parent );
	}
}