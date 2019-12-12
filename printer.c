#include "printer.h"
#include "device.h"
#include "system.h"

int printer_putchar( dtpreg_t *device, char ch ){
    word stat = Dev_GetStatus( (devreg_t*)device, DEV_FULL_MASK );

    if (stat != DEV_STATUS_READY ){
        return -1;
    }

	device->data0 = (word)ch;
	Dev_SetCommand( (devreg_t*)device, PRINTER_CMD_PRINT );
	
    while ( (stat = Dev_GetStatus( (devreg_t*)device, DEV_FULL_MASK ) ) == DEV_STATUS_BUSY )
        ;

	Dev_SetCommand( (devreg_t*)device, DEV_CMD_ACK );

    if (stat == PRINTER_STATUS_PRINT_ERROR )
        return -1;
    else
        return 0;
}

void printer_puts( dtpreg_t *device, const char *str ){
	bool flag_continue = true;
	while( flag_continue && str != NULL && *str != '\0' ){
		if( printer_putchar( device, *str++ ) ){
			flag_continue = false;
		}
	}
}