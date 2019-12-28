#include "system.h"
#include "terminal.h"
#include "printer.h"

#define SIZE_BUFFER 1024

int main(void) {
    char buffer[ SIZE_BUFFER ];
    unsigned int str_length = 0;

    dtpreg_t *dev_print0_reg = (dtpreg_t*)DEV_REG_ADDR(IL_PRINTER, 0);
    termreg_t *dev_term0_reg = (termreg_t *)DEV_REG_ADDR(IL_TERMINAL, 0);

    terminal_puts(dev_term0_reg, "Input To send:\n");

    int keep_get = 1;
    // continua a riempire il buffer dell input sul terminale e lo manda alla stampante fino ad uno '\n'
    do{
        keep_get = terminal_gets(dev_term0_reg, buffer, SIZE_BUFFER, &str_length );
        // terminal_puts(dev_term0_reg, buffer); // DEBUG
        printer_puts(dev_print0_reg, buffer );
    }
    while( keep_get );

    if( !keep_get )
        terminal_puts(dev_term0_reg, "Input sent successfully\n");

    /* Go to sleep indefinetely */
    while (1) 
        WAIT();
    return 0;
}

