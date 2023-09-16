#include "key.h"

//PB12 PB13 PB14 PB15
void Key_Init(void)
{
	//1、申请结构体参数
	GPIO_InitTypeDef  GPIO_Struct;

	//2、使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	//3、配置结构体参数
	GPIO_Struct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;  //引脚
	GPIO_Struct.GPIO_Mode = GPIO_Mode_IPU;  //带上拉电阻输入

	//4、使能GPIO功能
	GPIO_Init(GPIOB, &GPIO_Struct);

}

//按键扫描功能，返回值判断是哪个按键
u8 key_scan(void)
{
	u8 key_val = 0;
	if (KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0)
	{
		delay_ms(10);//消抖
		if (KEY1 == 0)
			key_val = 1;
		if (KEY2 == 0)
			key_val = 2;
		if (KEY3 == 0)  key_val = 3;
		if (KEY4 == 0)  key_val = 4;

	}

	return key_val;
}
