#include "system.h"
#include "terminal.h"
#include "printer.h"

#define SIZE_BUFFER 1024

int main(void) {
    char buffer[SIZE_BUFFER];

    dtpreg_t *dev_print0_reg = (dtpreg_t*)DEV_REG_ADDR(IL_PRINTER, 0);
    termreg_t *dev_term0_reg = (termreg_t *)DEV_REG_ADDR(IL_TERMINAL, 0);

    terminal_puts(dev_term0_reg, "Input to send:\n");
    
    int keep_get;
    int print_err;
    
    /*L'op viene ripetuta se:
    * 1) Viene letto parte dell'input e il buffer si è riempito
    * 2) E' stato stampato sulla stampante la parte dell'input scandito
    */

    do{
        keep_get   = terminal_gets(dev_term0_reg, buffer, SIZE_BUFFER);
        print_err  = printer_puts(dev_print0_reg, buffer);
    }
    while(keep_get==1 && !print_err);

    if(!keep_get){
        terminal_puts(dev_term0_reg, "Input sent successfully\n");
    }else{
        terminal_puts(dev_term0_reg, "Terminal busy or in error\n");
    }

    if(print_err){
        terminal_puts(dev_term0_reg, "Printer stops printing unexpectedly\n");
    }else{
        terminal_puts(dev_term0_reg, "Input printed successfully\n");
    }

    /* Go to sleep indefinetely */
    while (1){
        WAIT();
    }
    
    return 0;
}

