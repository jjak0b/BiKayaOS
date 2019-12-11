#include "terminal.h"
#include "system.h"

#define SIZE_BUFFER 1024

int main(void) {
    char buffer[ SIZE_BUFFER ];
    int str_length = 0;

    term_puts("Input To send:\n");

    int keep_get = 1;
    do{
        keep_get = term_gets( buffer, SIZE_BUFFER, &str_length );
        term_puts(buffer);
        // print( buffer );
    }
    while( keep_get );

    /* Go to sleep indefinetely */
    while (1) 
        WAIT();
    return 0;
}

