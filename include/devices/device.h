#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <utilities/types.h>

#define DEV_FULL_MASK 				0xFFFFFFFF

// stati generici di un dispositivo
#define DEV_STATUS_NOT_INSTALLED 	0
#define DEV_STATUS_READY 			1
#define DEV_STATUS_UNKNOWN_CMD 		2
#define DEV_STATUS_BUSY 			3

// comandi generici di un dispositivo
#define DEV_CMD_RESET 				0
#define DEV_CMD_ACK 				1

/**
 * @brief Restituisce lo stato del device applicando una specifica maschera
 * @PreCondition solo su dispositivo di tipo non terminale, device != NULL
 */
word Dev_GetStatus( devreg_t *device, word mask );

/**
 * @brief Assegna un comando al device fornito
 * @PreCondition solo su dispositivo di tipo non terminale, device != NULL
 */
void Dev_SetCommand( devreg_t *device, word cmd );

/**
 * @brief Restituisce la word del dispositivo dedicata al data0
 * @PreCondition solo su dispositivo di tipo non terminale, device != NULL
 */
word Dev_GetData0( devreg_t *device, word mask );

/**
 * @brief Restituisce la word del dispositivo dedicata al data1
 * @PreCondition solo su dispositivo di tipo non terminale, device != NULL
 */
word Dev_GetData1( devreg_t *device, word mask );


#endif