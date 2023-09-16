#include "servo.h"
#include "usart.h"

//检测舵机 指令1
void Servo_ping(u8 Servo_ID)
{
  u8 cmd_data[6] = {0};
	
	cmd_data[0] = 0x12;  //包头1
	cmd_data[1] = 0x4c;  //包头2
	cmd_data[2] = 0x01;  //指令
	cmd_data[3] = 0x01;  //数据长度
	cmd_data[4] = Servo_ID; //数据内容-舵机ID 
	cmd_data[5] = cmd_data[0]+cmd_data[1]+cmd_data[2]+cmd_data[3]+cmd_data[4]; //校验和
	
	//通过单片机串口2发送指令给舵机
  Usart_SendBytes(USART2,cmd_data,6); 
	
}

/*
自下往上：
1号舵机：-120 ~ 120
2号舵机：-95 ~ 60
3号舵机：-160 ~ 75
4号舵机：-20 ~ 20
*/
//设置舵机转角度 指令8
void Servo_SetAngle(u8 Servo_ID,int set_angle)
{
  u8 cmd_data[12] = {0};
	
	cmd_data[0] = 0x12;
	cmd_data[1] = 0x4c;
	cmd_data[2] = 0x08; //指令
	cmd_data[3] = 0x07; //数据长度
	cmd_data[4] = Servo_ID; //舵机ID
	cmd_data[5] = set_angle; //转角度低八位
	cmd_data[6] = set_angle>>8; //转角度高八位
	cmd_data[7] = 0xf4;
	cmd_data[8] = 0x01;
	cmd_data[9] = 0;
	cmd_data[10] = 0;
	cmd_data[11] = cmd_data[0]+cmd_data[1]+cmd_data[2]+cmd_data[3]+cmd_data[4]+\
	               cmd_data[5]+cmd_data[6]+cmd_data[7]+cmd_data[8]+cmd_data[9]+\
	               cmd_data[10];
	
	
  Usart_SendBytes(USART2,cmd_data,12);


}
