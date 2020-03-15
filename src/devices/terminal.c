#include <devices/terminal.h>

word tx_status(termreg_t *device) {
    return ((device->transm_status) & TERMINAL_STATUS_MASK);
}

word rx_status(termreg_t *device) {
    return ((device->recv_status) & TERMINAL_STATUS_MASK);
}

word rx_data(termreg_t *device) {
    return (device->recv_status >> CHAR_OFFSET) & TERMINAL_STATUS_MASK;
}

int terminal_putchar(termreg_t *device, char ch ) {
    word stat;

    stat = tx_status(device);
    if (stat != DEV_STATUS_READY && stat != TERMINAL_STATUS_TRANSMITTED){
        return -1;
    }

    device->transm_command = ((ch << CHAR_OFFSET) | TERMINAL_CMD_TRANSMIT);

    while ((stat = tx_status(device)) == DEV_STATUS_BUSY)
        ;

    device->transm_command = DEV_CMD_ACK;

    if (stat != TERMINAL_STATUS_TRANSMITTED){
        return -1;
    }
    
    return 0;
}

void terminal_puts(termreg_t *device, const char *str){
    while (*str){
        if (terminal_putchar(device, *str++)){
            return;
        }
    }
}

int terminal_getchar(termreg_t *device, char *str ){
    word stat;

    stat = rx_status(device);

    if (stat != DEV_STATUS_READY && stat != TERMINAL_STATUS_RECEIVED){
        return -1;
    }
    
    /* invio comando per far scrivere il carattere inserito sul buffer */
    device->recv_command = TERMINAL_CMD_RECEIVE;

    /* attendo che elabori la richiesta */
    while ((stat = rx_status(device)) == DEV_STATUS_BUSY )
        ;

    /* leggo il carattere scritto sul buffer di input */
    *str = rx_data( device );

    device->recv_command = DEV_CMD_ACK; /* rendo disponibile il device */

    if(stat != TERMINAL_STATUS_RECEIVED){
        return -1;
    }

    return 0;
}

int terminal_gets(termreg_t *device, char *str_buffer, unsigned int size_str) {    
    if(!size_str){
        return 0;
    }

    size_str = size_str - 1; /* riservato per CHAR fine stringa */

    int             status = 0;
    unsigned int    length = 0;
    bool flag_continue  = true;
    bool flag_EOL       = false;

    /* Continua a riempire il buffer finchè:
     * 1) c'è spazio; 
     * 2) non sono avvenuti errori; 
     * 3) non trova \n.
     * 
     * Se non c'è più spazio sul buffer fornito 
     * lo indica nel flag di ritorno.
    */
    
    while(flag_continue && size_str>length){
        if((status = terminal_getchar(device, str_buffer))){ /* errore nel recupero del carattere */
            break;
        }

        if(*str_buffer != EOL){
            str_buffer++;
            length++;
        }else {
            flag_EOL        = true;
            flag_continue   = false;
        }
    }

    *str_buffer = '\0';

    /*Se ci sono stati dei problemi                RETURN -1 ;
    * Se non è stata raggiunta la fine della linea RETURN 1 ;
    * In caso contrario                            RETURN 0
    */
    
   return status ? -1 : !flag_EOL;
}