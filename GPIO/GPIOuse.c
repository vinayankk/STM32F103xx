//// Include These files to enable and use GPIO pins make changes accordingly ////

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

GPIO_InitTypeDef GPIO_B;

#define LED_PORT GPIOB
#define LED_PIN GPIO_Pin_0
void engpio(void)
{
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	 GPIO_B.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_B.GPIO_Mode  = GPIO_Mode_Out_PP;
	 GPIO_B.GPIO_Pin   = LED_PIN ;
	 GPIO_Init(LED_PORT, &GPIO_B);
}
