#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "delay.h"


#define KEYPAD_RCC_GPIO_COL		RCC_APB2Periph_GPIOB
#define KEYPAD_GPIO_COL				GPIOB
#define KEYPAD_PIN_COL0				GPIO_Pin_6
#define KEYPAD_PIN_COL1				GPIO_Pin_7
#define KEYPAD_PIN_COL2				GPIO_Pin_8
#define KEYPAD_PIN_COL3				GPIO_Pin_9
#define KEYPAD_RCC_GPIO_ROW		RCC_APB2Periph_GPIOA
#define KEYPAD_GPIO_ROW				GPIOA
#define KEYPAD_PIN_ROW0				GPIO_Pin_8
#define KEYPAD_PIN_ROW1				GPIO_Pin_11
#define KEYPAD_PIN_ROW2				GPIO_Pin_0
#define KEYPAD_PIN_ROW3	            GPIO_Pin_15

#define KEYPAD_NO_PRESSED	0xFF

GPIO_InitTypeDef _GPIO_InitStructKeypad;

void USARTSend(char *);

void KeypadInit()
{
	DelayInit();
	// GPIO clock for keypad columns and rows
	RCC_APB2PeriphClockCmd(KEYPAD_RCC_GPIO_COL, ENABLE);
	RCC_APB2PeriphClockCmd(KEYPAD_RCC_GPIO_ROW, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	// Configure GPIO as output open drain for keypad columns
	_GPIO_InitStructKeypad.GPIO_Pin = KEYPAD_PIN_COL0 | KEYPAD_PIN_COL1 |
		KEYPAD_PIN_COL2 | KEYPAD_PIN_COL3;
	_GPIO_InitStructKeypad.GPIO_Mode = GPIO_Mode_Out_PP;
	_GPIO_InitStructKeypad.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(KEYPAD_GPIO_COL, &_GPIO_InitStructKeypad);

	// Configure GPIO as input with pull-up resistor for keypad rows
	_GPIO_InitStructKeypad.GPIO_Pin = KEYPAD_PIN_ROW0 | KEYPAD_PIN_ROW1 |
		KEYPAD_PIN_ROW2 | KEYPAD_PIN_ROW3;
	_GPIO_InitStructKeypad.GPIO_Mode = GPIO_Mode_IPD;
	_GPIO_InitStructKeypad.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(KEYPAD_GPIO_ROW, &_GPIO_InitStructKeypad);

	_GPIO_InitStructKeypad.GPIO_Pin = KEYPAD_PIN_ROW3;
		_GPIO_InitStructKeypad.GPIO_Mode = GPIO_Mode_IPD;
		_GPIO_InitStructKeypad.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOC, &_GPIO_InitStructKeypad);
	}

/**
  ******************************************************************************
  * @brief	Get which key is pressed by scanning the columns and read the rows.
  * @param	None
  * @retval	Pressed key char value.
  ******************************************************************************
  */
uint8_t getnum;
char num2call[10]="";
int dialset=0;
uint8_t KeypadGetKey()
{
	 //Scan column 0 (column 0 pin is grounded, other column pins is open drain)
	GPIO_SetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL0);
	GPIO_ResetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL1);
	GPIO_ResetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL2);
	GPIO_ResetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL3);
	DelayUs(1);
	// Read rows
	if (GPIO_ReadInputDataBit(KEYPAD_GPIO_ROW, KEYPAD_PIN_ROW0)==1)
		return '1';
	if (GPIO_ReadInputDataBit(KEYPAD_GPIO_ROW, KEYPAD_PIN_ROW1)==1)
		return '4';
	if (GPIO_ReadInputDataBit(KEYPAD_GPIO_ROW, KEYPAD_PIN_ROW2)==1)
		return '7';
	if (GPIO_ReadInputDataBit(GPIOC, KEYPAD_PIN_ROW3)==1)
		return '*';

	// Scan column 1 (column 1 pin is grounded, other column pins is open drain)
	GPIO_ResetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL0);
	GPIO_SetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL1);
	GPIO_ResetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL2);
	GPIO_ResetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL3);
	DelayUs(1);
	// Read rows
	if (GPIO_ReadInputDataBit(KEYPAD_GPIO_ROW, KEYPAD_PIN_ROW0)==1)
			return '2';
	if (GPIO_ReadInputDataBit(KEYPAD_GPIO_ROW, KEYPAD_PIN_ROW1)==1)
			return '5';
	if (GPIO_ReadInputDataBit(KEYPAD_GPIO_ROW, KEYPAD_PIN_ROW2)==1)
			return '8';
	if (GPIO_ReadInputDataBit(GPIOC, KEYPAD_PIN_ROW3)==1)
			return '0';

	// Scan column 2 (column 2 pin is grounded, other column pins is open drain)
	GPIO_ResetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL0);
	GPIO_ResetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL1);
	GPIO_SetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL2);
	GPIO_ResetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL3);
	DelayUs(1);
	// Read rows
	if (GPIO_ReadInputDataBit(KEYPAD_GPIO_ROW, KEYPAD_PIN_ROW0)==1)
			return '3';
	if (GPIO_ReadInputDataBit(KEYPAD_GPIO_ROW, KEYPAD_PIN_ROW1)==1)
			return '6';
	if (GPIO_ReadInputDataBit(KEYPAD_GPIO_ROW, KEYPAD_PIN_ROW2)==1)
			return '9';
	if (GPIO_ReadInputDataBit(GPIOC, KEYPAD_PIN_ROW3)==1)
			return '#';


	// Scan column 3 (column 3 pin is grounded, other column pins is open drain)
	GPIO_ResetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL0);
	GPIO_ResetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL1);
	GPIO_ResetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL2);
	GPIO_SetBits(KEYPAD_GPIO_COL, KEYPAD_PIN_COL3);
	DelayUs(1);
	// Read rows
	if (GPIO_ReadInputDataBit(KEYPAD_GPIO_ROW, KEYPAD_PIN_ROW0)==1)
			return 'A';
	if (GPIO_ReadInputDataBit(KEYPAD_GPIO_ROW, KEYPAD_PIN_ROW1)==1)
			return 'B';
	if (GPIO_ReadInputDataBit(KEYPAD_GPIO_ROW, KEYPAD_PIN_ROW2)==1)
			return 'C';
	if (GPIO_ReadInputDataBit(GPIOC, KEYPAD_PIN_ROW3)==1)
			return 'D';

	return KEYPAD_NO_PRESSED;
}
void dial2num()
{
	if(dialset==0)
	{
	unsigned int c=0;
	lcd16x2_gotoxy(0,1);
	lcd16x2_puts("DIAL:");
	while(c<10)
	{
		getnum=KeypadGetKey();
		if(getnum!=KEYPAD_NO_PRESSED)
		{
			if(getnum=='A')
			{
				num2call[--c]="";
				if(c>0)
				{
				lcd16x2_cursor_shift_left();
				lcd16x2_putc(".");
				lcd16x2_cursor_shift_left();
				}
				DelayMs(500);
			}
			else
			{
			num2call[c++]=getnum;
			lcd16x2_putc(getnum);
			DelayMs(500);
			}
		}
	}
	USARTSend(num2call);
	USARTSend("\r\n");
	dialset=1;
	}
}
/********************************* END OF FILE ********************************/
/******************************************************************************/

//#include "stm32f10x.h"
//#include "stm32f10x_rcc.h"
//#include "stm32f10x_gpio.h"
//#include "delay.h"
//#include "lcd16x2.h"
//
//#define KEYPAD_NO_PRESSED	0xFF
//void KeypadInit();
//uint8_t KeypadGetKey();
//// Custom char data (battery symbol)
//uint8_t custom_char[] = { 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x1F, 0x00, 0x00 };
//uint8_t range5[] = { 0x00,0x01, 0x03, 0x07, 0x0F, 0x1F, 0x1F,  0x00 };
//uint8_t battery_low[] = { 0x0E, 0x1B, 0x11, 0x11, 0x1f, 0x1f, 0x1F, 0x1F };
//uint8_t key;
//GPIO_InitTypeDef _GPIO_InitStructKeypad;
//
//
//int main(void)
//{
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
////	_GPIO_InitStructKeypad.GPIO_Pin = GPIO_Pin_12;
////	_GPIO_InitStructKeypad.GPIO_Mode = GPIO_Mode_IN_FLOATING;
////	_GPIO_InitStructKeypad.GPIO_Speed = GPIO_Speed_2MHz;
////	GPIO_Init(GPIOA, &_GPIO_InitStructKeypad);
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
////	_GPIO_InitStructKeypad.GPIO_Pin = GPIO_Pin_13;
////	_GPIO_InitStructKeypad.GPIO_Mode = GPIO_Mode_Out_PP;
////	_GPIO_InitStructKeypad.GPIO_Speed = GPIO_Speed_2MHz;
////	GPIO_Init(GPIOC, &_GPIO_InitStructKeypad);
//   // Delay initialization
//    DelayInit();
//   // LCD initialization
//   lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_ON_BLINK_ON);
//   lcd16x2_clrscr();
//    KeypadInit();
//    // Create custom char
//    lcd16x2_create_custom_char(0, battery_low);
//    lcd16x2_create_custom_char(1, range5);
//    lcd16x2_put_custom_char(0, 0, 1);
//    lcd16x2_puts("     BSNL  ");
//    lcd16x2_put_custom_char(15, 0, 0);
//    while (1)
//    {
////    	GPIO_SetBits(GPIOC, GPIO_Pin_13);
////    	for(int i=0;i<5000000;i++);
////    	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
////    	for(int i=0;i<5000000;i++);
//    	lcd16x2_gotoxy(0,1);
//    	lcd16x2_puts(">>>");
//    	lcd16x2_puts("+919447968952");
//        lcd16x2_puts(" Low");
//        lcd16x2_clrscr();
//    	DelayMs(500);
//        key = KeypadGetKey();
//        	        // Display pressed char to LCD
//        	        if (key != KEYPAD_NO_PRESSED)
//        	        {
//        	            //lcd16x2_gotoxy(0, 1);
//        	            lcd16x2_putc(key);
//        	            DelayMs(250);
//        	        }
//        lcd16x2_gotoxy(0,1);
//        lcd16x2_puts("                ");
//        lcd16x2_clrscr();
//
//
//    }
//}
