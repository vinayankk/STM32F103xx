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

#define KEYPAD_NO_PRESSED	0xFF
#define RX_BUF_SIZE 80

char RX_FLAG_END_LINE = 0;
char RXi;
char RXc;
char RX_BUF[RX_BUF_SIZE] = {'\0'};
char *BUFF;
char buffer[80] = {'\0'};
char val[]={'\0'};
char RX_FLAG_END_LINE2 = 0;
char RXi2;
char RXc2;
char RX_BUF2[RX_BUF_SIZE] = {'\0'};
char num[15]="";
int answrpin = GPIO_Pin_3;
int hangpin = GPIO_Pin_1;
int ansflag=0,hangflag=0;
extern int incall_flag,dial_flag;
uint8_t  key;

void usart_init(void);
void clear_RXBuffer(void);
void clear_RXBuffer2(void);
void USARTSend(char *);
void USART2Send(char *);
void clear_RXBuffer(void);
void clear_RXBuffer2(void);
void GSM_init(void);
void gsm_command(void);
void rcvcall(void);
void buttonconfig(void);
void readinputs(void);
void respondcall(void);
void interruptconfig(void);
void managecall(void);
void KeypadInit();
uint8_t KeypadGetKey();
void dial2num();


int main()
{

    usart_init();
    lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_ON_BLINK_ON);
    DelayMs(500);
    lcd16x2_clrscr();
    DelayMs(500);
    GSM_init();
    buttonconfig();
    interruptconfig();
    KeypadInit();
    USARTSend(" SIM ACTIVE\r\n");
    while(1)
    {
    	rcvcall();
        managecall();
    	if(dial_flag==1)
    	{
    		dial2num();
    	}

     }
}



