#include "usart.h"

//串口printf打印端口的选择
#define USARTX USART1

/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
	//USART_ClearFlag(pUSARTx,USART_FLAG_TXE);
}

/*****************  发送指定长度的字节 **********************/
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(pUSARTx,*p);
		
		p++;
		
		//等待数据发送成功
		while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);
		USART_ClearFlag(pUSARTx, USART_FLAG_TXE);
	}
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
	uint8_t i;

	for(i=0; i<num; i++)
	{
		/* 发送一个字节数据到USART */
		Usart_SendByte(pUSARTx,array[i]);	
	}
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	//USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
	do 
	{
		Usart_SendByte( pUSARTx, *(str + k) );
		k++;
	} while(*(str + k)!='\0');

	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;

	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

//重定向c库函数printf到串口，重定向后可使用printf函数
//int fputc(int ch, FILE *f)
//{
//	/* 发送一个字节数据到串口 */
//	USART_SendData(USARTX, (uint8_t) ch);

//	/* 等待发送完毕 */
//	while (USART_GetFlagStatus(USARTX, USART_FLAG_TXE) == RESET);		

//	return (ch);
//}

////重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
//int fgetc(FILE *f)
//{
//	/* 等待串口输入数据 */
//	while (USART_GetFlagStatus(USARTX, USART_FLAG_RXNE) == RESET);

//	return (int)USART_ReceiveData(USARTX);
//}

//蓝牙模块的波特率：9600  名称：BTO5

//PA9 -- 发送端 PA10 -- 接收端 串口1初始化
void USART1_Config(uint32_t USART_BaudRate)
{
	//1、申请GPIO、USART、NVIC结构体参数
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* GPIOA Periph clock enable */
	//2、使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* Configure USART Tx and Rx as alternate function push-pull */
	//3、GPIO参数配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //推挽复用
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//USART configuration
	//4、USART参数配置
	USART_InitStructure.USART_BaudRate = USART_BaudRate; //波特率 115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //使用接收/发送功能
  USART_Init(USART1, &USART_InitStructure);
    
	/* Enable the USART Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  //中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USART Receive Interrupt */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //接收到数据时产生中断

	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
}

//PA2 -- 发送端 PA3 -- 接收端 串口2初始化
void USART2_Config(uint32_t USART_BaudRate)
{
	//1、申请GPIO、USART、NVIC结构体参数
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* GPIOA Periph clock enable */
	//2、使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* Enable USART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* Configure USART Tx and Rx as alternate function push-pull */
	//3、GPIO参数配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //推挽复用
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//USART configuration
	//4、USART参数配置
	USART_InitStructure.USART_BaudRate = USART_BaudRate; //波特率 115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //使用接收/发送功能
  USART_Init(USART2, &USART_InitStructure);
    
	/* Enable the USART Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;  //中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USART Receive Interrupt */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //接收到数据时产生中断

	/* Enable USART */
	USART_Cmd(USART2, ENABLE);
}


volatile uint8_t	Rx1Data 	= 0; //uint8_t --> unsigned char
u8 Rx1Carry[54];
int count1=0;

//This function handles USART2 global interrupt request.
//每接收到一个字节产生一次中断
void USART1_IRQHandler(void)
{
	//判断串口接收数据标志位是否为1
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		/* Read one byte from the receive data register */
		Rx1Data = USART_ReceiveData(USART1);
		
		Rx1Carry[count1++] = Rx1Data; //Rx1Carry[count++] --> Rx1Carry1[0]
		
		if(count1==54)
			count1 = 0;
		
		//把数据发送回发送方，即回显
	  USART_SendData(USART1, Rx1Data); 
		
	}
	
	//清除标志位
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

volatile uint8_t	Rx2Data 	= 0; //uint8_t --> unsigned char
u8 Rx2Carry[54];
int count2=0;

//This function handles USART2 global interrupt request.
//每接收到一个字节产生一次中断
void USART2_IRQHandler(void)
{
	//判断串口接收数据标志位是否为1
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		/* Read one byte from the receive data register */
		Rx2Data = USART_ReceiveData(USART2);
		
		Rx2Carry[count2++] = Rx2Data; //Rx1Carry[count++] --> Rx1Carry1[0]
		
		if(count2==54)
			count2 = 0;
		
		//把数据发送回发送方，即回显
	  //USART_SendData(USART2, Rx1Data); 
		
	}
	
	//清除标志位
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}

