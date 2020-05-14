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
