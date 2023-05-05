//*****************************************************************************
//
// File Name	: 'main.c'
// Title		: LCD demo
// Author		: Scienceprog.com - Copyright (C) 2007
// Created		: 2007-03-29
// Revised		: 2011-12-13
// Version		: 1.0
// Target MCU	: Atmel AVR series
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd_lib.h"

//Strings stored in AVR Flash memory
const uint8_t LCDwelcomeln1[] PROGMEM="counter =  " ;
const uint8_t LCDwelcomeln2[] PROGMEM=" Welcome Sir " ;
uint8_t counter = 0 ;
uint8_t start[20];
uint8_t str[]="ok";
uint8_t str1[]="hello";
uint8_t str2[]="test1";
uint8_t str3[]="perfection";


void tostring(uint8_t str[], uint8_t num)
{
	int i, rem, len = 0, n;
	
	n = num;
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	for (i = 0; i < len; i++)
	{
		rem = num % 10;
		num = num / 10;
		str[len - (i + 1)] = rem + '0';
	}
	str[len] = '\0';
}



int main(void)
{
	tostring(start,counter);
	LCDinit();//init LCD bit, dual line, cursor right
	LCDsendCommand(LCD_ON_DISPLAY);
	LCDsendCommand(LCD_ON_CURSOR);
	_delay_ms(20);

	CopyStringtoLCD(LCDwelcomeln2,0,0);
	_delay_ms(500);
	LCDclr();//clears LCD
	_delay_ms(20);
	LCDGotoXY(0,0);
	
	LCDstring(str);
	_delay_ms(500);
	//LCDclr();//clears LCD	LCDstring(str1,6);
	_delay_ms(20);
	LCDGotoXY(6,0);
	
	LCDstring(str1);
	_delay_ms(500);
	//LCDclr();//clears LCD	LCDstring(str1,6);
	_delay_ms(20);
	LCDGotoXY(0,1);
	
	LCDstring(str2);
	_delay_ms(500);
	//LCDclr();//clears LCD	LCDstring(str1,6);
	_delay_ms(20);
	LCDGotoXY(6,1);
	
	LCDstring(str3);
	_delay_ms(500);
	LCDclr();//clears LCD	LCDstring(str1,6);
	_delay_ms(20);
// 

	
	while(1)//loop demos
	{
		
		CopyStringtoLCD(LCDwelcomeln1,0,0 );
		LCDGotoXY(0,1);
		LCDstring( start);
		_delay_ms(500);
		LCDclr();//clears LCD
		_delay_ms(20);
		counter++;
		tostring(start,counter);
	}
	return 0;
}
