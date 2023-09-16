#include "exti.h"


//记录是否进入功能菜单  0 表示未进入 1 表示已进入
__IO  uint8_t Enter_FunMenu_Flag = 0;

//记录用户选择选项  0 表示没有选择 1 表示手动控制 2 表示蓝牙控制 3 表示角度显示 4 表示返回主页
__IO  uint8_t Control_Number = 0;


//记录按键KEY1的按下次数  默认没按下
__IO  uint8_t KEY1_CntNum = 0;


//PB12 ~ PB15
void Exti_Init(void)
{
  //1、申请GPIO、EXTI、NVIC结构体
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  //2、使能GPIO、AFIO（复用）时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  //3、配置GPIO结构体参数
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //输入模式
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure); //使能GBIO外设

  //4、映射GPIO到中断线
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);

  //配置EXTI结构体参数
  EXTI_InitStructure.EXTI_Line = EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15; //中断线
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;  //使能
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  //中断响应
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //触发方式--下降沿触发
  EXTI_Init(&EXTI_InitStructure);  //使能EXTI外设功能

  //配置NVIC参数
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;  //中断向量
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //抢占
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //响应
  NVIC_Init(&NVIC_InitStructure);  //使能NVIC

}

u32 exti_flag = 0;  //u32 --> unsigned int

//中断服务函数，触发中断条件时自动执行
void EXTI15_10_IRQHandler(void)
{
  //判断中断标志位是否置1
  if (EXTI_GetITStatus(EXTI_Line12) == SET) //SET = 1  REST = 0
  {
    delay_ms(5);
    if (KEY1 == 0)
    {
		

        Control_Number = 1;


    }
    //清除中断标志位
    EXTI_ClearITPendingBit(EXTI_Line12);
  }

  //判断中断标志位是否置1
  if (EXTI_GetITStatus(EXTI_Line13) == SET) //SET = 1  REST = 0
  {
    delay_ms(5);
    if (KEY2 == 0)
    {

        Control_Number = 2;


    }

    //清除中断标志位
    EXTI_ClearITPendingBit(EXTI_Line13);
  }

  //判断中断标志位是否置1
  if (EXTI_GetITStatus(EXTI_Line14) == SET) //SET = 1  REST = 0
  {
    delay_ms(5);
    if (KEY3 == 0)
    {

        Control_Number = 3;
    }

    //清除中断标志位
    EXTI_ClearITPendingBit(EXTI_Line14);
  }

  //判断中断标志位是否置1
  if (EXTI_GetITStatus(EXTI_Line15) == SET) //SET = 1  REST = 0
  {
    delay_ms(5);
    if (KEY4 == 0)
    {

      Control_Number = 4;

        


    }

    //清除中断标志位
    EXTI_ClearITPendingBit(EXTI_Line15);
  }

}
