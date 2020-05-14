/***********************************pcb.c****************************************
*                   - Bikaya OS - Phase 1 - Modulo PCB -
*    @credit: 
*    Stefano De Santis, Cristiano Guidotti, Iacopo Porcedda, Jacopo Rimediotti
*/

#include <pcb.h>

#include <types_bikaya.h>
#include <const.h>

// puntatore alla sentinella della lista pcbFree
static struct list_head *pcbFree_h;

/*
    Inizializza la pcbFree in
    modo da contenere tutti gli elementi della
    pcbFree_table. Questo metodo deve essere
    chiamato una volta sola in fase di
    inizializzazione della struttura dati.
*/
void initPcbs(void){
    //array di PCB con dimensione massima di MAX_PROC
    static pcb_t pcbFree_table[MAXPROC];
    
    //lista dei PCB liberi o inutilizzati
    static pcb_t *pcbFree;
    
    /* creazione sentinella lista pcbFree */
    static pcb_t dummy;
    INIT_LIST_HEAD(&(dummy.p_next));
    pcbFree = &dummy;
    
    pcbFree_h = &(pcbFree->p_next);

    //aggiunta di tutti i pcb nella lista dei pcb liberi
    for(int i = 0; i < MAXPROC; i++){
        freePcb(&pcbFree_table[i]);
    }
}

/*
    Inserisce il PCB
    puntato da p nella lista dei PCB liberi
    (pcbFree)
*/
void freePcb(pcb_t *p){
    insertProcQ(pcbFree_h, p);
}

/*
    Inserisce l’elemento puntato da p nella
    coda dei processi puntata da head. L’inserimento deve
    avvenire tenendo conto della priorita’ di ciascun pcb
    (campo p->priority). La coda dei processi deve essere
    ordinata in base alla priorita’ dei PCB, in ordine
    decrescente (i.e. l’elemento di testa è l’elemento con la
    priorita’ più alta).    
*/
void insertProcQ(struct list_head *head, pcb_t *p){
    INIT_LIST_HEAD(&(p->p_next));

    /*CASO 1: La coda è vuota*/
    if(emptyProcQ(head)){
        list_add(&(p->p_next), head);
        return;
    }

    int major_priority = (container_of(list_next(head),pcb_t,p_next))->priority;
    int minor_priority = (container_of(list_prev(head),pcb_t,p_next))->priority;
    
    /*CASO 2: Il processo ha la priorità più alta*/
    if(p->priority > major_priority){
        list_add(&(p->p_next), head);
        return;
    }
    
    /*CASO 3: Il processo ha la priorità più bassa*/
    if(p->priority <= minor_priority){
        list_add_tail(&(p->p_next), head);
        return;
    }
    
    /*CASO 4*/
    struct list_head* i;
    list_for_each(i, head){
        pcb_t *q = container_of(i, pcb_t, p_next);
        if((p->priority)>(q->priority)){
            list_add(&(p->p_next), list_prev(i));
            return;
        }
    }
}

/*
    Restituisce NULL se la
    pcbFree è vuota. Altrimenti rimuove
    un elemento dalla pcbFree, inizializza
    tutti i campi (NULL/0) e restituisce
    l’elemento rimosso.
*/
pcb_t *allocPcb(void){
    pcb_t *new = removeProcQ(pcbFree_h);
    pcb_init( new );
    return new;
}

/*
    inizializza
    tutti i campi (NULL/0)
    e le liste come liste vuote
*/
void pcb_init( pcb_t *new ){
    if(new != NULL){
        //inizializzazione pcb    
        INIT_LIST_HEAD(&(new->p_child));
        INIT_LIST_HEAD(&(new->p_sib));
        INIT_LIST_HEAD(&(new->p_next));
        new->p_parent = NULL;
        new->p_semkey = NULL;
        new->priority = 0;
        new->first_activation_tod = 0;
        new->chrono_start_tod = 0;
        new->kmode_timelapse = 0;
        new->umode_timelapse = 0;
#ifdef TARGET_UMPS
        new->p_s.entry_hi = 0;
        new->p_s.cause = 0;
        new->p_s.status = 0;
        new->p_s.pc_epc = 0;
        new->p_s.hi = 0;
        new->p_s.lo = 0;
        int i;
        for( i = 0; i < STATE_GPR_LEN; i++ ){
            new->p_s.gpr[ i ] = 0;
        }
#endif
#ifdef TARGET_UARM
        new->p_s.a1 = 0;
        new->p_s.a2 = 0;
        new->p_s.a3 = 0;
        new->p_s.a4 = 0;
        new->p_s.v1 = 0;
        new->p_s.v2 = 0;
        new->p_s.v3 = 0;
        new->p_s.v4 = 0;
        new->p_s.v5 = 0;
        new->p_s.v6 = 0;
        new->p_s.sl = 0;
        new->p_s.fp = 0;
        new->p_s.ip = 0;
        new->p_s.sp = 0;
        new->p_s.lr = 0;
        new->p_s.pc = 0;
        new->p_s.cpsr = 0;
        new->p_s.CP15_Control = 0;
        new->p_s.CP15_EntryHi = 0;
        new->p_s.CP15_Cause = 0;
        new->p_s.TOD_Hi = 0;
        new->p_s.TOD_Low = 0;
#endif
    }
}

/* PCB QUEUE HANDLING */

/*
    Inizializza la lista dei PCB,
    inizializzando l’elemento sentinella.
*/
void mkEmptyProcQ(struct list_head *head){
    INIT_LIST_HEAD(head);
}
/*
    Restituisce TRUE se la
    lista puntata da head è vuota, FALSE
    altrimenti.    
*/
int emptyProcQ(struct list_head *head){
    return list_empty(head);
}

/*
    Restituisce l’elemento di testa della
    coda dei processi da head, SENZA RIMUOVERLO.
    Ritorna NULL se la coda non ha elementi.
*/
pcb_t *headProcQ(struct list_head *head){
    if(emptyProcQ(head)){
        return NULL;
    }
    return container_of(head->next, pcb_t, p_next);
}

/*
    Rimuove il primo elemento dalla
    coda dei processi puntata da head. Ritorna
    NULL se la coda è vuota. Altrimenti ritorna il
    puntatore all’elemento rimosso dalla lista.
*/
pcb_t *removeProcQ(struct list_head *head){
    pcb_t *item = headProcQ(head);
    if(item != NULL){
        list_del(&(item->p_next));
        INIT_LIST_HEAD(&(item->p_next));
    }
    return item;
}

/*
    Rimuove il PCB puntato da p
    dalla coda dei processi puntata da head. Se p
    non è presente nella coda, restituisce NULL.
    (NOTA: p può trovarsi in una posizione arbitraria)
*/
pcb_t *outProcQ(struct list_head *head, pcb_t *p){
    if(!emptyProcQ(head)){
        struct list_head *it;
    
        list_for_each(it, head){
            pcb_t* item = container_of(it, pcb_t, p_next);
            if(item == p){
                list_del(it);
                INIT_LIST_HEAD(it);
                return p;
            }
        }
    }
    return NULL;
}

/* PCB TREE HANDLING */

/*
    restituisce TRUE se il PCB
    puntato da p non ha figli, restituisce FALSE
    altrimenti.
*/
int emptyChild(pcb_t *this) {
	return list_empty( &this->p_child ); 
}

/*
    Inserisce il PCB puntato da p
    come figlio del PCB puntato da prnt.
*/
void insertChild(pcb_t *prnt, pcb_t *p) {
    INIT_LIST_HEAD(&p->p_sib);
    if( emptyChild( prnt ) ){
        INIT_LIST_HEAD(&prnt->p_child);
    }

	/* Inserirsce 'p' in coda alla lista dei figli di 'prnt' */
	list_add_tail( &p->p_sib, &prnt->p_child );
	p->p_parent = prnt;
}

/*
    Rimuove il primo figlio del
    PCB puntato da p. Se p non ha figli,
    restituisci NULL    
*/
pcb_t *removeChild(pcb_t *p) {
	if ( emptyChild( p ) )
		return NULL;
	else {
		/* Rimozione del primo nodo */
		pcb_t *removedChild = container_of( p->p_child.next, pcb_t, p_sib );
		list_del( &removedChild->p_sib );
		
		/* Reset dei puntatori del nodo rimosso */
		removedChild->p_parent = NULL;
		INIT_LIST_HEAD( &removedChild->p_sib );
		
		return removedChild;
	}
}

/*
    Rimuove il PCB
    puntato da p dalla lista dei figli del
    padre. Se il PCB puntato da p non ha
    un padre, restituisce NULL. Altrimenti
    restituisce l’elemento rimosso (cioè p).
    A differenza della removeChild, p può
    trovarsi in una posizione arbitraria
    (ossia non è necessariamente il primo
    figlio del padre).
*/
pcb_t *outChild(pcb_t *p) {
	
	if ( p->p_parent != NULL ) {
		/* Rimozione di 'p' dalla lista */
		list_del( &p->p_sib );
		
		/* Reset dei puntatori di 'p' */
		p->p_parent = NULL;
		INIT_LIST_HEAD( &p->p_sib );
		
		return p;
	}
	else return NULL;
}