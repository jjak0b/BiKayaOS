#ifndef __PRINTER_H__
#define __PRINTER_H__

#include "device.h"

#define PRINTER_STATUS_PRINT_ERROR 4
#define PRINTER_CMD_PRINT 2

/**
 * @brief Invia al dispositivo stampante una stringa da stampare
 * @PreCondition: device != NULL
 * @param device 
 * @param str 
 */
void printer_puts( dtpreg_t *device, const char *str );

#endif