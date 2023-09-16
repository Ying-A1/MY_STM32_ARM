#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

#define LED1_ON    GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define LED1_OFF   GPIO_SetBits(GPIOB,GPIO_Pin_10)

#define LED2_ON    GPIO_ResetBits(GPIOB,GPIO_Pin_11)
#define LED2_OFF   GPIO_SetBits(GPIOB,GPIO_Pin_11)


void LED_Init(void);

#endif

