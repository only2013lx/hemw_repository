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
#ifndef __PWOERMANAGE_H 
#define __PWOERMANAGE_H

#include <stdint.h>



//纯氢输出功率
#define PURE_H_POWER1 	1000   
#define PURE_H_POWER2 	1500    
#define PURE_H_POWER3 	2000   
#define PURE_H_POWER4 	2500 
#define PURE_H_POWER5 	3000   


#define DC_UPLIMIT 			294  //DC输出上限 29.4v
#define DC_OUTVOL  			290   //DC默认输出29V

extern void powerMange(void) ;
extern void powerInit(void) ;
extern void InsertQuere(float buff[],float dt);
extern void Cal_SOC_Rate(void);
extern uint8_t CalPowerGrade(void);
extern void SetPerisSpeed(int grade);
extern void MakeH2_Prepare(void);
extern void LoadTimehandle(uint16_t destCur);

extern void Cal_SOC_Charge_Time(void) ;

extern void DCDCSetCAN(uint16_t SV_IN ,uint16_t SV , uint16_t SA) ;
extern void DCDCSetPower(float power) ;




extern void socRate_timer_start(void) ;
extern void socRate_timer_stop(void) ;



#endif 
/****************************************************************************
**                            End Of File
****************************************************************************/
