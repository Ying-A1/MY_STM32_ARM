#include "pwm.h"


//PB10 - TIM2_CH3  PB11 - TIM2_CH4
void PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//定时器结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//输出比较结构体
	TIM_OCInitTypeDef TimOCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE); //部分重映射2
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	 //推挽复用
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	  

	
	 TIM_DeInit(TIM2);  //重置定时器
	 TIM_InternalClockConfig(TIM2);
	 TIM_TimeBaseStructure.TIM_Prescaler = 72; //预分频系数 1MHz 500us~1ms
	 TIM_TimeBaseStructure.TIM_Period = 1000-1;	//ARR 
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数
	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	 TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	 TIM_ARRPreloadConfig(TIM2,ENABLE); //使能自动重装载
	 TIM_Cmd(TIM2, ENABLE); //使能定时器
	
	 //输出比较参数配置
	 TIM_OCStructInit(&TimOCInitStructure);
	 TimOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //输出模式
	 TimOCInitStructure.TIM_Pulse = 0; //初始化占空比CCR值
	 TimOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //有效电平为低
	 TimOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出比较使能
	 TIM_OC3Init(TIM2, &TimOCInitStructure);
	 
	 TIM_CtrlPWMOutputs(TIM2,ENABLE); //使能PWM
	 
	 TIM_SetCompare3(TIM2,500); //设置占空比
}
