#ifndef __USART_H
#define __USART_H

//包含头文件
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "led.h"

//宏定义
//调试宏
#define D printf("\r\nfile is: %s, function is: %s, line is: %d\r\n", __FILE__, __FUNCTION__, __LINE__);

//变量声明
extern volatile uint8_t		Rx2Data;

//函数声明
void USART1_Config(uint32_t USART_BaudRate);
void USART2_Config(uint32_t USART_BaudRate);


void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len);
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif
