/***********************************asl.c****************************************
*                   - Bikaya OS - Phase 1 - Modulo ASL -
*    @credit: 
*    Stefano De Santis, Cristiano Guidotti, Iacopo Porcedda, Jacopo Rimediotti
*/

#include "asl.h"
#include "pcb.h"

#include "const.h"
#include <types_bikaya.h>

// puntatore alla sentinella della lista ASL
static struct list_head *semd_h;
// puntatore alla sentinella della lista semdFree
static struct list_head *semdFree_h;

/*
    Restituisce il puntatore al SEMD nella
    ASL la cui chiave è pari a key. Se non esiste un elemento
    nella ASL con chiave eguale a key, viene restituito NULL.
    
    key: chiave del semaforo
*/
semd_t* getSemd(int *key){
    struct list_head* iter;
    list_for_each(iter, semd_h){
        semd_t *s= container_of(iter,semd_t,s_next);
        if(s->s_key == key){
            return s;
        }
    }
    return NULL; 
}
/*
    Inizializza ASL
*/
void initASL(){
    //array di SEMD con dimensione massima di MAX_PROC
    static semd_t semd_table[MAXPROC];
    //Lista dei SEMD liberi o inutilizzati
    static semd_t *semdFree;

    /*creazione sentinelle liste asl e semdfree*/
    static semd_t dummy4Free;
    static semd_t dummy4ASL;
    INIT_LIST_HEAD(&(dummy4ASL.s_next));
    INIT_LIST_HEAD(&(dummy4Free.s_next));
    
    semd_h = &(dummy4ASL.s_next);
    semdFree = &dummy4Free;

    for(int i=0;i<MAXPROC; i++){
        list_add_tail(&(semd_table[i].s_next), &(semdFree->s_next));
    }

    semdFree_h = &(semdFree->s_next);
}

/*
    Viene inserito il PCB puntato da p nella
    coda dei processi bloccati associata al SEMD con chiave
    key. Se il semaforo corrispondente non è presente nella
    ASL, alloca un nuovo SEMD dalla lista di quelli liberi
    (semdFree) e lo inserisce nella ASL, settando I campi in
    maniera opportuna (i.e. key e s_procQ). Se non è
    possibile allocare un nuovo SEMD perché la lista di
    quelli liberi è vuota, restituisce TRUE. In tutti gli altri
    casi, restituisce FALSE.

    key: chiave del semaforo
    p: pcb da aggiungere alla coda dei processi bloccati
*/
int insertBlocked(int *key,pcb_t* p){
    struct semd_t* iter;
    iter = getSemd(key);

    if(iter == NULL){/*necessario allocare nuovo semd*/
        if(emptyProcQ(semdFree_h)){/*nessun semd disponibile*/
            return TRUE;
        }

        iter = container_of(list_next(semdFree_h),semd_t,s_next);
        
        // inizializzazione
        iter->s_key = key;
        INIT_LIST_HEAD(&(iter->s_procQ));
        // rimozione da semdFree
        list_del(&(iter->s_next));
        // aggiunta ad ASL
        list_add_tail(&(iter->s_next), semd_h);
    }

    p->p_semkey = key;
    // aggiunta processo in coda
    insertProcQ(&(iter->s_procQ),p);

    return FALSE;
}

/*
    Ritorna il primo PCB dalla
    coda dei processi bloccati (s_ProcQ)
    associata al SEMD della ASL con chiave key.
    Se tale descrittore non esiste nella ASL,
    restituisce NULL. Altrimenti, restituisce
    l’elemento rimosso. Se la coda dei processi
    bloccati per il semaforo diventa vuota,
    rimuove il descrittore corrispondente dalla
    ASL e lo inserisce nella coda dei descrittori
    liberi (semdFree).

    key: chiave del semaforo
*/
pcb_t* removeBlocked(int *key){
    semd_t *semd;
    semd = getSemd(key);
    
    if(semd == NULL){
        return NULL;
    }

    pcb_t *pcb = removeProcQ(&(semd->s_procQ));
    if(emptyProcQ(&(semd->s_procQ))){/*semaforo libero*/
        list_del(&(semd->s_next));
        list_add_tail(&(semd->s_next), semdFree_h);        
    }
    return pcb;
}
/*
    Rimuove il PCB puntato da p dalla coda del
    semaforo su cui è bloccato (indicato da p->p_semKey). Se il
    PCB non compare in tale coda, allora restituisce NULL
    (condizione di errore). Altrimenti, restituisce p. Se la coda
    dei processi bloccati per il semaforo diventa vuota,
    rimuove il descrittore corrispondente dalla ASL e lo
    inserisce nella coda dei descrittori liberi (semdFree).

    p: pcb da rimuovere
*/
pcb_t* outBlocked(pcb_t *p){
    semd_t *semd;
    semd = getSemd(p->p_semkey);
    
    if(semd == NULL){
        return NULL;
    }

    pcb_t *pcb = outProcQ(&(semd->s_procQ),p);
    if(emptyProcQ(&(semd->s_procQ))){/*semaforo libero*/
        list_del(&(semd->s_next));
        list_add_tail(&(semd->s_next), semdFree_h);        
    }
    return pcb;
}
/*
    Restituisce (senza rimuovere) il puntatore al
    PCB che si trova in testa alla coda dei processi associata al
    SEMD con chiave key. Ritorna NULL se il SEMD non
    compare nella ASL oppure se compare ma la sua coda dei
    processi è vuota.

    key: chiave del semaforo
*/
pcb_t* headBlocked(int *key){
    semd_t *semd;
    semd = getSemd(key);

    if(semd == NULL){
        return NULL;
    }

    return headProcQ(&(semd->s_procQ));
}
/*
    Rimuove il PCB
    puntato da p dalla coda del semaforo
    su cui è bloccato (indicato da
    p->p_semKey). Inoltre, elimina tutti i
    processi dell’albero radicato in p (ossia
    tutti i processi che hanno come avo p)
    dalle eventuali code dei semafori su
    cui sono bloccati.

    p: pcb da rimuovere
*/
void outChildBlocked(pcb_t *p){
    pcb_t *out = outBlocked(p);
    if(out==NULL){
        return;
    }

    pcb_t *child;
    list_for_each_entry(child,&(out->p_child),p_sib){
        outChildBlocked(child);
    }
}