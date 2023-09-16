#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"

#define BEEP_OFF    GPIO_ResetBits(GPIOB,GPIO_Pin_2)
#define BEEP_ON  GPIO_SetBits(GPIOB,GPIO_Pin_2)

void BEEP_Init(void);

#endif

