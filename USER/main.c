/*
1、显示功能：
	OLED显示能够实现的机械臂控制方式
2、控制方式选择：
	按键选择
3、机械臂控制：
	（1）自动放置物品到指定位置
	（2）可调电阻控制机械臂放置物品到指定位置
	（3）无线控制（WIFI、蓝牙）控制机械臂放置物品到指定位置
4、自动复位功能：
	进入系统后，机械臂有一个初始化状态，执行完功能控制后，自动恢复到初始化状态。
*/

#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "timer.h"
#include "pwm.h"
#include "usart.h"
#include "string.h"
#include "OLED_I2C.h"
#include "photoresistor.h"
#include "adjr.h"
#include "stdio.h"
#include "servo.h"
#include "iwdg.h"
#include "wwdg.h"


extern uint8_t Rx1Data;

extern u8 Rx1Carry[54];
extern int count1;
extern unsigned char BMP1[];



//记录用户选择选项  0 表示没有选择 1 表示自动控制 2 表示电阻控制 3 表示蓝牙 4 表示返回主页
extern __IO  uint8_t Control_Number;

int16_t  BlueTooth[5] = { 0,-150,400,0,0 };


void limit_Angle(int Angle1, int Angle2, int Angle3, int Angle4)
{
	if (Angle1 < -1200) { BlueTooth[0] = -1200; }
	else if (Angle1 > 1200) { BlueTooth[0] = 1200; }

	if (Angle2 < -950) { BlueTooth[1] = -950; }
	else if (Angle2 > 600) { BlueTooth[1] = 600; }

	if (Angle3 < -1600) { BlueTooth[2] = -1600; }
	else if (Angle3 > 750) { BlueTooth[2] = 750; }

	if (Angle4 < -200) { BlueTooth[3] = -200; }
	else if (Angle4 > 200) { BlueTooth[3] = 200; }
}

void limit_Angle(int Angle1, int Angle2, int Angle3, int Angle4);

//保存可调电阻转换后的数字量
extern __IO uint16_t ADC_ConvertedValue[5];

int main()
{

	int i = 0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组管理

	//机械臂硬件初始化
	delay_Init(72);  //嘀嗒定时器初始化 八分频 F1系列单片机时钟频率为72MHz
	LED_Init();  //LED初始化
	//BEEP_Init(); //蜂鸣器初始化
	Key_Init();  //按键初始化
	Exti_Init();//按键中断
	//TIM2_Config(20000,3600); //72000000/3600 = 20000 //TIM2测试
	//TIM3_Config(20000, 3600);//TIM3测试
	//PWM_Init();//PWM测试
	USART1_Config(115200);// USART1控制蓝牙 蓝牙模块修改了的波特率为115200bps
	USART2_Config(115200);// USART2控制舵机 串口舵机默认的波特率为115200bps
	//PR_Init();  //光敏电阻ADC初始化
	ADJR_Init(); //可调电阻初始化
	I2C_Configuration(); //模拟IIC使用引脚初始化
	OLED_Init(); //OLED初始化
	OLED_ON(); //开启OLED

	//刷一下OLED
	OLED_Fill(0xff);
	delay_s(1);
	OLED_Fill(0x00);
	delay_s(1);


	//IWDG_Init(2,2500);  //独立看门狗初始化 超时时间（检测喂狗时间）1000ms
	// WWDG_Init(0x7f, 0x7f, WWDG_Prescaler_8); //超时时间 4096*2^3*64/36000000 = 58.25ms
	// WWDG_NVIC_Init(); //WWDG中断分组管理
	//WWDG_Set_Counter(0x7f); //设置计数值及使能看门狗，不用中断时使用，手动喂狗

	//LED匀速闪烁3次表示硬件初始化成功
#if 1
	for (i = 0;i < 2;i++)
	{
		LED1_ON;
		LED2_ON;
		delay_ms(50);
		LED1_OFF;
		LED2_OFF;
		delay_ms(50);
	}
#endif

	//先检测舵机是否在线，机械臂启动之后把各舵机转动到初始位置
	for (i = 1;i < 5;i++)
	{
		Servo_ping(i);
		delay_ms(100);
	}
	Servo_SetAngle(1, 0); //-120 ~ 120
	delay_ms(500);
	Servo_SetAngle(2, -150); // -95 ~ 60
	delay_ms(500);
	Servo_SetAngle(3, 400); //-160 ~ 75
	delay_ms(500);
	Servo_SetAngle(4, 0); //-20 ~ 20
	delay_ms(500);


	//OLED显示系统主页，此时按下KEY1可进入功能菜单
	//OLED_MainMenu();   



	//OLED_FuncMenu();
	//OLED显示系统主页，此时按下KEY1可进入功能菜单
	//封装成OLED_ShowFuncMenu();
	//OLED的总大小是 128*64  128/16= 8 64/16 = 4
	//x:0~127 y:64/8=8 0~7 
	//ying注：有点莫名其妙的函数，可优化
	//x,y -- 起始点坐标(x:0~127, y:0~7); 
	//显示姓名(汉字) 汉字格式：行、列占16个像素点
	for (i = 0;i < 4;i++)
	{
		OLED_ShowCN_1(40 + 16 * i, 0, i);
		OLED_ShowCN_2(40 + 16 * i, 2, i);
		OLED_ShowCN_3(40 + 16 * i, 4, i);
		OLED_ShowCN_4(40 + 16 * i, 6, i);
	}
	OLED_ShowStr(22, 0, "01", 2);  //2:8*16
	OLED_ShowStr(22, 2, "02", 2);  //2:8*16
	OLED_ShowStr(22, 4, "03", 2);  //2:8*16
	OLED_ShowStr(22, 6, "04", 2);  //2:8*16

	//OLED_Fill(0x00);//清屏  //1:6*8

	delay_ms(500);

	while (1)
	{

		// //如果用户在启动机械臂之后按下KEY1，则表示进入功能菜单，执行以下语句
		// if( Enter_FunMenu_Flag == 1 )
		// {
		// 	//显示机械臂功能界面
		// 	//OLED_FuncMenu(); 
		// 	OLED_Fill(0x00);//清屏

		// 	OLED_ShowStr(22, 0, "01", 2);  //2:8*16
		// 	OLED_ShowStr(22, 2, "02", 2);  //2:8*16
		// 	OLED_ShowStr(22, 4, "03", 2);  //2:8*16
		// 	OLED_ShowStr(22, 6, "04", 2);  //2:8*16
		// }
		// else {
		// 	//OLED_MainMenu(); 
		// 	OLED_Fill(0x00);//清屏
		// 	for (i = 0;i < 4;i++)
		// 	{
		// 		OLED_ShowCN_1(40 + 16 * i, 0, i);
		// 		OLED_ShowCN_2(40 + 16 * i, 2, i);
		// 		OLED_ShowCN_3(40 + 16 * i, 4, i);
		// 		OLED_ShowCN_4(40 + 16 * i, 6, i);
		// 	}
		// 	OLED_ShowStr(22, 0, "01", 2);  //2:8*16
		// 	OLED_ShowStr(22, 2, "02", 2);  //2:8*16
		// 	OLED_ShowStr(22, 4, "03", 2);  //2:8*16
		// 	OLED_ShowStr(22, 6, "04", 2);  //2:8*16
		// }
		switch (Control_Number)
		{
		case 1:
			OLED_Fill(0x00);//清屏
			for (i = 0;i < 4;i++)
			{
				OLED_ShowCN_1(40 + 16 * i, 0, i);
			}
			OLED_ShowStr(22, 0, "01", 2);  //2:8*16
			//自动放置物品到指定位置
			Servo_SetAngle(2, -550); //-120 ~ 120
			delay_ms(500);
			Servo_SetAngle(4, -100); // -95 ~ 60
			delay_ms(500);
			Servo_SetAngle(2, -150); //-160 ~ 75
			delay_ms(500);
			Servo_SetAngle(1, 900); //-20 ~ 20
			delay_ms(500);
			Servo_SetAngle(2, -550); //-20 ~ 20
			delay_ms(500);
			Servo_SetAngle(4, 0); //-20 ~ 20
			delay_ms(500);
			Servo_SetAngle(2, -150); //-20 ~ 20
			delay_ms(500);
			Servo_SetAngle(1, 0); //-20 ~ 20
			delay_ms(500);
			Control_Number = 0;
			break;
		case 2:
			//电阻控制
			OLED_Fill(0x00);//清屏
			OLED_ShowStr(22, 2, "02", 2);  //2:8*16
			for (i = 0;i < 4;i++)
			{
				OLED_ShowCN_2(40 + 16 * i, 2, i);
			}
			//可调电阻控制机械臂放置物品到指定位置
			//归一化的一般规范函数:y = (ymax-ymin)*(x-xmin)/(xmax-xmin) + ymin
			while(1){
				Servo_SetAngle(1, (ADC_ConvertedValue[0] * 2400 / 4095) + (-1200)); //-120 ~ 120
				Servo_SetAngle(2, (ADC_ConvertedValue[1] * 1550 / 4095) + (-950)); // -95 ~ 60
				Servo_SetAngle(3, (ADC_ConvertedValue[2] * 2350 / 4095) + (-1600)); //-160 ~ 75
				Servo_SetAngle(4, (ADC_ConvertedValue[3] * 400 / 4095) + (-200)); //-20 ~ 20
				if(Control_Number == 4){
					break;
				}
				
			}
			
			break;
		case 3:
			//蓝牙控制
			//蓝牙控制

			OLED_Fill(0x00);//清屏
			OLED_ShowStr(22, 4, "03", 2);  //2:8*16
			for (i = 0;i < 4;i++)
			{
				OLED_ShowCN_3(40 + 16 * i, 4, i);
			}
			while(1){
			if (Rx1Carry[count1 - 1] == '#')
			{
				if (strncmp((char*)Rx1Carry, "left", count1 - 1) == 0)//
				{
					BlueTooth[0] = BlueTooth[0] - 50;
				}
				else if (strncmp((char*)Rx1Carry, "right", count1 - 1) == 0)
				{
					BlueTooth[0] = BlueTooth[0] + 50;
				}

				else if (strncmp((char*)Rx1Carry, "front", count1 - 1) == 0)
				{
					BlueTooth[1] = BlueTooth[1] - 50;
				}
				else if (strncmp((char*)Rx1Carry, "back", count1 - 1) == 0)
				{
					BlueTooth[1] = BlueTooth[1] + 50;
				}

				else if (strncmp((char*)Rx1Carry, "up", count1 - 1) == 0)
				{
					BlueTooth[2] = BlueTooth[2] - 50;
				}
				else if (strncmp((char*)Rx1Carry, "dowm", count1 - 1) == 0)
				{
					BlueTooth[2] = BlueTooth[2] + 50;
				}

				else if (strncmp((char*)Rx1Carry, "loose", count1 - 1) == 0)
				{
					BlueTooth[3] = BlueTooth[3] + 50;
				}
				else if (strncmp((char*)Rx1Carry, "close", count1 - 1) == 0)
				{
					BlueTooth[3] = BlueTooth[3] - 50;
				}
			}

			memset(Rx1Carry, 0, sizeof(Rx1Carry));
			count1 = 0;


			limit_Angle(BlueTooth[0], BlueTooth[1], BlueTooth[2], BlueTooth[3]);

			Servo_SetAngle(1, BlueTooth[0]);
			Servo_SetAngle(2, BlueTooth[1]);
			Servo_SetAngle(3, BlueTooth[2]);
			Servo_SetAngle(4, BlueTooth[3]);
			if(Control_Number == 4){
					break;
				}
		}

			break;

		case 4:
			OLED_Fill(0x00);//清屏
			for (i = 0;i < 4;i++)
			{
				OLED_ShowCN_1(40 + 16 * i, 0, i);
				OLED_ShowCN_2(40 + 16 * i, 2, i);
				OLED_ShowCN_3(40 + 16 * i, 4, i);
				OLED_ShowCN_4(40 + 16 * i, 6, i);
			}
			OLED_ShowStr(22, 0, "01", 2);  //2:8*16
			OLED_ShowStr(22, 2, "02", 2);  //2:8*16
			OLED_ShowStr(22, 4, "03", 2);  //2:8*16
			OLED_ShowStr(22, 6, "04", 2);  //2:8*16

			Servo_SetAngle(1, 0); //-120 ~ 120
			delay_ms(500);
			Servo_SetAngle(2, -150); // -95 ~ 60
			delay_ms(500);
			Servo_SetAngle(3, 400); //-160 ~ 75
			delay_ms(500);
			Servo_SetAngle(4, 0); //-20 ~ 20
			delay_ms(1000);
			Control_Number = 0;
			break;

		default:
			break;
		}


		//独立看门狗
		//		delay_ms(500); //1个delay在需喂狗时间（检测）内，3个delay超时
		////		delay_ms(500);
		////		delay_ms(500);
		//		LED1_OFF;
		//		IWDG_Feed(); //喂狗
		//		delay_ms(500);
		////		delay_ms(500);
		////		delay_ms(500);
		//		LED1_ON;
		//		IWDG_Feed(); //喂狗

		// //舵机控制
		// Servo_SetAngle(1, -1000); //-120 ~ 120
		// Servo_SetAngle(2, 0); // -95 ~ 60
		// Servo_SetAngle(3, 200); //-160 ~ 75
		// Servo_SetAngle(4, 200); //-20 ~ 20

		//OLED显示可调电阻
		//r_value = ReadADC();
		//r_value = Read_ADJRADC();



		// for (i = 0;i < 5;i++)
		// {
		// 	sprintf((char*)buff, "AR[%d]:value:%d", i, ADC_ConvertedValue[i]);
		// 	OLED_ShowStr(0, 0 + i, buff, 1);  //1:6*8
		// }
		//delay_s(1);
		//OLED_Fill(0x00);//清屏  OLED_ShowStr(0,0+i,"    ",1);  //1:6*8


		//OLED显示	
		//		//全屏显示
		//		OLED_Fill(0xff);
		//		delay_s(1);
		//		//息屏
		//		OLED_Fill(0x00);
		//    delay_s(1);
		//		

		//		
		//		//显示英文
		//		OLED_ShowStr(0,2,"Hello world",1);  //1:6*8
		//		OLED_ShowStr(0,3,"Hello world",2);  //2:8*16
		//		delay_s(2);
		//		
		//		//显示图案
		//		OLED_DrawBMP(0,0,128,8,BMP1);
		//		delay_s(2);

		// //蓝牙
		// if (Rx1Carry[count1 - 1] == '#')
		// {
		// 	if (strncmp((char*)Rx1Carry, "open", count1 - 1) == 0)
		// 	{
		// 		BEEP_ON;
		// 	}

		// 	if (strncmp((char*)Rx1Carry, "close", count1 - 1) == 0)
		// 	{
		// 		BEEP_OFF;
		// 	}

		// 	memset(Rx1Carry, 0, sizeof(Rx1Carry));
		// 	count1 = 0;
		// }

		//PWM
		//    TIM_SetCompare3(TIM2,0); //p = 0	
		//		TIM_SetCompare3(TIM2,499); //p = 500/1000
		//		TIM_SetCompare3(TIM2,999); //p = 1

	}

}
