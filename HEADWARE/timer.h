#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"

void TIM2_Config(uint16_t PeriodValue, uint16_t PrescalerValue);
void TIM3_Config(uint16_t PeriodValue, uint16_t PrescalerValue);

#endif

