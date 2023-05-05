//*****************************************************************************
//
// File Name	: 'lcd_lib.c'
// Title		: 8 and 4 bit LCd interface
// Author		: Scienceprog.com - Copyright (C) 2007
// Created		: 2007-03-29
// Revised		: 2007-08-08
// Version		: 1.0
// Target MCU	: Atmel AVR series
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************
#include "lcd_lib.h"
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#ifdef LCD_4BIT_M
static void LCDMix_4Bit(uint8_t data);
#endif
void LCD_vdMoveCursor(uint8_t row, uint8_t column);

const uint8_t LcdCustomChar[] PROGMEM=//define 8 custom LCD chars
{
	0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, // 0. 0/5 full progress block
	0x00, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x00, // 1. 1/5 full progress block
	0x00, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, // 2. 2/5 full progress block
	0x00, 0x1F, 0x1C, 0x1C, 0x1C, 0x1C, 0x1F, 0x00, // 3. 3/5 full progress block
	0x00, 0x1F, 0x1E, 0x1E, 0x1E, 0x1E, 0x1F, 0x00, // 4. 4/5 full progress block
	0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, // 5. 5/5 full progress block
	0x03, 0x07, 0x0F, 0x1F, 0x0F, 0x07, 0x03, 0x00, // 6. rewind arrow
	0x18, 0x1C, 0x1E, 0x1F, 0x1E, 0x1C, 0x18, 0x00  // 7. fast-forward arrow
};

#ifdef LCD_4BIT_M
//sets four port pins to corresponding nibble
//for high nibble use mask data & 0b11110000
//for low nibble use (data & 0b00001111)<<4
static void LCDMix_4Bit(uint8_t data)
{
  if((data)&(0b10000000)) LDPD7 |=1<<LCD_D7;
    else LDPD7 &=~(1<<LCD_D7);
  if((data)&(0b01000000)) LDPD6 |=1<<LCD_D6;
    else LDPD6 &=~(1<<LCD_D6);
  if((data)&(0b00100000)) LDPD5 |=1<<LCD_D5;
    else LDPD5&=~(1<<LCD_D5);
  if((data)&(0b00010000)) LDPD4 |=1<<LCD_D4;
    else LDPD4 &=~(1<<LCD_D4);	
}
#endif

void LCDinit(void)//Initializes LCD
{

#ifdef LCD_4BIT_M
	//4 mixed bit part
	_delay_ms(15);
	//zero to data pins
	LCDMix_4Bit(0b00000000);
	//zero to control pins
	LDPRS &=~(1<<LCD_RS);
	LDPRW &=~(1<<LCD_RW);
	LDPE &=~(1<<LCD_E);
	//set data direction
	LDDD4 |=1<<LCD_D4;
	LDDD5 |=1<<LCD_D5;
	LDDD6 |=1<<LCD_D6;
	LDDD7 |=1<<LCD_D7;
	//control direction pins
	LDDRS |=1<<LCD_RS;
	LDDRW |=1<<LCD_RW;
	LDDE |=1<<LCD_E;
   //---------one------
	LCDMix_4Bit(0b00110000);
	LDPE |=1<<LCD_E;		
	_delay_ms(1);
	LDPE &=~(1<<LCD_E);
	_delay_ms(1);
	//-----------two-----------
	LCDMix_4Bit(0b00110000);
	LDPE |=1<<LCD_E;		
	_delay_ms(1);
	LDPE &=~(1<<LCD_E);
	_delay_ms(1);
	//-------three-------------
	LCDMix_4Bit(0b00100000);
	LDPE |=1<<LCD_E;		
	_delay_ms(1);
	LDPE &=~(1<<LCD_E);
	_delay_ms(1);
	//--------4 bit--dual line---------------
	LCDsendCommand(0b00101000);
   //-----increment address, invisible cursor shift------
	LCDsendCommand(0b00001100);
#endif

		//init 8 custom chars
	uint8_t ch=0, chn=0;
	while(ch<64)
	{
		LCDdefinechar((LcdCustomChar+ch),chn++);
		ch=ch+8;
	}
}	
void LCDsendChar(uint8_t ch)		//Sends Char to LCD
{


#ifdef LCD_4BIT_M
	LCDMix_4Bit(ch & 0b11110000);
	LDPRS |=1<<LCD_RS;
	LDPE |=1<<LCD_E;		
	_delay_ms(1);
	LDPE &=~(1<<LCD_E);
	LDPRS &=~(1<<LCD_RS);
	_delay_ms(1);
	LCDMix_4Bit((ch & 0b00001111)<<4);
	LDPRS |=1<<LCD_RS;
	LDPE |=1<<LCD_E;		
	_delay_ms(1);
	LDPE &=~(1<<LCD_E);
	LDPRS &=~(1<<LCD_RS);
	_delay_ms(1);
#endif

}
void LCDsendCommand(uint8_t cmd)	//Sends Command to LCD
{

#ifdef LCD_4BIT_M
	LCDMix_4Bit(cmd & 0b11110000);
	LDPE |=1<<LCD_E;		
	_delay_ms(1);
	LDPE &=~(1<<LCD_E);
	_delay_ms(1);
	LCDMix_4Bit((cmd & 0b00001111)<<4);
	LDPE |=1<<LCD_E;		
	_delay_ms(1);
	LDPE &=~(1<<LCD_E);
	_delay_ms(1);
#endif

}		
void LCDclr(void)				//Clears LCD
{
	LCDsendCommand(1<<LCD_CLR);
}

void LCDstring(uint8_t* data)	//Outputs string to LCD
{
register uint8_t i;

	// check to make sure we have a good pointer
	if (!data) return;

	// print data
	for(i=0; data[i] != '\0' ; i++)
	{
		LCDsendChar(data[i]);
	}
}
void LCDGotoXY(uint8_t x, uint8_t y)	//Cursor to X Y position
{
	register uint8_t DDRAMAddr;
	// remap lines into proper order
	switch(y)
	{
	case 0: DDRAMAddr = LCD_LINE0_DDRAMADDR+x; break;
	case 1: DDRAMAddr = LCD_LINE1_DDRAMADDR+x; break;
	case 2: DDRAMAddr = LCD_LINE2_DDRAMADDR+x; break;
	case 3: DDRAMAddr = LCD_LINE3_DDRAMADDR+x; break;
	default: DDRAMAddr = LCD_LINE0_DDRAMADDR+x;
	}
	// set data address
	LCDsendCommand(1<<LCD_DDRAM | DDRAMAddr);
	
}
//Copies string from flash memory to LCD at x y position
//const uint8_t welcomeln1[] PROGMEM="AVR LCD DEMO\0";
//CopyStringtoLCD(welcomeln1, 3, 1);	
void CopyStringtoLCD(const uint8_t *FlashLoc, uint8_t x, uint8_t y)
{
	uint8_t i;
	LCDGotoXY(x,y);
	for(i=0;(uint8_t)pgm_read_byte(&FlashLoc[i]);i++)
	{
		LCDsendChar((uint8_t)pgm_read_byte(&FlashLoc[i]));
	}
}

void LCDdefinechar(const uint8_t *pc,uint8_t char_code){
	uint8_t a, pcc;
	uint16_t i;
	a=(char_code<<3)|0x40;
	for (i=0; i<8; i++){
		pcc=pgm_read_byte(&pc[i]);
		LCDsendCommand(a++);
		LCDsendChar(pcc);
		}
}



//adapted fro mAVRLIB
