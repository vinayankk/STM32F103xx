/*THIS PROGRAM READS ADC VALUES FROM STM 32.
 * Use adc_init(); in main.c to initialise ADC. Make necessary changes
 * Use adc_calib to calibrate
 * Use adc_readval to read value from ADC for one time
*/
#include<stm32f10x_rcc.h>
#include<stm32f10x_gpio.h>
#include<stm32f10x_adc.h>
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC_InitStructure;

void adc1_init(void)
{
	RCC_ADCCLKConfig (RCC_PCLK2_Div6); //providing clock =72/6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;   //configuring ADC
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 2;
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 1,ADC_SampleTime_28Cycles5); //pa0=0 pa7=7
	ADC_Init ( ADC1, &ADC_InitStructure);

	ADC_Cmd (ADC1,ENABLE); //enabling ADC

}


int adc1_calib()
{
	ADC_ResetCalibration(ADC1); //calibrating ADC
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
}


int adc1_readval(int channel) //upto 9 channels
{
		ADC_RegularChannelConfig(ADC1,channel,1,ADC_SampleTime_28Cycles5);
	    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		int val = ADC_GetConversionValue(ADC1);
		return(val);
}
