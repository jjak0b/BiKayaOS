#include "system.h"

#define STATUS_READY           1
#define STATUS_BUSY            3
#define STATUS_TRANSMITTED     5
#define STATUS_RECEIVED STATUS_TRANSMITTED

#define CMD_ACK            1
#define CMD_TRANSMIT       2    
#define CMD_RECEIVE CMD_TRANSMIT

#define CHAR_OFFSET        8
#define TERMINAL_STATUS_MASK   0xFF
#define TERMINAL_DATA_MASK   0xFF00

#define EOL '\n'

static termreg_t *term0_reg = (termreg_t *) DEV_REG_ADDR(IL_TERMINAL, 0);

static unsigned int tx_status(termreg_t *tp) {
    return ((tp->transm_status) & TERMINAL_STATUS_MASK);
}

static unsigned int rx_status(termreg_t *tp) {
    return ((tp->recv_status) & TERMINAL_STATUS_MASK);
}

static unsigned int rx_data(termreg_t *tp) {
    return (tp->recv_status >> CHAR_OFFSET) & TERMINAL_STATUS_MASK;
}

int term_putchar(char c) {
    word stat;

    stat = tx_status(term0_reg);
    if (stat != STATUS_READY && stat != STATUS_TRANSMITTED )
        return -1;

    term0_reg->transm_command = ((c << CHAR_OFFSET) | CMD_TRANSMIT);

    while ((stat = tx_status(term0_reg)) == STATUS_BUSY)
        ;

    term0_reg->transm_command = CMD_ACK;

    if (stat != STATUS_TRANSMITTED)
        return -1;
    else
        return 0;
}


void term_puts(const char *str) {
    while (*str)
        if (term_putchar(*str++))
            return;
}

int term_getchar( char *str ){
    word stat;

    stat = rx_status(term0_reg);

    if (stat != STATUS_READY && stat != STATUS_RECEIVED)
        return -1;

    term0_reg->recv_command = CMD_RECEIVE;

    while ((stat = rx_status(term0_reg)) == STATUS_BUSY )
        ;

    *str = rx_data( term0_reg );

    term0_reg->recv_command = CMD_ACK;

    if (stat != STATUS_RECEIVED)
        return -1;
    else
        return 0;
}

int term_gets(char *str_buffer, unsigned int size_str, unsigned int *length_str ) {
    *length_str = 0;
    int status = 0;
    unsigned int length = 0;
    bool flag_continue = true;
    bool flag_EOL = false;

    if ( size_str > 0 ){
        size_str -= 1; // riservato per CHAR fine stringa
        while( flag_continue && !status && length < size_str ){
            status = term_getchar( str_buffer );
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