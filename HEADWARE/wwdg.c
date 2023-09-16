#include "wwdg.h"

//窗口值范围 0x40 ~ 0x7f 
u8 WWDG_CNT = 0x7f;
void WWDG_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

  
//初始化窗口看门狗
//tr :T[6:0],计数器值 tr要在窗口值之间
//wr :W[6:0],上窗口值  0x40 ~ 0x7f = 0b 0111 1111
//fprer:分频系数（WDGTB）,仅最低 2 位有效
//Fwwdg=PCLK1/(4096*2^fprer)
void WWDG_Init(u8 tr, u8 wr, u32 fprer)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE); //时钟使能
    WWDG_CNT = tr & WWDG_CNT;
    WWDG_SetPrescaler(fprer);			//设置预分频值 使用频率：32M/4096/fprer
    WWDG_SetWindowValue(wr);			//设置窗口值 上窗口值  与喂狗间隔没关系
    WWDG_Enable(WWDG_CNT);			//使能看门狗，设置计数值
    WWDG_ClearFlag();				//清除提前唤醒中断
    WWDG_NVIC_Init();				//设置中断优先级
    WWDG_EnableIT();				//开启窗口看门狗中断
}

void WWDG_Set_Counter(u8 cnt)
{
    WWDG_Enable(cnt);				//使能看门狗，设置计数值
}

void WWDG_IRQHandler(void)
{
    WWDG_SetCounter(WWDG_CNT);		       //喂狗
    WWDG_ClearFlag();				//清除提前唤醒中断
    
}
