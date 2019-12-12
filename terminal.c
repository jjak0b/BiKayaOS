#include "terminal.h"

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
    if (stat != DEV_STATUS_READY && stat != TERMINAL_STATUS_TRANSMITTED )
        return -1;

    device->transm_command = ((ch << CHAR_OFFSET) | TERMINAL_CMD_TRANSMIT);

    while ((stat = tx_status(device)) == DEV_STATUS_BUSY)
        ;

    device->transm_command = DEV_CMD_ACK;

    if (stat != TERMINAL_STATUS_TRANSMITTED)
        return -1;
    else
        return 0;
}

void terminal_puts(termreg_t *device, const char *str) {
    while (*str)
        if (terminal_putchar( device, *str++))
            return;
}

int terminal_getchar(termreg_t *device, char *str ){
    word stat;

    stat = rx_status(device);

    if (stat != DEV_STATUS_READY && stat != TERMINAL_STATUS_RECEIVED )
        return -1;

    device->recv_command = TERMINAL_CMD_RECEIVE;

    while ((stat = rx_status(device)) == DEV_STATUS_BUSY )
        ;

    *str = rx_data( device );

    device->recv_command = DEV_CMD_ACK;

    if (stat != TERMINAL_STATUS_RECEIVED)
        return -1;
    else
        return 0;
}

int terminal_gets(termreg_t *device, char *str_buffer, unsigned int size_str, unsigned int *length_str ) {
    *length_str = 0;
    int status = 0;
    unsigned int length = 0;
    bool flag_continue = true;
    bool flag_EOL = false;

    if ( size_str > 0 ){
        size_str -= 1; // riservato per CHAR fine stringa
        while( flag_continue && !status && length < size_str ){
            status = terminal_getchar( device, str_buffer );
            if( !status ){
                if( *str_buffer != EOL ){
                    str_buffer++;
                    length++;
                }
                else{
                    flag_EOL = true;
                    flag_continue = false;
                }
            }
            else{
                flag_continue = false;
            }
        }
    }
    *str_buffer = '\0';
    
    if ( length_str != NULL ){
        *length_str = length;
    }
    
    if ( status == 0 ){
        if( !flag_EOL ){
            status = 1;
        }
    }

    return status;
}