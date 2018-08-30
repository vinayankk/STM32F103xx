#include<string.h>
#include "stm32f10x_usart.h"
void Serialprint(char s[])
{
	unsigned char buf[strlen(s)+1];
	strcpy(buf,"");
	strcat(buf,s);
	UARTSend(buf, sizeof(buf));
}
void Serialprintln(char s[])
{
	unsigned char buf[strlen(s)+4];
	strcpy(buf,"");
	strcat(buf,s);
	strcat(buf,"\r\n");
	UARTSend(buf, sizeof(buf));
}
void Serialprintint(int num)//use Serialprintln(""); before calling this
{
	if(num>10)
	{
		int temp=0;
		for (num;num>0;num/=10)
		{
		   temp=(num%10)+(temp*10);
		}
		int rem=temp%10;
		while(temp>0)
		{
			char c=rem+'0';
			USART_SendData(USART1,c);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			{
			}
			temp/=10;
			rem=temp%10;
		}
	}
	else
	{
		char c=num+'0';
		USART_SendData(USART1,c);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		{
		}
	}
}
