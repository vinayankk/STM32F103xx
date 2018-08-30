#include<stm32f10x_rcc.h>
#include<stm32f10x_gpio.h>
#include<stm32f10x_adc.h>
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC_InitStructure;

void adc_init(void)
{
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN; //channel 1 ADC 1
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	RCC_ADCCLKConfig (RCC_PCLK2_Div6); //providing clock =72/6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;   //configuring ADC
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 1,ADC_SampleTime_28Cycles5);
	ADC_Init ( ADC1, &ADC_InitStructure);

	ADC_Cmd (ADC1,ENABLE); //enabling ADC


	ADC_ResetCalibration(ADC1); //calibrating ADC
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	//ADC_Cmd (ADC1,ENABLE);


}
int adc_readval(void)
{
	    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		int j = ADC_GetConversionValue(ADC1);
		return(j);

}
