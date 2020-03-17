#include "types_bikaya.h"

void moveState(state_t *before, state_t *after);
void initStatusFlag(state_t *state);

void initAreas(void);
void initSysCallArea(state_t *area, devregarea_t *devregarea);
void initTrapArea(state_t *area, devregarea_t *devregarea);
void initTLBArea(state_t *area, devregarea_t *devregarea);
void initInterruptArea(state_t *area, devregarea_t *devregarea);