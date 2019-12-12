#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "types.h"

#define DEV_FULL_MASK 				0xFFFFFFFF

#define DEV_STATUS_NOT_INSTALLED 	0
#define DEV_STATUS_READY 			1
#define DEV_STATUS_UNKNOWN_CMD 		2
#define DEV_STATUS_BUSY 			3

#define DEV_CMD_RESET 				0
#define DEV_CMD_ACK 				1


word Dev_GetStatus( devreg_t *device, word mask );

void Dev_SetCommand( devreg_t *device, word cmd );

word Dev_GetData0( devreg_t *device, word mask );

word Dev_GetData1( devreg_t *device, word mask );


#endif