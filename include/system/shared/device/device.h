#ifndef __SHARED_DEVICE_H__
#define __SHARED_DEVICE_H__

#define DEV_FULL_MASK 				0xFFFFFFFF

// stati generici di un dispositivo
#define DEV_STATUS_NOT_INSTALLED 	0
#define DEV_STATUS_READY 			1
#define DEV_STATUS_UNKNOWN_CMD		2
#define DEV_STATUS_BUSY				3
#define DEV_STATUS_CODE_4			4
#define DEV_STATUS_CODE_5			5
#define DEV_STATUS_CODE_6			6
#define DEV_STATUS_CODE_7			7
#define DEV_STATUS_CODE_128			128
// comandi generici di un dispositivo
#define DEV_CMD_RESET 				0
#define DEV_CMD_ACK 				1

#define IS_DEV_IN_ERROR(status) (\
   (status&0xFF)==DEV_STATUS_NOT_INSTALLED || (status&0xFF)==DEV_STATUS_UNKNOWN_CMD || \
   (status&0xFF)==DEV_STATUS_CODE_4 || (status&0xFF)==DEV_STATUS_CODE_5 || \
   (status&0xFF)==DEV_STATUS_CODE_6 || (status&0xFF)==DEV_STATUS_CODE_7 || \
   (status&0xFF)==DEV_STATUS_CODE_128)

//
#define IS_IRQ_RAISED_FROM_I(line,i) ((i>=0 && i<8 && CDEV_BITMAP_ADDR(line)&(1U<<i)) != 0)

/* We are 8 type of interrupt lines. But, only 5
* lines are related to devices.
*/
#define SEM_DEV_N ((N_EXT_IL+1)*N_DEV_PER_IL)

//
#define GET_SEM_OFFSET(dev_reg,line) (line!=IL_TERMINAL ? (0) : (IS_TERM_READY(dev_reg->term.transm_status)? (0) : (1)))
//
#define GET_SEM_INDEX(dev_reg,line,device) (N_DEV_PER_IL*(line-N_EXT_IL+GET_SEM_OFFSET(dev_reg,line))+device)
#define GET_SEM_INDEX_SUBDEV(line,device,subdevice) (N_DEV_PER_IL*(line-N_EXT_IL+ ( line!=IL_TERMINAL ? 0 : subdevice ) )+device)
//
#define GET_DEV_STATUS(dev_reg, line) (line!=IL_TERMINAL ?(dev_reg->dtp.status):(GET_SEM_OFFSET(dev_reg,line) ?(dev_reg->term.recv_status):(dev_reg->term.transm_status)) ) 


void device_init();

/**
 * @brief Memorizza il tipo (linea) di interrupt e numero di device relativo al registro del device fornito
 * 
 * @param devreg indirizzo del registro del device
 * @param line tipo (linea) di interrupt
 * @param devNo numero del device
 */
void device_GetInfo( devreg_t *devreg, int *devline, int *devNo );

/**
 * @brief Restituisce il puntatore al semaforo dedicato al (sotto-)dispositivo del tipo specificato
 * 
 * @param devline 
 * @param devNo 
 * @param subDev 
 * @return int* 
 */
int *device_GetSem( int devline, int devNo, int subDev );

#endif


