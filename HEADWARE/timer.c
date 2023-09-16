#include "timer.h"
#include "led.h"

//定时器2中断 -- 定时执行一个功能 ,每一个周期产生一次中断
//PeriodValue:计数值   PrescalerValue：预分频值
void TIM2_Config(uint16_t PeriodValue, uint16_t PrescalerValue)
{
	//1、申请TIM、NVIC结构体参数
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/* Enable TIM clock 36MHz*/
	//2、使能定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//定时公式：T = 1/( 定时器频率 / (PeriodValue-1) / (PrescalerValue-1)/ TIM_CKD_DIV1 )
	/* TIM base configuration */
	//3、配置定时器TIM结构体参数
	TIM_TimeBaseStructure.TIM_Period = PeriodValue; //周期 Max:65535,20000  设置最大计数值（自动重装载寄存器）
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue; //预分频系数 Max:65535,72000000/7200 = 10000Hz  (输出后)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //分频因子为1时 (输出前)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数方式，向上计数
	//使能生效
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	/* Enable the TIM Interrupt Request */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //设置定时器计数器溢出触发中断
	
	/* Enable the TIM Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //TIM2_IRQHandler 中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //抢占
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //响应
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIM enable counter */
	//开启定时器
	TIM_Cmd(TIM2, ENABLE);
}

//定时器3中断 -- 定时执行一个功能 ,每一个周期产生一次中断
//PeriodValue:计数值   PrescalerValue：预分频值
void TIM3_Config(uint16_t PeriodValue, uint16_t PrescalerValue)
{
	//1、申请TIM、NVIC结构体参数
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/* Enable TIM clock 36MHz*/
	//2、使能定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//定时公式：T = 1/( 定时器频率 / (PeriodValue-1) / (PrescalerValue-1)/ TIM_CKD_DIV1 )
	/* TIM base configuration */
	//3、配置定时器TIM结构体参数
	TIM_TimeBaseStructure.TIM_Period = PeriodValue; //周期 Max:65535,20000  设置最大计数值（自动重装载寄存器）
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue; //预分频系数 Max:65535,72000000/7200 = 10000Hz  (输出后)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //分频因子为1时 (输出前)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数方式，向上计数
	//使能生效
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* Enable the TIM Interrupt Request */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //设置定时器计数器溢出触发中断
	
	/* Enable the TIM Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM2_IRQHandler 中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //抢占
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //响应
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIM enable counter */
	//开启定时器
	TIM_Cmd(TIM3, ENABLE);
}

//interrupt
//定时器中断服务函数
void TIM2_IRQHandler(void)
{
	//判断是否计数溢出
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)  //
	{
	
	}
	/* Clear TIM TIM_IT_Update interrupt pending bit */
	//清除标志位
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

u8 tim_flag = 0;
u8 t_count = 0;
//interrupt
//定时器中断服务函数
void TIM3_IRQHandler(void)
{
	//判断是否计数溢出
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)  //
	{
		t_count++;
		if(t_count==5)
		{
			tim_flag++;
			if(tim_flag%2)
			{
				//LED1_ON;
				LED2_ON;
			}
			else{
				//LED1_OFF;
				LED2_OFF;
			}
			t_count = 0;
	  }
	}
	/* Clear TIM TIM_IT_Update interrupt pending bit */
	//清除标志位
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}
