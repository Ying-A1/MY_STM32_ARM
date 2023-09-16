#include "adjr.h"

////PA4 -- ADC12_IN4 :可以使用ADC1、ADC2 通道4
//void adjr_ADC_GPIO_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN ; //模拟输入
//	GPIO_Init(GPIOA,&GPIO_InitStructure);//
//}

//void adjr_ADC1_Init(void)
//{	
//	//1、申请ADC结构体
//	ADC_InitTypeDef ADC_InitStructure; 
//	
//	//使能ADC时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //要14MHz以下 72MHz/6 = 12MHz
//	
//	ADC_DeInit(ADC1); //复位ADC
//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//单独模式，只有一个ADC时可用
//	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式
//	ADC_InitStructure.ADC_ContinuousConvMode =DISABLE;//单次转换
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //通过软件触发转换
//	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right; //右对齐
//	ADC_InitStructure.ADC_NbrOfChannel=1; //转换通道数
//	ADC_Init(ADC1,&ADC_InitStructure);
//	
//	//采样周期 1/14KHz*239  17.07us
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,1,ADC_SampleTime_239Cycles5);

//	//使能ADC功能
//	ADC_Cmd(ADC1,ENABLE);

//	/*ADC校准*/
//	ADC_ResetCalibration(ADC1); //复位校准
//	while(ADC_GetResetCalibrationStatus(ADC1)); //等待复位校准完成
//	ADC_StartCalibration(ADC1); //开始校准
//	while(ADC_GetCalibrationStatus(ADC1)); //结束校准
//}

////可调电阻初始化
//void ADJR_Init(void)
//{
//  adjr_ADC_GPIO_Init();
//  adjr_ADC1_Init();
//}

////获取ADC转换值（数字量）
//uint16_t Read_ADJRADC(void)
//{
//  ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//软件触发转换
//  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//ADC_FLAG_EOC：转换结束标志位
//  return ADC_GetConversionValue(ADC1);	//获取ADCx->DR寄存器数值
//}

//DMA传输数据的数量 - 5个可调电阻
#define DMA_DATASIZE  5

//保存可调电阻转换后的数字量
__IO uint16_t ADC_ConvertedValue[DMA_DATASIZE] ={0};  


//可调电阻引脚初始化  PA4 ~ PA7 PB0
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;				    				//模拟输入
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	//引脚编号
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;				    				//模拟输入
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;										//引脚编号
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//ADC模数转换器的初始化
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//1.打开DMA1的外设时钟 + ADC1的外设时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	//2.复位DMA1控制器  ADC1-->DMA1的通道1
  DMA_DeInit(DMA1_Channel1);
	
    //3.配置DMA1初始化结构体
  DMA_InitStructure.DMA_PeripheralBaseAddr 	= (u32)(&( ADC1->DR ));  			//把ADC1的数据寄存器作为DMA1的外设基地址 
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (u32)ADC_ConvertedValue;   		//用户定义一个数组作为保存结果的目标地址 	
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralSRC;  			//数据源来自外设	
	DMA_InitStructure.DMA_BufferSize 			= DMA_DATASIZE;     				//缓冲区大小，和定义的数组元素个数应该一致	
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;		//外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable; 			//存储器地址递增，指的是数据缓冲区地址递增
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_HalfWord; 	//外设数据大小为半字，即两个字节
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_HalfWord;    	//内存数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;				//开启循环传输模式
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_High; 				//DMA传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable;  				//禁止存储器到存储器模式，因为是从外设到存储器
	
	//4.初始化DMA1控制器
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); 
	
	//5.使能DMA1通道
	DMA_Cmd(DMA1_Channel1 , ENABLE);

	//6.配置ADC1初始化结构体	
	ADC_InitStructure.ADC_Mode 					= ADC_Mode_Independent; 		//只使用一个ADC，所以设置为独立模式	
	ADC_InitStructure.ADC_ScanConvMode 			= ENABLE ;   					//开启扫描模式，因为进行多通道转换
	ADC_InitStructure.ADC_ContinuousConvMode 	= ENABLE; 						//开启连续转换模式，因为进行多通道转换
	ADC_InitStructure.ADC_ExternalTrigConv 		= ADC_ExternalTrigConv_None;   	//不采用外部触发转换，软件开启即可	
	ADC_InitStructure.ADC_DataAlign 			= ADC_DataAlign_Right;  		//转换结果采用右对齐															
	ADC_InitStructure.ADC_NbrOfChannel 			= 5;	     		 			//转换通道个数 ，因为有5个通道需要转换
		
	//7.初始化ADC
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//8.配置ADC1时钟为PCLK2的8分频，PCLK2= 72MHZ ---> 所以8分频之后为9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	//9.配置ADC1通道的转换顺序和采样时间    
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 4, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_239Cycles5);
	
	//10.开启ADC1的DMA请求
	ADC_DMACmd(ADC1, ENABLE);
	
	//11.开启ADC1模数转换器
	ADC_Cmd(ADC1, ENABLE);
	
	//12.初始化ADC1校准寄存器 + 等待校准寄存器初始化完成 
	ADC_ResetCalibration(ADC1);	
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	//13.ADC开始校准 + 等待校准完成
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
	//14.由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

//机械臂可调电阻的初始化
void ADJR_Init(void)
{
	ADC1_GPIO_Config();  //配置GPIO引脚
	ADC1_Mode_Config();  //配置ADC1+DMA1控制器
}



