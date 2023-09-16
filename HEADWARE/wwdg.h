#ifndef __WWDG_H
#define __WWDG_H

#include "stm32f10x.h"

void WWDG_NVIC_Init(void);
void WWDG_Init(u8 tr, u8 wr, u32 fprer);
void WWDG_Set_Counter(u8 cnt);

#endif
