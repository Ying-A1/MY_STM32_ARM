#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "stm32f10x.h"
#include "system.h"


/*
PB6 -- OLED_SCK
PB7 -- OLED_SDA
*/

#define OLED_SCK    	  PBout(6)
#define OLED_SDA_OUT    PBout(7)
#define OLED_SDA_IN    	PBin(7)



#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78


void I2C_Configuration(void);  //模拟IIC初始化
void I2C_WriteByte(uint8_t addr,uint8_t data); 
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data); 
void OLED_Init(void);  //OLED屏幕初始化
void OLED_SetPos(unsigned char x, unsigned char y);  //设置起始坐标
void OLED_Fill(unsigned char fill_Data);  //填充屏幕
void OLED_CLS(void);  //清屏
void OLED_ON(void);  //启动OLED
void OLED_OFF(void); //休眠OLED
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize); //显示字符串
void OLED_ShowCN_1(unsigned char x, unsigned char y, unsigned char N); //显示中文1
void OLED_ShowCN_2(unsigned char x, unsigned char y, unsigned char N); //显示中文2
void OLED_ShowCN_3(unsigned char x, unsigned char y, unsigned char N); //显示中文3
void OLED_ShowCN_4(unsigned char x, unsigned char y, unsigned char N); //显示中文4




void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]); //显示图片

#endif
