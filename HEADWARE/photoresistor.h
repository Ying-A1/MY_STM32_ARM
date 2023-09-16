//photoresistor
#ifndef __PHOTORESISTOR_H
#define __PHOTORESISTOR_H

#include "stm32f10x.h"

void PR_Init(void);
uint16_t ReadADC(void);

#endif
