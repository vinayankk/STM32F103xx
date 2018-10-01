/**
  *****************************************************************************
  * @title   usart_rxtx.c
  * @author  sina_morshedi
  * @date    16 Dec 2012
  * @brief   a problem in USARTSend Function, this function is solved.
  *******************************************************************************
  */
////// This function enables USART 1 for communication ///////////////////

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include <ctype.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

#define NUM 10


char name[NUM+1]={'\0'},i;
/* Private function prototypes -----------------------------------------------*/
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);
char rcvdata,rcvbuf;
int count=0;

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles USARTx global interrupt request.
  * @param  None
  * @retval None
  */
//int j=0;


//void USART1_IRQHandler(void)
//{
//
//	char c;
//	/* RXNE handler */
//	    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	    {
//	        /* If received 't', toggle LED and transmit 'T' */
//	    	c=USART_ReceiveData(USART1);
//	        if(c != '\r\n')
//	        {
//	            //
////	            Serialprintln(c);
//	            buf[j++]=c;
//	        }
//	        else
//	        {
//	        	j=0;
//	        	a=1;
//
//	        	//Serialprintln(buf);
//
//	        }
//
//	    }
//	    USART_SendData(USART1, c);
// }



/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : usart_rxtx
* Description    : Print "Welcome to CooCox!"  on Hyperterminal via USART1.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usart_init(void)
{
	    //const unsigned char menu[] = " Welcome to CooCox!\r\n";
		unsigned char init[]=" USART ENABLED\r\n";	    /* Enable USART1 and GPIOA clock */
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);


	    /* NVIC Configuration */
	    NVIC_Configuration();


	    /* Configure the GPIOs */
	    GPIO_Configuration();

	    /* Configure the USART1 */
	    USART_Configuration();

	    /* Enable the USART1 Receive interrupt: this interrupt is generated when the
	         USART1 receive data register is not empty */
	    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	    /* print welcome information */
	    UARTSend(init, sizeof(init));

}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;


  /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configures the USART1.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

/* USART1 configuration ------------------------------------------------------*/
  /* USART1 configured as follow:
        - BaudRate = 115200 baud
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

  /* Enable USART1 */
  USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : UARTSend
* Description    : Send a string to the UART.
* Input          : - pucBuffer: buffers to be printed.
*                : - ulCount  : buffer's length
* Output         : None
* Return         : None
*******************************************************************************/
void UARTSend(const unsigned char *pucBuffer,unsigned long ulCount)
{
    //
    // Loop while there are more characters to send.
    //
    while(ulCount--)
    {
        USART_SendData(USART1, *pucBuffer);
        pucBuffer++;// Last Version USART_SendData(USART1,(uint16_t) *pucBuffer++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        {
        }
    }
}


/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
