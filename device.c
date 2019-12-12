#include "device.h"

word Dev_GetStatus( devreg_t *device, word mask ){
	return device->dtp.status & mask;
}

void Dev_SetCommand( devreg_t *device, word cmd ){
	device->dtp.command = cmd;
}

word Dev_GetData0( devreg_t *device, word mask ){
	return device->dtp.data0 & mask;
}

word Dev_GetData1( devreg_t *device, word mask ){
	return device->dtp.data1 & mask;
}