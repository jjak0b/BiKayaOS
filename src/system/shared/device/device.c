#include <system/system.h>
#include <system/shared/device/device.h>

HIDDEN int _semdev[SEM_DEV_N];

void device_init() {
    /* semafori device inizializzati a 0, in modo che attendano fino alla risposta (interrupt) */
    int i;
    for( i = 0; i < SEM_DEV_N; i++ )
        _semdev[ i ] = 0;
}

void device_GetInfo( devreg_t *devreg, int *_line, int *_devNo ) {
    if( _line == NULL ) return;

    int line, devNo;
    for( line = DEV_IL_START; line < DEV_PER_INT ; line++ ) {
        if( !( (devreg_t*)DEV_REG_ADDR(line, 0) <= devreg && devreg <= (devreg_t*)DEV_REG_ADDR(line, N_EXT_IL-1 ) ) ) {
            continue;
        }
        *_line = line;
        if( _devNo == NULL ) return;

        for( devNo = 0; devNo < N_EXT_IL; devNo ++ ){
            if( devreg == (devreg_t*)DEV_REG_ADDR(line, devNo) ) {    
                *_devNo = devNo;
                return;
            }
        }
    }
}

int *device_GetSem( int devline, int devNo, int subDev ) {
    return &_semdev[ GET_SEM_INDEX_SUBDEV(devline, devNo, subDev) ];
}
