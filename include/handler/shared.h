#ifndef H_HANDLER_SHARED
#define H_HANDLER_SHARED

/*
descrizione da aggiungere
 */
int sys2_create( state_t *child_state, int child_priority, void **child_pid );

/*
descrizione da aggiungere
 */
int sys3_terminate( void *pid );

#endif
