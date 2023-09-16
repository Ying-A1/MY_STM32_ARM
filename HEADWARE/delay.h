/*********************************************

*********************************************/
#ifndef _DELAY_H
#define _DELAY_H

#include "stm32f10x.h"


void delay_Init(u8 SYSCLK);
void delay_us(u32 nus);
void delay_ms(u16 nms);
void delay_s(u16 ns);


#endif
