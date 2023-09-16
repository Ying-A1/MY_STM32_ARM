#include "iwdg.h"

//初始化独立看门狗
//prer:分频数:0~7(只有低 3 位有效!)
//分频因子=4*2^prer.但最大值只能是 256!
//rlr:重装载寄存器值:低 11 位有效.
//时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
void IWDG_Init(u8 prer,u16 rlr)
{
	// 1、取消寄存器写保护 写0x5555
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	// 2、设置独立看门狗预分频系数
	IWDG_SetPrescaler(prer);
	// 3、设置独立看门狗重装载值
	IWDG_SetReload(rlr);;
	// 4、重载计数值喂狗 写0xAAAA
	IWDG_ReloadCounter();
	// 5、启动看门狗     写0xCCCC
	IWDG_Enable();
}

void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}
