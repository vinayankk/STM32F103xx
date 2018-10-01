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
extern char num[15],num2call[10];
int incall_flag,dial_flag=0,errflag=0,outcall_flag=0,s=0,ns=0,disponflag=0,dispoutflag=0;
_Bool oncall_flag=0;
extern int ansflag;
extern int hangflag,dialset;
extern uint8_t range5[];
extern uint8_t battery_low[];

void clear_RXBuffer(void);
void clear_RXBuffer2(void);
void USARTSend(char *);
void USART2Send(char *);
void clear_RXBuffer(void);
void clear_RXBuffer2(void);


void GSM_init(void)
{
	lcd16x2_create_custom_char(0, battery_low);
	lcd16x2_create_custom_char(1, range5);
	int err=0;incall_flag=0;
 k: USART2Send("AT\r\n");
    for(int i=0;i<1000000;i++)
    {}
	if(RX_FLAG_END_LINE2==1)
	{
		RX_FLAG_END_LINE2=0;
		USARTSend(RX_BUF2);
		if(RX_BUF2[0]=='A'&&RX_BUF2[1]=='T'&&RX_BUF2[4]=='O'&&RX_BUF2[5]=='K')
		{
     		lcd16x2_clrscr();
        	DelayMs(100);
			USARTSend(" GSM INITIALISED");
			USARTSend("\r\n");
			lcd16x2_put_custom_char(0, 0, 1);
			lcd16x2_puts("    AIRTEL  ");
     		lcd16x2_put_custom_char(15, 0, 0);
			err=0;
		}
		else
		{
			USARTSend(" GSM INITIALISING.....");
			USARTSend("\r\n");
			err=1;
			lcd16x2_puts("Booting.....");
			DelayMs(500);
			lcd16x2_clrscr();
			DelayMs(100);
	    }
	clear_RXBuffer2();
	}
	if(err==1)
		goto k;
}


void gsm_command(void)
{
	if(RX_FLAG_END_LINE==1)
	{
		RX_FLAG_END_LINE=0;
		strcat(RX_BUF,"\r\n");
		USART2Send(RX_BUF);
		clear_RXBuffer();
	}
	if(RX_FLAG_END_LINE2==1)
	{
		RX_FLAG_END_LINE2=0;
		USARTSend(" ");
		strcat(RX_BUF2,"\r\n");
		USARTSend(RX_BUF2);
		clear_RXBuffer2();
    }
}


void rcvcall(void)
{
	num[15]="";
	int numindex=0;
	if(RX_FLAG_END_LINE2==1)
	            {
	            	RX_FLAG_END_LINE2=0;
	            	USARTSend(RX_BUF2);
	            	for(int y=0;y<80;y++)
	            	{
	            		if(RX_BUF2[y]=='R')
	            			s=y;
	            	}
	              	if(RX_BUF2[s]=='R'&&RX_BUF2[s+1]=='I'&&RX_BUF2[s+2]=='N'&&RX_BUF2[s+3]=='G')
	              	{
	              		lcd16x2_gotoxy(0, 1);
	              		lcd16x2_puts("                ");
	              		incall_flag=1;
	              		clear_RXBuffer2();
	              		USART2Send("AT+CLCC\r\n");
	              		for(int c=0;c<1000000;c++);
	              		if(RX_FLAG_END_LINE2==1)
	              			{
	              				RX_FLAG_END_LINE2=0;
	              				for(int i=0;i<80;i++)
	              				{
	              					if(RX_BUF2[i]=='+')
	              					{
	              						ns=i;
	              						for(numindex=0;numindex<13;numindex++)
	              						{
	              							num[numindex]=RX_BUF2[ns++];
	              						}
	              					}
	              					else
	              					{
	              						num[13]="";
									}
	              				}
	              				num[numindex++]='\0';
	              				USARTSend("INCOMING CALL FROM  : ");
	              				USARTSend(num);
	              				USARTSend("\r\n");
	              				lcd16x2_gotoxy(0, 1);
	              				lcd16x2_puts(">>>");
	              				lcd16x2_puts(num);
	              				clear_RXBuffer2();
	              			}
	              	}
	              	else
	              	{
	              		clear_RXBuffer2();
	              		incall_flag=0;
	              		lcd16x2_gotoxy(0, 1);
	              		lcd16x2_puts("                ");
					}
	            }
}

void respondcall(void)
{
	if(incall_flag==1)
	{
		if(ansflag==1)
		{
			USART2Send("ATA\r\n");
			USARTSend("CALL ANSWERED\r\n");
			ansflag=0;
		}
		else if(hangflag==1)
		{
			USART2Send("ATH\r\n");
			USARTSend("CALL DISCONNECTED\r\n");
			hangflag=1;
		}
  		incall_flag=0;
	}
}

void gsmgetresponse(void)
{
	clear_RXBuffer2();
	while(!RX_FLAG_END_LINE2);
	if(RX_FLAG_END_LINE2==1)
	{
		RX_FLAG_END_LINE2=0;
		USARTSend(" ");
		strcat(RX_BUF2,"\r\n");
		USARTSend(RX_BUF2);
	 }
}
void managecall(void)
{
	if(disponflag==1)
	{
		lcd16x2_gotoxy(0, 1);
		lcd16x2_puts("ON:");
		lcd16x2_puts(num);
	}
	if(dispoutflag==1)
	{
		lcd16x2_gotoxy(0, 1);
		lcd16x2_puts("<<<+91");
		lcd16x2_puts(num2call);
	}
}
