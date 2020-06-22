/*****************************************************************************
 *   pwm.c:  PWM module file for NXP LPC17xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.26  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/

#include "lcdTask.h"
#include "ram.h"
#include "timerDriver.h"
#include "ioControl.h"
#include "warn.h"
#include "uartDriver.h"
#include "canDriver.h"
#include "canHard.h"
#include "canSendQueue.h"
#include "canTask.h"

static uTIMER buttontimer ;
static uint8_t button_function = 0 ;
static LCDDisp lcdStr ; 
//	static uint8_t sendChar[35] = 
//		{
//			0xAA, 0x25,  //帧头
//			0x46, 0x43, 0x5F, 0x56, 0x20,0x20, 0x20, 0x20, 0x20, 
//			0x3A, 0x31, 0x34,0x2E, 0x36, 0x35, 0x56,   //第一行
//			0x46, 0x43, 0x5F,0x50, 0x20, 0x20, 0x20, 0x20, 0x20, 
//			0x3A, 0x31, 0x35, 0x36, 0x2E, 0x37, 0x77,  //第二行
//			0x0D         //帧尾
//		} ;

void lcd_timer(void)
{
	 if(1 == buttontimer.BitsM.Tim1_enM)
	 {
			buttontimer.BitsM.Tim1_timM ++ ;
	 }
}

void lcd_Init(void)
{
		timer_set(LCD_TIMER_NO, 1000, TIMER_TICK_RESOLUTION_100MS, TIMER_STOP, TIMER_MODE_INT_LOOP, lcd_timer);
		timer_start(LCD_TIMER_NO);
		lcdStr.Bites.Header[0] = 0xAA ;
		lcdStr.Bites.Header[1] = 0x25 ;
		lcdStr.Bites.tailer[0] = 0x0D ;
}

//功能按键
void button_control(void)
{
		if(1 == getEINTState())
		{
				//按键
				if(0 == IO_FUNC_STATE())
				{
						//定时开始
						buttontimer.BitsM.Tim1_enM = 1 ;
						//超时
						if(buttontimer.BitsM.Tim1_timM >= 3)
						{
								button_function = 1 ; //开机按键
								buttontimer.Word = 0 ;
								setEINTState(0);				
						}
				}
				else
				{
						setEINTState(0);
						button_function = 2 ; //翻页按键
						buttontimer.Word = 0 ;
				}
		}
}

void LCD_Write_1(uint8_t *str)
{
		uint8_t i = 0 ;
		for(i = 0 ; i < 16 ;i++)
		{
				lcdStr.Bites.firstRow[i] = str[i] ;
		}
}

void LCD_Write_2(uint8_t *str)
{
		uint8_t i = 0 ;
		for(i = 0 ; i < 16 ;i++)
		{
				lcdStr.Bites.secondRow[i] = str[i] ;
		}
}

void float_To_str(float num , uint8_t *str)
{
//		uint8_t i = 0 ;
		uint32_t tmp = num * 10 ;
		if(tmp > 10000)
		{
			str[0] = tmp / 10000 + '0' ;
			str[1] = tmp % 10000 / 1000 + '0';
			str[2] = tmp % 1000 / 100 + '0'  ;
			str[3] = tmp % 100 / 10 + '0'    ;
			str[4] = '.' ;  // .
			str[5] = tmp % 10  + '0' ; 
		}
		else
		{
			if(num >= 0)
			{
				str[0] = '+' ;
			}
			else
			{
				str[0] = '-' ;
			}
			str[1] = tmp % 10000 / 1000 + '0';
			str[2] = tmp % 1000 / 100 + '0'  ;
			str[3] = tmp % 100 / 10 + '0'    ;
			str[4] = '.' ;  // .
			str[5] = tmp % 10  + '0' ; 
		}
		if(tmp < 1000)
		{
			str[1] = ' ';
		}		
		if(tmp < 100)
		{
			str[2] = ' ';
		}		
//		str[7] = '\0' ;
}

/*******************************
//每行分区，共16字
//Name : 8
//:    : 1
//NUm  : 6
//Unit : 1
********************************/
void LCD_Write(uint8_t Row ,uint8_t *strName, float num , uint8_t strUint)
{
		uint8_t i = 0 ;
		uint8_t str[6];
		float_To_str(num,str);
		if(1 == Row)
		{
				//NAME
				for(i = 0 ; i < 8 ; i++)
				{
						lcdStr.Bites.firstRow[i] = strName[i] ;
				}
				lcdStr.Bites.firstRow[8] = 0x3A ;
				//NUM
				for(i = 0 ; i < 6 ; i++)
				{
						lcdStr.Bites.firstRow[i+9] = str[i] ;
				}
				lcdStr.Bites.firstRow[15] = strUint ;				
		}
		else
		{
				//NAME
				for(i = 0 ; i < 8 ; i++)
				{
						lcdStr.Bites.secondRow[i] = strName[i] ;
				}
				lcdStr.Bites.secondRow[8] = 0x3A ;
				//NUM
				for(i = 0 ; i < 6 ; i++)
				{
						lcdStr.Bites.secondRow[i+9] = str[i] ;
				}
				lcdStr.Bites.secondRow[15] = strUint ;					
		}
}

void LCD_Write_Warn(uint8_t *strName , uint8_t MainWarn , uint8_t FCWarn , uint8_t BMSWarn)
{
		uint8_t i = 0 ;
		
		if(MainWarn >= 100)
		{
				MainWarn = 0 ;
		}

		if(FCWarn >= 100)
		{
				FCWarn = 0 ;
		}
		
		if(BMSWarn >= 10)
		{
				BMSWarn = 0 ;
		}
		
		//NAME
		for(i = 0 ; i < 8 ; i++)
		{
				lcdStr.Bites.secondRow[i] = strName[i] ;
		}
		lcdStr.Bites.secondRow[8] = 0x3A ;
		//NUM
		lcdStr.Bites.secondRow[9]  = (MainWarn / 10)+'0' ;
		lcdStr.Bites.secondRow[10] = (MainWarn % 10)+'0' ;
		lcdStr.Bites.secondRow[11] = 0x3b ;
		lcdStr.Bites.secondRow[12] = (FCWarn / 10)+'0' ;
		lcdStr.Bites.secondRow[13] = (FCWarn % 10)+'0' ;
		lcdStr.Bites.secondRow[14] = 0x3b ;
		lcdStr.Bites.secondRow[15] = (BMSWarn % 10)+'0' ;
}

//主控告警 转换
uint8_t MainWarnTransform(void)
{
		uint8_t tmp = 0 ;
		if(1 == g_MCMS_system.msysWarn.Bites.w1)
		{
				tmp = 1 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w2)
		{
				tmp = 2 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w3)
		{
				tmp = 3 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w4)
		{
				tmp = 4 ;
		}		
		else if(1 == g_MCMS_system.msysWarn.Bites.w5)
		{
				tmp = 5 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w6)
		{
				tmp = 6 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w7)
		{
				tmp = 7 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w8)
		{
				tmp = 8 ;
		}		
		else if(1 == g_MCMS_system.msysWarn.Bites.w9)
		{
				tmp = 9 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w10)
		{
				tmp = 10 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w11)
		{
				tmp = 11 ;
		}		
		else if(1 == g_MCMS_system.msysWarn.Bites.w12)
		{
				tmp = 12 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w13)
		{
				tmp = 13 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w14)
		{
				tmp = 14 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w15)
		{
				tmp = 15 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w16)
		{
				tmp = 16 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w17)
		{
				tmp = 17 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w18)
		{
				tmp = 18 ;
		}		
		else if(1 == g_MCMS_system.msysWarn.Bites.w19)
		{
				tmp = 19 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w20)
		{
				tmp = 20 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w21)
		{
				tmp = 21 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w22)
		{
				tmp = 22 ;
		}		
		else if(1 == g_MCMS_system.msysWarn.Bites.w23)
		{
				tmp = 23 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w24)
		{
				tmp = 24 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w25)
		{
				tmp = 25 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w26)
		{
				tmp = 26 ;
		}		
		else if(1 == g_MCMS_system.msysWarn.Bites.w27)
		{
				tmp = 27 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w28)
		{
				tmp = 28 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w29)
		{
				tmp = 29 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w30)
		{
				tmp = 30 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w31)
		{
				tmp = 31 ;
		}
		else if(1 == g_MCMS_system.msysWarn.Bites.w32)
		{
				tmp = 32 ;
		}		
		
		return tmp ;
}

//电堆告警 转换
uint8_t FCWarnTransform(void)
{
		uint8_t tmp = 0 ;
		if(1 == g_FCMS_system[0].msysWarn.Bites.w1)
		{
				tmp = 1 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w2)
		{
				tmp = 2 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w3)
		{
				tmp = 3 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w4)
		{
				tmp = 4 ;
		}		
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w5)
		{
				tmp = 5 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w6)
		{
				tmp = 6 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w7)
		{
				tmp = 7 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w8)
		{
				tmp = 8 ;
		}		
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w9)
		{
				tmp = 9 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w10)
		{
				tmp = 10 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w11)
		{
				tmp = 11 ;
		}		
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w12)
		{
				tmp = 12 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w13)
		{
				tmp = 13 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w14)
		{
				tmp = 14 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w15)
		{
				tmp = 15 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w16)
		{
				tmp = 16 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w17)
		{
				tmp = 17 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w18)
		{
				tmp = 18 ;
		}		
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w19)
		{
				tmp = 19 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w20)
		{
				tmp = 20 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w21)
		{
				tmp = 21 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w22)
		{
				tmp = 22 ;
		}		
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w23)
		{
				tmp = 23 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w24)
		{
				tmp = 24 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w25)
		{
				tmp = 25 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w26)
		{
				tmp = 26 ;
		}		
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w27)
		{
				tmp = 27 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w28)
		{
				tmp = 28 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w29)
		{
				tmp = 29 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w30)
		{
				tmp = 30 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w31)
		{
				tmp = 31 ;
		}
		else if(1 == g_FCMS_system[0].msysWarn.Bites.w32)
		{
				tmp = 32 ;
		}		
		
		return tmp ;
}

//BMS 告警转换
uint8_t BMSWarnTransform(void)
{
		uint8_t tmp = 0 ;
		if(76 == g_FCMS_BMSpara.mBMS_Warn)
		{
				tmp = 1 ;
		}
		else if(200 == g_FCMS_BMSpara.mBMS_Warn)
		{
				tmp = 2 ;
		}
		else if(203 == g_FCMS_BMSpara.mBMS_Warn)
		{
				tmp = 3 ;
		}
		else if(10 == g_FCMS_BMSpara.mBMS_Warn)
		{
				tmp = 4 ;
		}
		else if(9 == g_FCMS_BMSpara.mBMS_Warn)
		{
				tmp = 5 ;
		}
		else if(133 == g_FCMS_BMSpara.mBMS_Warn)
		{
				tmp = 6 ;
		}
		else if(131 == g_FCMS_BMSpara.mBMS_Warn)
		{
				tmp = 7;
		}
		else if(45 == g_FCMS_BMSpara.mBMS_Warn)
		{
				tmp = 8 ;
		}
		else if(135 == g_FCMS_BMSpara.mBMS_Warn)
		{
				tmp = 9 ;
		}


		return tmp ;
}


//44 43 5F 50 20 20 20 20 3A 20 31 35 36 2E 37 77

/******
*1：DC电压   、 DC功率
*2：电堆电压 、 电堆功率
*3：电堆温度、环境温度
*4：告警编号
*******/
void LCD_page(uint8_t *button_function)
{
		static uint8_t page = 0 ;
		if(2 == *button_function)
		{
				page++ ;
				*button_function = 0 ;
		}
#if 0
		switch(page)
		{
			case 0:
//				LCD_Write(1,"SysPower",g_DCMS_runpara[0].mD_V * (g_DCMS_runpara[0].mD_A +g_DCMS_runpara[1].mD_A) + g_FCMS_BMSpara.mBMS_V * g_FCMS_BMSpara.mBMS_A,'w');
//				LCD_Write(2,"Sys_Warn",0,' ');
				LCD_Write(1,"SysVol  ",g_DCMS_runpara[0].mD_V,'v');
				LCD_Write(2,"SysPower",g_DCMS_runpara[0].mD_V * (g_DCMS_runpara[0].mD_A * 3),'w');
				break ;
			case 1:
				LCD_Write(1,"FCVol1  ",g_FCMS_runpara[0].mF_V.flaotword ,'V');
				LCD_Write(2,"FCPower1",g_FCMS_runpara[0].mF_V.flaotword * g_FCMS_runpara[0].mF_A.flaotword,'w');				
//				LCD_Write(1,"BMSPower",g_FCMS_BMSpara.mBMS_V * g_FCMS_BMSpara.mBMS_A,'w');
//				LCD_Write(2,"BMS_SOC ",g_FCMS_BMSpara.mBMS_SOC,'%');					
				break ;
//			case 2:
//				LCD_Write(1,"FCVol2  ",g_FCMS_runpara[1].mF_V.flaotword ,'V');
//				LCD_Write(2,"FCPower2",g_FCMS_runpara[1].mF_V.flaotword * g_FCMS_runpara[1].mF_A.flaotword,'w');			
////				LCD_Write(1,"BMSPower",g_FCMS_BMSpara.mBMS_V * g_FCMS_BMSpara.mBMS_A,'w');
////				LCD_Write(2,"BMS_SOC ",g_FCMS_BMSpara.mBMS_SOC,'%');					
//				break ;
			case 3:
				LCD_Write(1,"FC_Temp1",g_FCMS_runpara[0].mF_CKWD.flaotword,'C');
				LCD_Write(2,"HJ_Temp1",g_FCMS_runpara[0].mF_JKWD.flaotword,'C');					
				break ;
//			case 4:
//				LCD_Write(1,"FC_Temp2",g_FCMS_runpara[1].mF_CKWD.flaotword,'C');
//				LCD_Write(2,"HJ_Temp2",g_FCMS_runpara[1].mF_JKWD.flaotword,'C');					
//				break ;
//			case 2:
//				LCD_Write(1,"DC_Vol  ",g_DCMS_runpara[0].mD_V,'v');
//				LCD_Write(2,"DC_Power",g_DCMS_runpara[0].mD_V * (g_DCMS_runpara[0].mD_A +g_DCMS_runpara[1].mD_A) ,'w');
//				break;
//			case 3:
//				LCD_Write(1,"FC_Vol  ",g_FCMS_runpara[0].mF_V.flaotword,'v');
//				LCD_Write(2,"FC_Power",g_FCMS_runpara[0].mF_V.flaotword * g_FCMS_runpara[0].mF_A.flaotword,'w');				
//				break ;
//			case 4:
//				LCD_Write(1,"FC_Temp ",g_FCMS_runpara[0].mF_CKWD.flaotword,'C');
//				LCD_Write(2,"HJ_Temp ",g_FCMS_runpara[0].mF_JKWD.flaotword,'C');					
//				break ;
			default:
				page = 0 ;	
				break;
		}
		
#else

		switch(page)
		{
			case 0:
				LCD_Write(1,"SysPower",g_DCMS_runpara[2].mD_P + g_FCMS_BMSpara.mBMS_V * g_FCMS_BMSpara.mBMS_A,'w');
				LCD_Write_Warn("Sys_Warn",MainWarnTransform() , FCWarnTransform() , BMSWarnTransform());				
				break ;
			case 1:
				LCD_Write(1,"BMSPower",g_FCMS_BMSpara.mBMS_V * g_FCMS_BMSpara.mBMS_A,'w');
				LCD_Write(2,"BMS_SOC ",g_FCMS_BMSpara.mBMS_SOC,'%');					
				break ;
			case 2:
				LCD_Write(1,"DC_Vol  ",g_DCMS_runpara[0].mD_V,'v');
				LCD_Write(2,"DC_Power",g_DCMS_runpara[2].mD_P ,'w');
				break;
			case 3:
				LCD_Write(1,"FC_Vol  ",g_FCMS_runpara[0].mF_V.flaotword,'v');
				LCD_Write(2,"FC_Power",g_FCMS_runpara[0].mF_V.flaotword * g_FCMS_runpara[0].mF_A.flaotword,'w');				
				break ;
			case 4:
				LCD_Write(1,"FC_Temp ",g_FCMS_runpara[0].mF_CKWD.flaotword,'C');
				LCD_Write(2,"HJ_Temp ",g_FCMS_runpara[0].mF_JKWD.flaotword,'C');					
				break ;
			default:
				page = 0 ;	
				break;
		}

#endif		
		
		lcdSend(lcdStr.str);
}

void sysOnOff(void)
{
		if(1 == button_function)
		{
					if(1 == g_MCMS_system.msysState)  //开机
					{
							g_MCMS_system.msysOnOffOrder = 1 ;
							button_function = 0 ;
					}
					else if(3 == g_MCMS_system.msysState)  //关机
					{
							g_MCMS_system.msysOnOffOrder = 2 ;
							button_function = 0 ;
					}
		}
}

void Lcd_Control(void)
{
	 if(3 == g_MCMS_system.msysState)
	 {
			FAMEN_2_Control(1);
	 }
	 else
	 {
			FAMEN_2_Control(0);
	 }
}

//字符串连接
void LCD_Task(void)
{
		
		button_control();
		LCD_page(&button_function);
//		sysOnOff();
//		Lcd_Control();
}

/******************************************************************************
**                            End Of File
******************************************************************************/
