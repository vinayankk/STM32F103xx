#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "lcd16x2.h"
#include "delay.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RX_BUF_SIZE 80
extern char RX_FLAG_END_LINE ;
extern char RXi;
extern char RXc;
extern char RX_BUF[RX_BUF_SIZE] ;
extern char *BUFF;
extern char RX_FLAG_END_LINE2 ;
extern char RXi2;
extern char RXc2;
extern char RX_BUF2[RX_BUF_SIZE];


void clear_RXBuffer(void)
{
    for (RXi=0; RXi<RX_BUF_SIZE; RXi++)
        RX_BUF[RXi] = '\0';
    RXi = 0;
}

void clear_RXBuffer2(void)
{
    for (RXi2=0; RXi2<RX_BUF_SIZE; RXi2++)
        RX_BUF2[RXi2] = '\0';
    RXi2 = 0;
}

void usart_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitTypeDef GPIO_2;
    USART_InitTypeDef USART_InitStructure;
    USART_InitTypeDef USART_2;
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitTypeDef NVIC_init;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

    /* NVIC Configuration */
    /* Enable the USART1 Interrupt */
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(4,0,0));
    /* Enable the USART2 Interrupt */
    NVIC_init.NVIC_IRQChannel = USART2_IRQn;
    NVIC_init.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_init.NVIC_IRQChannelSubPriority = 0;
    NVIC_init.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_init);
    NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(4,0,0));
    /* Configure the GPIOs */
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

   /* Configure USART2 Tx (PA.02) as alternate function push-pull */
    GPIO_2.GPIO_Pin = GPIO_Pin_2;
    GPIO_2.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_2.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_2);

    /* Configure USART2 Rx (PA.3) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_2);

    /* Configure the USART1 */


    /* USART1 configuration ------------------------------------------------------*/
    /* USART1 configured as follow:
        - BaudRate = 9600 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        - USART Clock disabled
        - USART CPOL: Clock is active low
        - USART CPHA: Data is captured on the middle
        - USART LastBit: The clock pulse of the last data bit is not output to
            the SCLK pin
     */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    /* USART2 configuration ------------------------------------------------------*/
    /* USART2 configured as follow:
        - BaudRate = 9600 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        - USART Clock disabled
        - USART CPOL: Clock is active low
        - USART CPHA: Data is captured on the middle
        - USART LastBit: The clock pulse of the last data bit is not output to
            the SCLK pin
     */
    USART_2.USART_BaudRate = 9600;
    USART_2.USART_WordLength = USART_WordLength_8b;
    USART_2.USART_StopBits = USART_StopBits_1;
    USART_2.USART_Parity = USART_Parity_No;
    USART_2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_2);

    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);
     /* Enable USART2 */
    USART_Cmd(USART2, ENABLE);

    /* Enable the USART1 Receive interrupt: this interrupt is generated when the
        USART1 receive data register is not empty */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
          /* Enable the USART2 Receive interrupt: this interrupt is generated when the
        USART1 receive data register is not empty */
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void USARTSend(char *pucBuffer)
{
    while (*pucBuffer)
    {
        USART_SendData(USART1, *pucBuffer++);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        {
        }
    }
}

void USART2Send(char *pucBuffer)
{
    while (*pucBuffer)
    {
        USART_SendData(USART2, *pucBuffer++);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
        {
        }
    }
}

void USART1_IRQHandler(void)
{
    if ((USART1->SR & USART_FLAG_RXNE) != (u16)RESET)
    {
            RXc = USART_ReceiveData(USART1);
            if (RXc != '\r')
            {
            	RX_BUF[RXi] = RXc;
            	RXi++;
                if (RXi > RX_BUF_SIZE-1)
                {
                    clear_RXBuffer();
                }
            }
            else
            {
                RX_FLAG_END_LINE = 1;
            }
    }
}

void USART2_IRQHandler(void)
{
    if ((USART2->SR & USART_FLAG_RXNE) != (u16)RESET)
    {
            RXc2 = USART_ReceiveData(USART2);

            if (RXc2 != '\r')
            {
            	RX_BUF2[RXi2] = RXc2;
            	RXi2++;

                if (RXi2 > RX_BUF_SIZE-1)
                {
                    clear_RXBuffer2();
                }
            }
            else
            {
                RX_FLAG_END_LINE2 = 1;
            }
    }
}
