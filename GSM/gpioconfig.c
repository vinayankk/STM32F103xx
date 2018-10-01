#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "lcd16x2.h"
#include "delay.h"
#include <string.h>

GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_InitTypeDef nvicint2;
EXTI_InitTypeDef EXTI_InitStructure;
EXTI_InitTypeDef int2;

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
extern int answrpin,ansflag;
extern int hangpin,hangflag,dialset;
extern int incall_flag,oncall_flag,dial_flag,errflag,outcall_flag,disponflag,dispoutflag;
extern char num2call[10];
char dialcmd[3]="ATD",dialnum[15]="",dialednum[10]="";

void USART2Send(char *);
void USARTSend(char *);
void clear_RXBuffer(void);
void clear_RXBuffer2(void);
void gsmgetresponse(void);

void interruptconfig(void)
{
NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
EXTI_InitStructure.EXTI_Line=EXTI_Line12;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);
NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //Priority 2,
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //Sub priority 2
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //Enable external interrupt channel
NVIC_Init(&NVIC_InitStructure);

int2.EXTI_Line=EXTI_Line1;
int2.EXTI_Mode = EXTI_Mode_Interrupt;
int2.EXTI_Trigger = EXTI_Trigger_Rising;
int2.EXTI_LineCmd = ENABLE;
EXTI_Init(&int2);
nvicint2.NVIC_IRQChannel = EXTI1_IRQn;
nvicint2.NVIC_IRQChannelPreemptionPriority = 4; //Priority 2,
nvicint2.NVIC_IRQChannelSubPriority = 0; //Sub priority 2
nvicint2.NVIC_IRQChannelCmd = ENABLE; //Enable external interrupt channel
NVIC_Init(&nvicint2);

NVIC_SetPriority(EXTI1_IRQn, NVIC_EncodePriority(4,4,0));
NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(4,2,0));

}


void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)//Judge whether a line break
	{
				if(incall_flag==1)
				{
					USART2Send("ATH\r\n");
					USARTSend("CALL DISCONNECTED\r\n");
					incall_flag=0;
					oncall_flag=0;
					disponflag=0;
				}
				if(oncall_flag==1)
				 {
					USART2Send("ATH\r\n");
					USARTSend("CALL DISCONNECTED\r\n");
					oncall_flag=0;
					dial_flag=0;
					disponflag=0;
					dispoutflag=0;
				}

		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line12)!=RESET)//Judge whether a line break
	{
		//USARTSend("interupted2\r\n")
				if(incall_flag==1)
				{
					USART2Send("ATA\r\n");
					USARTSend("CALL ANSWERED\r\n");
					incall_flag=0;
					oncall_flag=1;
					disponflag=1;
				}
				else if(oncall_flag==0)
				{

					//clear_RXBuffer();
					USARTSend("PLEASE DIAL NUMBER....\r\n");
					dial_flag=1;
					oncall_flag=1;
//				   while(!RX_FLAG_END_LINE);
//				   if(RX_FLAG_END_LINE==1)
//					{
//						strcpy(dialnum,dialcmd);
//						strcat(dialnum,RX_BUF);
//						strcat(dialnum,";\r\n");
//						strcpy(dialednum,RX_BUF);

//						USARTSend("PRESS CALL BUTTON TO CALL\r\n");
//						RX_FLAG_END_LINE=0;
//						clear_RXBuffer();
//					}
				}
				else if(dial_flag==1)
				{
					strcpy(dialnum,dialcmd);
					strcat(dialnum,num2call);
					strcat(dialnum,";\r\n");
					USARTSend("Dialing.........\r\n");
					USARTSend("\r\n");
					USART2Send(dialnum);
					dial_flag=0;
					dialset=0;
					dispoutflag=1;
					clear_RXBuffer2();
				}
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
}

void buttonconfig(void)
{
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_1 ;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
}
//void readinputs(void)
//{
//	ansflag=GPIO_ReadInputDataBit(GPIOB,answrpin);
//	hangflag=GPIO_ReadInputDataBit(GPIOB,hangpin);
//}
