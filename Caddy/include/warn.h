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
#ifndef __WARN_H 
#define __WARN_H

#include <stdint.h>

#include "system_LPC17xx.h"
#include "lpc17xx.h"

//#include "ram.h"
#include "ioControl.h"
#include "IAPFlashDriver.h"
/*********************************
*					告警状态数据结构
* 1：告警        0：正常
* 可同时容纳 32种告警状态
*********************************/
typedef union  _uWARN_
{
	uint32_t	word;
	struct{
		uint32_t	DATA0		:8;
		uint32_t	DATA1		:8;
		uint32_t	DATA2		:8;
		uint32_t	DATA3		:8;
	}Bits;
	struct{
		uint32_t	w1			:1;		
		uint32_t	w2			:1;		
		uint32_t	w3			:1;	
		uint32_t	w4			:1;		
		uint32_t	w5			:1;		
		uint32_t	w6			:1;
		uint32_t	w7			:1;
		uint32_t	w8			:1;
		
		uint32_t	w9			:1;		
		uint32_t	w10			:1;		
		uint32_t	w11			:1;		
		uint32_t	w12			:1;
		uint32_t	w13			:1;
		uint32_t	w14			:1;
		uint32_t	w15			:1;
		uint32_t	w16			:1;
		
		uint32_t	w17			:1;		
		uint32_t	w18			:1;		
		uint32_t	w19			:1;
		uint32_t	w20			:1;
		uint32_t	w21			:1;
		uint32_t	w22			:1;
		uint32_t	w23			:1;
		uint32_t	w24			:1;
		
		uint32_t	w25			:1;		
		uint32_t	w26			:1;	
		uint32_t	w27			:1;
		uint32_t	w28			:1;
		uint32_t	w29			:1;
		uint32_t	w30			:1;
		uint32_t	w31			:1;
		uint32_t	w32			:1;
	}Bites;
}*pUWARN,uWARN;	


//告警	
typedef struct  _uWarnHandle_
{
		uWARN					sysWarn ;							//系统报警
//		uTIMER				warnReportTimer ;			//告警上报时间
//		uBeepHandle		beep       ;					//蜂鸣器	
		void (*warnCanSend)(void);					//执行程序
}*P_uWarnHandle,uWarnHandle;

/*
//传感器检测
typedef struct  _upara_Handle_
{
		uint8_t zero_warn_flag ;
		uint8_t high_warn_flag ;
		uint8_t highOver_warn_flag ;
		uint8_t low_warn_flag ;
		uint8_t lowOver_warn_flag ;
	
		//断线
		uint16_t zero ;
		//高
		uint16_t high ;
		//过高
		uint16_t highOver ;
		//低
		uint16_t low ;
		//过低
		uint16_t lowOver ;
	
		void (*warnCheck_zero)(void);
		void (*warnCheck_high)(void) ;
		void (*warnCheck_highOver)(void);
		void (*warnCheck_low)(void);
		void (*warnCheck_lowOver)(void);

}*P_uparaHandle,uparaHandle;
*/

//#define		TIMER_10mS 		1UL
//#define		TIMER_50mS 		5UL
//#define		TIMER_100mS 	1UL
//#define		TIMER_200mS		2UL
//#define		TIMER_300mS		3UL
//#define		TIMER_500mS		5UL
#define		TIMER_1S	    1UL
#define		TIMER_2S	    2UL
#define		TIMER_3S	    3UL
#define		TIMER_4S	    4UL
#define		TIMER_5S	    5UL
#define		TIMER_10S			10UL
#define		TIMER_20S			20UL
#define		TIMER_60S			60UL
#define		TIMER_120S		120UL
#define		TIMER_5min		300UL
#define		TIMER_12min		720UL
#define   TIMER_15min		900UL
#define		TIMER_20min		120UL
#define		TIMER_60min		360UL

//时间结构体与联合体定义
typedef union  _uTIMER_
{
//-----------------------------------
	uint32_t	Word;
	struct{
		uint32_t	Tim_timLL 		:31;
		uint32_t	Tim_enLL 			:1;
	}BitsLL;
//-----------------------------------
	struct{
		uint32_t	Tim1_L				:16;
		uint32_t	Tim2_LM				:8;
		uint32_t	Tim3_LM				:8;
	}ByteL;
	struct{
		uint16_t	Tim1_timL			:15;
		uint16_t	Tim1_enL			:1;
		uint16_t	Tim2_timLM		:7;
		uint16_t	Tim2_enLM			:1;
		uint16_t	Tim3_timLM		:7;
		uint16_t	Tim3_enLM			:1;
	}BitsL;
//-----------------------------------
	struct{
		uint32_t	Tim1					:8;
		uint32_t	Tim2					:8;
		uint32_t	Tim3					:8;
		uint32_t	Tim4					:8;//high bit
	}ByteM;
	struct{
		uint16_t	Tim1_timM			:7;
		uint16_t	Tim1_enM			:1;
		uint16_t	Tim2_timM			:7;
		uint16_t	Tim2_enM			:1;
		uint16_t	Tim3_timM			:7;
		uint16_t	Tim3_enM			:1;
		uint16_t	Tim4_timM			:7;
		uint16_t	Tim4_enM			:1;
	}BitsM;
}*P_uTIMER,uTIMER;


//告警限值
//输出电压
#define DCV_ZERO								-1
//#define DCV_HIGH								(FLASHMEMORYMAP->Bits.warnBites.Warn_DDDY_H / 10.0)
#define DCV_HIGH_OVER          (FLASHMEMORYMAP->Bits.warnBites.Warn_DCV_H_Over / 10.0)
//#define DCV_LOW                (FLASHMEMORYMAP->Bits.warnBites.Warn_DDDY_L / 10.0)
#define DCV_LOW_OVER           (FLASHMEMORYMAP->Bits.warnBites.Warn_DCV_L_Over / 10.0)

//输出电流
#define DCA_ZERO               -1
//#define DCA_HIGH               (FLASHMEMORYMAP->Bits.warnBites.Warn_DDDL_H / 10.0) 
#define DCA_HIGH_OVER          (FLASHMEMORYMAP->Bits.warnBites.Warn_DCA_H_Over / 10.0)

//输出功率
#define DCW_HIGH_OVER          (FLASHMEMORYMAP->Bits.warnBites.Warn_DCW_H_Over / 10.0)

//电堆温度                      
//#define DDWD_ZERO								-1
//#define DDWD_HIGH								(FLASHMEMORYMAP->Bits.warnBites.Warn_DDWD_H / 10.0)
//#define DDWD_HIGH_OVER          (FLASHMEMORYMAP->Bits.warnBites.Warn_DDWD_H_Over / 10.0)
//#define DDWD_LOW                (FLASHMEMORYMAP->Bits.warnBites.Warn_DDWD_L / 10.0)
//#define DDWD_LOW_OVER           (FLASHMEMORYMAP->Bits.warnBites.Warn_DDWD_L_Over / 10.0)

//环境温度
#define HJWD_ZERO               -100.0
#define HJWD_HIGH               (FLASHMEMORYMAP->Bits.warnBites.Warn_HJWD_H / 10.0)
#define HJWD_HIGH_OVER          (FLASHMEMORYMAP->Bits.warnBites.Warn_HJWD_H_Over / 10.0)
//#define HJWD_LOW                (FLASHMEMORYMAP->Bits.warnBites.Warn_HJWD_L / 10.0)
//#define HJWD_LOW_OVER           (FLASHMEMORYMAP->Bits.warnBites.Warn_HJWD_L_Over / 10.0)

//氢气压力
#define HYL_ZERO               -100
#define HYL_HIGH               (FLASHMEMORYMAP->Bits.warnBites.Warn_HYL_H / 100.0)
#define HYL_HIGH_OVER          (FLASHMEMORYMAP->Bits.warnBites.Warn_HYL_H_Over / 100.0)
#define HYL_LOW                (FLASHMEMORYMAP->Bits.warnBites.Warn_HYL_L / 100.0)
#define HYL_LOW_OVER           (FLASHMEMORYMAP->Bits.warnBites.Warn_HYL_L_Over / 100.0)

//氢气浓度
#define HND_ZERO               -10.0
#define HND_HIGH               (FLASHMEMORYMAP->Bits.warnBites.Warn_HND_H / 100.0)
#define HND_HIGH_OVER          (FLASHMEMORYMAP->Bits.warnBites.Warn_HND_H_Over / 100.0)

//锂电池SOC
//#define Li_SOC_HIGH             100	//(FLASHMEMORYMAP->Bits.warnBites.Warn_SOC_H / 10.0)
//#define Li_SOC_HIGH_OVER        (FLASHMEMORYMAP->Bits.warnBites.Warn_SOC_H_Over / 10.0)
//#define Li_SOC_LOW              0		//(FLASHMEMORYMAP->Bits.warnBites.Warn_SOC_L / 10.0)
//#define Li_SOC_LOW_OVER         (FLASHMEMORYMAP->Bits.warnBites.Warn_SOC_L_Over / 10.0)
#define Li_SOC_HIGH             (FLASHMEMORYMAP->Bits.warnBites.Warn_SOC_H_Over / 10.0)
#define Li_SOC_HIGH_OVER        100
#define Li_SOC_LOW              (FLASHMEMORYMAP->Bits.warnBites.Warn_SOC_L_Over / 10.0)
#define Li_SOC_LOW_OVER         0

extern volatile uWarnHandle fcWarnHandle;

static void DCV_Warn_Check(void) ;
static void DCA_Warn_Check(void) ;
//static void DDWD_Warn_Check(void) ;
//static void HJWD_Warn_Check(void) ;
static void HYL_Warn_Check(void) ;
static void HND_Warn_Check(void) ;


/*
//线路未接
extern void FV_zero(float zero);
extern void FA_zero(float zero);
extern void DDWD_zero(float zero);
extern void HJWD_zero(float zero);
extern void HYL_zero(float zero);
extern void HND_zero(float zero);
//告警
extern void FV_High_Warn(float para);
extern void FV_Low_Warn(float para);
extern void FA_High_Warn(float para);
extern void DDWD_High_Warn(float para);
extern void DDWD_Low_Warn(float para);
extern void HJWD_High_Warn(float para);
extern void HJWD_Low_Warn(float para);
extern void HYL_High_Warn(float para);
extern void HYL_Low_Warn(float para);
extern void HND_High_Warn(float para);
//关机
extern void FV_High_Over_Warn(float para);
extern void FV_Low_Over_Warn(float para);
extern void FA_High_Over_Warn(float para);
extern void DDWD_High_Over_Warn(float para);
extern void DDWD_Low_Over_Warn(float para);
extern void HJWD_High_Over_Warn(float para);
extern void HJWD_Low_Over_Warn(float para);
extern void HYL_High_Over_Warn(float para);
extern void HYL_Low_Over_Warn(float para);
extern void HND_High_Over_Warn(float para);

*/

extern void Warnning_Check_Task(void) ;
extern void warn_Timer_Init(void) ;

#endif 
/****************************************************************************
**                            End Of File
****************************************************************************/
