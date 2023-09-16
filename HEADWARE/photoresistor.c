#include "photoresistor.h"

//光照越暗，转换数值越大
//PB1 -- ADC12_IN9 :可以使用ADC1、ADC2 通道9
void ADC_GPIO_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN ; //模拟输入
	GPIO_Init(GPIOB,&GPIO_InitStructure);//
}

void ADC1_Init(void)
{	
	//1、申请ADC结构体
	ADC_InitTypeDef ADC_InitStructure; 
	
	//使能ADC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //要14MHz以下 72MHz/6 = 12MHz
	
	ADC_DeInit(ADC1); //复位ADC
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//单独模式，只有一个ADC时可用
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode =DISABLE;//单次转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //通过软件触发转换
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right; //右对齐
	ADC_InitStructure.ADC_NbrOfChannel=1; //转换通道数
	ADC_Init(ADC1,&ADC_InitStructure);
	
	//采样周期 1/14KHz*239  17.07us
	ADC_RegularChannelConfig(ADC1,ADC_Channel_9,1,ADC_SampleTime_239Cycles5);

	//使能ADC功能
	ADC_Cmd(ADC1,ENABLE);

	/*ADC校准*/
	ADC_ResetCalibration(ADC1); //复位校准
	while(ADC_GetResetCalibrationStatus(ADC1)); //等待复位校准完成
	ADC_StartCalibration(ADC1); //开始校准
	while(ADC_GetCalibrationStatus(ADC1)); //结束校准
}

//光敏电阻初始化
void PR_Init(void)
{
  ADC_GPIO_Init();
  ADC1_Init();
}

//获取ADC转换值（数字量）
uint16_t ReadADC(void)
{
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//软件触发转换
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//ADC_FLAG_EOC：转换结束标志位
  return ADC_GetConversionValue(ADC1);	//获取ADCx->DR寄存器数值
}
