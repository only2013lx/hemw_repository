/*****************************************************************************
 *   pwm.h:  Header file for NXP LPC17xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.5.25  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __LCD_TASK_H 
#define __LCD_TASK_H

#include <stdint.h>

typedef union
{
		uint8_t str[35];
		struct
		{
				uint8_t Header[2] ;
				uint8_t firstRow[16];
				uint8_t secondRow[16];
				uint8_t tailer[1] ;
		}Bites;
}LCDDisp;



extern void lcd_Init(void) ;

extern void LCD_Task(void) ;

extern void sysOnOff(void) ;





#endif 
/****************************************************************************
**                            End Of File
****************************************************************************/
