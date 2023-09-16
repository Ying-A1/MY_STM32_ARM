#include "led.h"

//PB10  PB11
void LED_Init(void)
{
	//申请结构体
  GPIO_InitTypeDef  GPIO_Struct;
	
	//使能GPIOB时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);

	//配置结构体参数
	GPIO_Struct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;  //引脚
	GPIO_Struct.GPIO_Speed = GPIO_Speed_10MHz;  //输出速度
	GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	
	//使能GPIO
	GPIO_Init(GPIOB,&GPIO_Struct);
	
	//输出高电平,灯灭
	LED1_OFF;   //GPIO_SetBits(GPIOB,GPIO_Pin_10);  
	LED2_OFF;   //GPIO_SetBits(GPIOB,GPIO_Pin_11);
	
}
