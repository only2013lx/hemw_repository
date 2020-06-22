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

#include "warn.h"
#include "ram.h"
#include "timerDriver.h"
#include "ram.h"
#include <stdint.h>
#include "canDriver.h"
#include "canSendQueue.h"
#include "canTask.h"
#include "ioControl.h"
#include "ucos_ii.h"
#include "am_softimer.h"


volatile uWarnHandle fcWarnHandle;
uint8_t beepflag ;
static am_softimer_t __g_warnTimer ;


//告警定时
uTIMER	DCVTimer ;
uTIMER	DCATimer ;
uTIMER	DDWDTimer ;
uTIMER	HJWDTimer ;
uTIMER	HYLTimer ;
uTIMER	HNDTimer ;
uTIMER	CommonTimer ;
uTIMER	Li_SOCTimer ;


static void __warn_timer_callback(void *p_arg)
{
			//告警定义
		//最大 12.8s
		if(DCVTimer.BitsM.Tim1_enM)
		{
				DCVTimer.BitsM.Tim1_timM++;
		}						
		if(DCVTimer.BitsM.Tim2_enM)
		{
				DCVTimer.BitsM.Tim2_timM++;
		}		
		if(DCVTimer.BitsM.Tim3_enM)
		{
				DCVTimer.BitsM.Tim3_timM++;
		}				
		if(DCVTimer.BitsM.Tim4_enM)
		{
				DCVTimer.BitsM.Tim4_timM++;
		}				
		
		if(DCATimer.BitsM.Tim1_enM)
		{
				DCATimer.BitsM.Tim1_timM++;
		}						
		if(DCATimer.BitsM.Tim2_enM)
		{
				DCATimer.BitsM.Tim2_timM++;
		}		
		if(DCATimer.BitsM.Tim3_enM)
		{
				DCATimer.BitsM.Tim3_timM++;
		}		
		if(DCATimer.BitsM.Tim4_enM)
		{
				DCATimer.BitsM.Tim4_timM++;
		}	
		
//		if(DDWDTimer.BitsM.Tim1_enM)
//		{
//				DDWDTimer.BitsM.Tim1_timM++;
//		}						
//		if(DDWDTimer.BitsM.Tim2_enM)
//		{
//				DDWDTimer.BitsM.Tim2_timM++;
//		}		
//		if(DDWDTimer.BitsM.Tim3_enM)
//		{
//				DDWDTimer.BitsM.Tim3_timM++;
//		}			
//		if(DDWDTimer.BitsM.Tim4_enM)
//		{
//				DDWDTimer.BitsM.Tim4_timM++;
//		}				
		
		if(HJWDTimer.BitsM.Tim1_enM)
		{
				HJWDTimer.BitsM.Tim1_timM++;
		}						
		if(HJWDTimer.BitsM.Tim2_enM)
		{
				HJWDTimer.BitsM.Tim2_timM++;
		}		
		if(HJWDTimer.BitsM.Tim3_enM)
		{
				HJWDTimer.BitsM.Tim3_timM++;
		}	
		if(HJWDTimer.BitsM.Tim4_enM)
		{
				HJWDTimer.BitsM.Tim4_timM++;
		}	
		
		if(HYLTimer.BitsM.Tim1_enM)
		{
				HYLTimer.BitsM.Tim1_timM++;
		}						
		if(HYLTimer.BitsM.Tim2_enM)
		{
				HYLTimer.BitsM.Tim2_timM++;
		}		
		if(HYLTimer.BitsM.Tim3_enM)
		{
				HYLTimer.BitsM.Tim3_timM++;
		}	
		if(HYLTimer.BitsM.Tim4_enM)
		{
				HYLTimer.BitsM.Tim4_timM++;
		}	
		
		if(HNDTimer.BitsM.Tim1_enM)
		{
				HNDTimer.BitsM.Tim1_timM++;
		}						
		if(HNDTimer.BitsM.Tim2_enM)
		{
				HNDTimer.BitsM.Tim2_timM++;
		}		
		if(HNDTimer.BitsM.Tim3_enM)
		{
				HNDTimer.BitsM.Tim3_timM++;
		}	
		if(HNDTimer.BitsM.Tim4_enM)
		{
				HNDTimer.BitsM.Tim4_timM++;
		}
		//告警通用定时器
		if(CommonTimer.BitsM.Tim1_enM)
		{
				CommonTimer.BitsM.Tim1_timM++;
		}						
		if(CommonTimer.BitsM.Tim2_enM)
		{
				CommonTimer.BitsM.Tim2_timM++;
		}		
		if(CommonTimer.BitsM.Tim3_enM)
		{
				CommonTimer.BitsM.Tim3_timM++;
		}	
		if(CommonTimer.BitsM.Tim4_enM)
		{
				CommonTimer.BitsM.Tim4_timM++;
		}
		
		if(Li_SOCTimer.BitsM.Tim1_enM)
		{
				Li_SOCTimer.BitsM.Tim1_timM++;
		}						
		if(Li_SOCTimer.BitsM.Tim2_enM)
		{
				Li_SOCTimer.BitsM.Tim2_timM++;
		}		
		if(Li_SOCTimer.BitsM.Tim3_enM)
		{
				Li_SOCTimer.BitsM.Tim3_timM++;
		}				
		if(Li_SOCTimer.BitsM.Tim4_enM)
		{
				Li_SOCTimer.BitsM.Tim4_timM++;
		}			
}

void warn_Timer_Init(void)
{
		am_softimer_init(&__g_warnTimer , __warn_timer_callback , NULL);
		am_softimer_start(&__g_warnTimer , 1000);
}

#if 1
//传感器未接
/*
uint8_t FV_zero(float zero)
{
		if(zero < DDDY_ZERO)
		{
				DCVTimer.BitsM.Tim1_enM = 1 ;
				if(DCVTimer.BitsM.Tim1_timM >= TIMER_3S)
				{
						g_MCMS_system.msysWarn.Bites.w1 = 1 ;
						DCVTimer.BitsM.Tim1_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				g_MCMS_system.msysWarn.Bites.w1 = 0 ;	
				DCVTimer.ByteM.Tim1 = 0 ;
				return 0 ;
		}
		return 0 ;
}

//电堆电压
uint8_t FV_High_Warn(float para)
{
		if(para > DDDY_HIGH)
		{
				DCVTimer.BitsM.Tim2_enM = 1 ;
				if(DCVTimer.BitsM.Tim2_timM >= TIMER_3S)
				{
						g_MCMS_system.msysWarn.Bites.w9 = 1 ;
						DCVTimer.BitsM.Tim2_timM = 0 ;
						return 1 ;
				}
		}
		else
		{
				g_MCMS_system.msysWarn.Bites.w9 = 0 ;	
				DCVTimer.ByteM.Tim2  = 0 ;
				return 0 ;
		}
		return 0 ;
}
*/
uint8_t DCV_High_Over_Warn(float para)
{
//		printf("%f,%f\n" , para , DCV_HIGH_OVER);
		if(para > DCV_HIGH_OVER)
		{
				DCVTimer.BitsM.Tim4_enM = 1 ;
				if(DCVTimer.BitsM.Tim4_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w22 = 1 ;
						DCVTimer.BitsM.Tim4_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w22 = 0 ;	
				DCVTimer.ByteM.Tim4  = 0 ;
				return 0 ;
		}
		return 0 ;
}
/*
uint8_t FV_Low_Warn(float para)
{
		if(para < DDDY_LOW)
		{
				DCVTimer.BitsM.Tim4_enM = 1 ;
				if(DCVTimer.BitsM.Tim4_timM >= TIMER_3S)
				{
						g_MCMS_system.msysWarn.Bites.w10 = 1 ;
						DCVTimer.BitsM.Tim4_timM = 0 ;
						return 1 ;
				}
		}
		else
		{
				g_MCMS_system.msysWarn.Bites.w10 = 0 ;	
				DCVTimer.ByteM.Tim4  = 0 ;
				return 0 ;
		}
		return 0 ;
}
*/
uint8_t DCV_Low_Over_Warn(float para)
{
		if(para < DCV_LOW_OVER)
		{
				DCVTimer.BitsM.Tim3_enM= 1 ;
				if(DCVTimer.BitsM.Tim3_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w21 = 1 ;
						DCVTimer.BitsM.Tim3_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w21 = 0 ;	
				DCVTimer.ByteM.Tim3  = 0 ;
				return 0 ;
		}
		return 0 ;
}

static void DCV_Warn_Check(void)
{
	/*
		if(FV_zero(g_FCMS_runpara[0].mF_V.flaotword))
		{
				g_MCMS_system.msysWarn.Bites.w9  = 0;
				g_MCMS_system.msysWarn.Bites.w10 = 0;
				g_MCMS_system.msysWarn.Bites.w21 = 0;
				g_MCMS_system.msysWarn.Bites.w22 = 0;
		}
		else*/ if(DCV_High_Over_Warn(g_DCMS_runpara[0].mD_V))
		{
				fcWarnHandle.sysWarn.Bites.w9 = 0;	
		}
		else if(DCV_Low_Over_Warn(g_DCMS_runpara[0].mD_V))
		{
				fcWarnHandle.sysWarn.Bites.w10 = 0;
		}/*
		else if(FV_High_Warn(g_FCMS_runpara[0].mF_V.flaotword))
		{
				;
		}
		else if(FV_Low_Warn(g_FCMS_runpara[0].mF_V.flaotword))
		{
				;
		}*/
}

static uint8_t DCW_High_Over_Warn(float para)
{
		if(para > DCW_HIGH_OVER)
		{
				DCATimer.BitsM.Tim4_enM = 1 ;
				if(DCATimer.BitsM.Tim4_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w24 = 1 ;
						DCATimer.BitsM.Tim4_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w24 = 0 ;	
				DCATimer.ByteM.Tim4  = 0 ;
				return 0 ;
		}
		return 0 ;
}


static void DCW_Warn_Check(void)
{
		float tmp = 0 ;
		tmp = g_DCMS_runpara[0].mD_V * (g_DCMS_runpara[0].mD_A + g_DCMS_runpara[1].mD_A + g_DCMS_runpara[2].mD_A);
	
		if(DCW_High_Over_Warn(tmp))
		{
				fcWarnHandle.sysWarn.Bites.w12 = 0;	
		}

}

/*
uint8_t FA_zero(float zero)
{
		if(zero < DDDL_ZERO)
		{
				DCATimer.BitsM.Tim1_enM  = 1 ;
				if(DCATimer.BitsM.Tim1_timM >= TIMER_3S)
				{
						g_MCMS_system.msysWarn.Bites.w2 = 1 ;
						DCATimer.BitsM.Tim1_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				g_MCMS_system.msysWarn.Bites.w2 = 0 ;	
				DCATimer.ByteM.Tim1 = 0 ;
				return 0 ;
		}
		return 0 ;
}*/

//电堆电流
/*
uint8_t FA_High_Warn(float para)
{
		if(para > DDDL_HIGH)
		{
				DCATimer.BitsM.Tim2_enM = 1 ;
				if(DCATimer.BitsM.Tim2_timM >= TIMER_3S)
				{
						g_MCMS_system.msysWarn.Bites.w11 = 1 ;
						DCATimer.BitsM.Tim2_timM = 0 ;
						return 1 ;
				}
		}
		else
		{
				g_MCMS_system.msysWarn.Bites.w11 = 0 ;	
				DCATimer.ByteM.Tim2  = 0 ;
				return 0 ;
		}
		return 0 ;
}
*/
uint8_t DCA_High_Over_Warn(float para)
{
		if(para > DCA_HIGH_OVER)
		{
				DCATimer.BitsM.Tim3_enM = 1 ;
				if(DCATimer.BitsM.Tim3_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w23 = 1 ;
						DCATimer.BitsM.Tim3_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w23 = 0 ;	
				DCATimer.ByteM.Tim3  = 0 ;
				return 0 ;
		}
		return 0 ;
}

static void DCA_Warn_Check(void)
{
		float tmp = 0 ;
	/*
		if(FA_zero(g_FCMS_runpara[0].mF_A.flaotword))
		{
				g_MCMS_system.msysWarn.Bites.w23 = 0;
				g_MCMS_system.msysWarn.Bites.w11 = 0;
		}
		else*/ 
	
		tmp = g_DCMS_runpara[0].mD_A + g_DCMS_runpara[1].mD_A ;
		
		if(DCA_High_Over_Warn(tmp))
		{
				fcWarnHandle.sysWarn.Bites.w11 = 0;	
		}/*
		else if(DCA_High_Warn(g_FCMS_runpara[0].mF_A.flaotword))
		{
				;
		}*/
}
/*
uint8_t DDWD_zero(float zero)
{
		if(zero < DDWD_ZERO)
		{
				DDWDTimer.BitsM.Tim1_enM  = 1 ;
				if(DDWDTimer.BitsM.Tim1_timM >= TIMER_3S)
				{
						g_MCMS_system.msysWarn.Bites.w3 = 1 ;
						DDWDTimer.BitsM.Tim1_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				g_MCMS_system.msysWarn.Bites.w3 = 0 ;	
				DDWDTimer.ByteM.Tim1  = 0 ;
				return 0 ;
		}
		return 0 ;
}

//电堆温度
uint8_t DDWD_High_Warn(float para)
{
		if(para > DDWD_HIGH)
		{
				DDWDTimer.BitsM.Tim2_enM = 1 ;
				if(DDWDTimer.BitsM.Tim2_timM >= TIMER_3S)
				{
						g_MCMS_system.msysWarn.Bites.w12 = 1 ;
						DDWDTimer.BitsM.Tim2_timM = 0 ;
						return 1 ;
				}
		}
		else
		{
				g_MCMS_system.msysWarn.Bites.w12 = 0 ;	
				DDWDTimer.ByteM.Tim2  = 0 ;
				return 0 ;
		}
		return 0 ;
}

uint8_t DDWD_High_Over_Warn(float para)
{
		if(para > DDWD_HIGH_OVER)
		{
				
				DDWDTimer.BitsM.Tim3_enM = 1 ;
				if(DDWDTimer.BitsM.Tim3_timM >= TIMER_3S)
				{
						
						g_MCMS_system.msysWarn.Bites.w24 = 1 ;
						DDWDTimer.BitsM.Tim3_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				g_MCMS_system.msysWarn.Bites.w24 = 0 ;	
				DDWDTimer.ByteM.Tim3  = 0 ;
				return 0 ;
		}
		return 0 ;
}

uint8_t DDWD_Low_Warn(float para)
{
		if(para < DDWD_LOW)
		{
				DDWDTimer.BitsM.Tim4_enM = 1 ;
				if(DDWDTimer.BitsM.Tim4_timM >= TIMER_3S)
				{
						g_MCMS_system.msysWarn.Bites.w13 = 1 ;
						DDWDTimer.BitsM.Tim4_timM = 0 ;
						return 1 ;
				}
		}
		else
		{
				g_MCMS_system.msysWarn.Bites.w13 = 0 ;	
				DDWDTimer.ByteM.Tim4  = 0 ;
				return 0 ;
		}
		return 0 ;
}

uint8_t DDWD_Low_Over_Warn(float para)
{
		if(para < DDWD_LOW_OVER)
		{
				CommonTimer.BitsM.Tim2_enM = 1 ;
				if(CommonTimer.BitsM.Tim2_timM >= TIMER_3S)
				{
						g_MCMS_system.msysWarn.Bites.w25 = 1 ;
						CommonTimer.BitsM.Tim2_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				g_MCMS_system.msysWarn.Bites.w25 = 0 ;	
				CommonTimer.ByteM.Tim2  = 0 ;
				return 0 ;
		}
		return 0 ;
}

static void DDWD_Warn_Check(void)
{
		if(DDWD_zero(g_FCMS_runpara[0].mF_CKWD.flaotword))
		{
				g_MCMS_system.msysWarn.Bites.w12 = 0;
				g_MCMS_system.msysWarn.Bites.w13 = 0;
				g_MCMS_system.msysWarn.Bites.w24 = 0;
				g_MCMS_system.msysWarn.Bites.w25 = 0;
		}
		else if(DDWD_High_Over_Warn(g_FCMS_runpara[0].mF_CKWD.flaotword))
		{
				g_MCMS_system.msysWarn.Bites.w12 = 0;	
		}
		else if(DDWD_Low_Over_Warn(g_FCMS_runpara[0].mF_CKWD.flaotword))
		{
				g_MCMS_system.msysWarn.Bites.w13 = 0;
		}
		else if(DDWD_High_Warn(g_FCMS_runpara[0].mF_CKWD.flaotword))
		{
				;
		}
		else if(DDWD_Low_Warn(g_FCMS_runpara[0].mF_CKWD.flaotword))
		{
				;
		}
}
*/
uint8_t HJWD_zero(float zero)
{
		if(zero < HJWD_ZERO)
		{
				HJWDTimer.BitsM.Tim1_enM = 1 ;
				if(HJWDTimer.BitsM.Tim1_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w3 = 1 ;
						HJWDTimer.BitsM.Tim1_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w3 = 0 ;	
				HJWDTimer.ByteM.Tim1 = 0 ;
				return 0 ;
		}
		return 0 ;
}

//环境温度
uint8_t HJWD_High_Warn(float para)
{
		if(para > HJWD_HIGH)
		{
				HJWDTimer.BitsM.Tim2_enM = 1 ;
				if(HJWDTimer.BitsM.Tim2_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w16 = 1 ;
						HJWDTimer.BitsM.Tim2_timM = 0 ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w16 = 0 ;	
				HJWDTimer.ByteM.Tim2  = 0 ;
				return 0 ;
		}
		return 0 ;
}

uint8_t HJWD_High_Over_Warn(float para)
{
		if(para > HJWD_HIGH_OVER)
		{
				HJWDTimer.BitsM.Tim3_enM = 1 ;
				if(HJWDTimer.BitsM.Tim3_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w28 = 1 ;
						HJWDTimer.BitsM.Tim3_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w28 = 0 ;	
				HJWDTimer.ByteM.Tim3  = 0 ;
				return 0 ;
		}
		return 0 ;
}
/*
uint8_t HJWD_Low_Warn(float para)
{
		if(para < HJWD_LOW)
		{
				HJWDTimer.BitsM.Tim4_enM = 1 ;
				if(HJWDTimer.BitsM.Tim4_timM >= TIMER_3S)
				{
						g_MCMS_system.msysWarn.Bites.w15 = 1 ;
						HJWDTimer.BitsM.Tim4_timM = 0 ;
						return 1 ;
				}
		}
		else
		{
				g_MCMS_system.msysWarn.Bites.w15 = 0 ;	
				HJWDTimer.ByteM.Tim4  = 0 ;
				return 0 ;
		}
		return 0 ;
}

uint8_t HJWD_Low_Over_Warn(float para)
{
		if(para < HJWD_LOW_OVER)
		{
				CommonTimer.BitsM.Tim3_enM = 1 ;
				if(CommonTimer.BitsM.Tim3_timM >= TIMER_3S)
				{
						g_MCMS_system.msysWarn.Bites.w27 = 1 ;
						CommonTimer.BitsM.Tim3_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				g_MCMS_system.msysWarn.Bites.w27 = 0 ;	
				CommonTimer.ByteM.Tim3  = 0 ;
				return 0 ;
		}
		return 0 ;
}
*/
void HJWD_Warn_Check(void)
{
		//printf("%f,%f,%f,%f\n" , g_MCMS_runpara.mF_HJWD.flaotword , HJWD_ZERO , HJWD_HIGH , HJWD_HIGH_OVER);
		if(HJWD_zero(g_MCMS_runpara.mF_HJWD.flaotword))
		{
				fcWarnHandle.sysWarn.Bites.w16 = 0;
				fcWarnHandle.sysWarn.Bites.w28 = 0;
		}
		else if(HJWD_High_Over_Warn(g_MCMS_runpara.mF_HJWD.flaotword))
		{
				fcWarnHandle.sysWarn.Bites.w16 = 0;	
		}/*
		else if(HJWD_Low_Over_Warn(g_FCMS_runpara[0].mF_JKWD.flaotword))
		{
				g_MCMS_system.msysWarn.Bites.w15 = 0;
		}*/
		else if(HJWD_High_Warn(g_MCMS_runpara.mF_HJWD.flaotword))
		{
				;
		}/*
		else if(HJWD_Low_Warn(g_FCMS_runpara[0].mF_JKWD.flaotword))
		{
				;
		}*/
}
/*
uint8_t HYL_zero(float zero)
{
		if(zero < HYL_ZERO)
		{
				HYLTimer.BitsM.Tim1_enM = 1 ;
				if(HYLTimer.BitsM.Tim1_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w1 = 1 ;
						HYLTimer.BitsM.Tim1_timM = TIMER_3S ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w1 = 0 ;	
				HYLTimer.ByteM.Tim1 = 0 ;
				return 0 ;
		}
		return 0 ;
}

//氢气压力
uint8_t HYL_High_Warn(float para)
{
		if(para > HYL_HIGH)
		{
				HYLTimer.BitsM.Tim2_enM = 1 ;
				if(HYLTimer.BitsM.Tim2_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w13 = 1 ;
						HYLTimer.BitsM.Tim2_timM = 0 ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w13 = 0 ;	
				HYLTimer.ByteM.Tim2  = 0 ;
				return 0 ;
		}
		return 0 ;
}

uint8_t HYL_High_Over_Warn(float para)
{
		if(para > HYL_HIGH_OVER)
		{
				HYLTimer.BitsM.Tim3_enM = 1 ;
				if(HYLTimer.BitsM.Tim3_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w25 = 1 ;
						HYLTimer.BitsM.Tim3_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w25 = 0 ;	
				HYLTimer.ByteM.Tim3  = 0 ;
				return 0 ;
		}
		return 0 ;
}

uint8_t HYL_Low_Warn(float para)
{
		if(para < HYL_LOW)
		{
				HYLTimer.BitsM.Tim4_enM = 1 ;
				if(HYLTimer.BitsM.Tim4_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w14 = 1 ;
						HYLTimer.BitsM.Tim4_timM = 0 ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w14 = 0 ;	
				HYLTimer.ByteM.Tim4  = 0 ;
				return 0 ;
		}
		return 0 ;
}

uint8_t HYL_Low_Over_Warn(float para)
{
//		printf("%f,%f\n" , para, HYL_LOW_OVER);
		if(para < HYL_LOW_OVER)
		{
				CommonTimer.BitsM.Tim4_enM = 1 ;
				if(CommonTimer.BitsM.Tim4_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w26 = 1 ;
						CommonTimer.BitsM.Tim4_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w26 = 0 ;	
				CommonTimer.ByteM.Tim4  = 0 ;
				return 0 ;
		}
		return 0 ;
}

static void HYL_Warn_Check(void)
{
		if(HYL_zero(g_MCMS_runpara.mH_YL.flaotword))
		{
				fcWarnHandle.sysWarn.Bites.w13 = 0;
				fcWarnHandle.sysWarn.Bites.w14 = 0;
				fcWarnHandle.sysWarn.Bites.w25 = 0;
				fcWarnHandle.sysWarn.Bites.w26 = 0;
		}
		else if(HYL_High_Over_Warn(g_MCMS_runpara.mH_YL.flaotword))
		{
				fcWarnHandle.sysWarn.Bites.w13 = 0;	
		}
		else if(HYL_Low_Over_Warn(g_MCMS_runpara.mH_YL.flaotword))
		{
				fcWarnHandle.sysWarn.Bites.w14 = 0;
		}
		else if(HYL_High_Warn(g_MCMS_runpara.mH_YL.flaotword))
		{
				;
		}
		else if(HYL_Low_Warn(g_MCMS_runpara.mH_YL.flaotword))
		{
				;
		}
}
*/
uint8_t HND_zero(float zero)
{
		if(zero <= HND_ZERO)
		{
				HNDTimer.BitsM.Tim1_enM = 1 ;
				if(HNDTimer.BitsM.Tim1_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w2 = 1 ;
						HNDTimer.BitsM.Tim1_timM  = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w2 = 0 ;
				HNDTimer.ByteM.Tim1 = 0 ;
				return 0 ;
		}
		return 0 ;
}

//氢气浓度
uint8_t HND_High_Warn(float para)
{
		if(para > HND_HIGH)
		{
				HNDTimer.BitsM.Tim2_enM = 1 ;
				if(HNDTimer.BitsM.Tim2_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w15 = 1 ;
						HNDTimer.BitsM.Tim2_timM = 0 ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w15 = 0 ;	
				HNDTimer.ByteM.Tim2  = 0 ;
				return 0 ;
		}
		return 0 ;
}

uint8_t HND_High_Over_Warn(float para)
{
		if(para > HND_HIGH_OVER)
		{
				HNDTimer.BitsM.Tim3_enM = 1 ;
				if(HNDTimer.BitsM.Tim3_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w27 = 1 ;
						HNDTimer.BitsM.Tim3_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w27 = 0 ;	
				HNDTimer.ByteM.Tim3  = 0 ;
				return 0 ;
		}
		return 0 ;
}

static void HND_Warn_Check(void)
{
//		printf("HND:%f,%f,%f\n" , g_MCMS_runpara.mH_ND.flaotword , HND_HIGH_OVER , HND_HIGH);
		if(HND_zero(g_MCMS_runpara.mH_ND.flaotword))
		{
				fcWarnHandle.sysWarn.Bites.w18 = 0;
				fcWarnHandle.sysWarn.Bites.w30 = 0;
		}
		else if(HND_High_Over_Warn(g_MCMS_runpara.mH_ND.flaotword))
		{
				fcWarnHandle.sysWarn.Bites.w18 = 0;	
		}
		else if(HND_High_Warn(g_MCMS_runpara.mH_ND.flaotword))
		{
				;
		}
}

void warn_Del(void)
{
//		printf("w:0x%x,0x%x\n",g_MCMS_system.msysWarn.word,g_FCMS_system[0].msysWarn.word);
//	printf("%d\n" , );

		//声光告警
		if((g_MCMS_system.msysWarn.word & 0x7f00007) || (g_FCMS_system[0].msysWarn.word & 0x1f0003f))
		{	
				//打开告警灯
//				YellowLed(1);
				beepflag = 1 ;
		}
		else if(g_MCMS_system.msysWarn.word | g_FCMS_system[0].msysWarn.word)
		{	
//				YellowLed(1);
				beepflag = 1 ;			
		}
		else
		{
				 if(3 == g_MCMS_system.msysState)
				 {
//						GreenLed(1) ;			
				 }
				 else
				 {
	//					GreenLed(0) ;	
				 }
//				 YellowLed(0);
				 beepflag = 0 ;		
		}	
}

void turnOff(void)
{
		j1939_message sendMessage ;
		sendMessage.channel 	     = CAN2 ;
		sendMessage.FF      			 = 1 ;
		sendMessage.LEN     			 = 2 ;		
		sendMessage.std_id.ID 		 = 0x8148c01 ;
		sendMessage.mCANDF.DATA[0] = CONTROL_ONOFF;
		sendMessage.mCANDF.DATA[1] = 2;
		WriteCANSedCyBuf(CAN2 , &sendMessage);
	
		sendMessage.channel 	     = CAN2 ;
		sendMessage.FF      			 = 1 ;
		sendMessage.LEN     			 = 2 ;		
		sendMessage.std_id.ID 		 = 0x8148d01 ;
		sendMessage.mCANDF.DATA[0] = CONTROL_ONOFF;
		sendMessage.mCANDF.DATA[1] = 2;
		WriteCANSedCyBuf(CAN2 , &sendMessage);
}


///////////////////////////锂电池SOC告警///////////////////////////
uint8_t Li_SOC_High_Warn(float para)
{
		if(para > Li_SOC_HIGH)
		{
				Li_SOCTimer.BitsM.Tim2_enM = 1 ;
				if(Li_SOCTimer.BitsM.Tim2_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w16 = 1 ;
						Li_SOCTimer.BitsM.Tim2_timM = 0 ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w16 = 0 ;	
				Li_SOCTimer.ByteM.Tim2  = 0 ;
				return 0 ;
		}
		return 0 ;
}

uint8_t Li_SOC_High_Over_Warn(float para)
{
		if(para > Li_SOC_HIGH_OVER)
		{
				Li_SOCTimer.BitsM.Tim3_enM = 1 ;
				if(Li_SOCTimer.BitsM.Tim3_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w28 = 1 ;
						Li_SOCTimer.BitsM.Tim3_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w28 = 0 ;	
				Li_SOCTimer.ByteM.Tim3  = 0 ;
				return 0 ;
		}
		return 0 ;
}

uint8_t Li_SOC_Low_Warn(float para)
{
		if(para < Li_SOC_LOW)
		{
				Li_SOCTimer.BitsM.Tim1_enM = 1 ;
				if(Li_SOCTimer.BitsM.Tim1_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w17 = 1 ;
						Li_SOCTimer.BitsM.Tim1_timM = 0 ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w17 = 0 ;	
				Li_SOCTimer.ByteM.Tim1  = 0 ;
				return 0 ;
		}
		return 0 ;
}

uint8_t Li_SOC_Low_Over_Warn(float para)
{
		if(para < Li_SOC_LOW_OVER)
		{
				Li_SOCTimer.BitsM.Tim4_enM = 1 ;
				if(Li_SOCTimer.BitsM.Tim4_timM >= TIMER_3S)
				{
						fcWarnHandle.sysWarn.Bites.w29 = 1 ;
						Li_SOCTimer.BitsM.Tim4_timM = TIMER_3S ;
						return 1 ;
				}
		}
		else
		{
				fcWarnHandle.sysWarn.Bites.w29 = 0 ;	
				Li_SOCTimer.ByteM.Tim4  = 0 ;
				return 0 ;
		}
		return 0 ;
}

void Li_SOC_Warn_Check(void)
{
		if(Li_SOC_High_Over_Warn(g_FCMS_BMSpara.mBMS_SOC))
		{
				fcWarnHandle.sysWarn.Bites.w16 = 0;	
		}
		else if(Li_SOC_Low_Over_Warn(g_FCMS_BMSpara.mBMS_SOC))
		{
				fcWarnHandle.sysWarn.Bites.w17 = 0;
		}
		else if(Li_SOC_High_Warn(g_FCMS_BMSpara.mBMS_SOC))
		{
				;
		}
		else if(Li_SOC_Low_Warn(g_FCMS_BMSpara.mBMS_SOC))
		{
				;
		}		
}

void Warnning_Check_Task(void)
{
		static uint8_t flag = 0 ;
		warn_Del() ;
//		printf("fc：0x%x,0x%x\n" , fcWarnHandle.sysWarn.word , g_MCMS_system.msysWarn.word);
		// && (3 != g_FCMS_system[0].msysState)
#if 1		
//		//系统告警关机，状态保持
//		if((g_MCMS_system.msysWarn.word & 0xfff000ff))
//		{
//				return ;
//		}
//		
//		//电堆告警关机,状态保持
//		if(g_FCMS_system[0].msysWarn.word & 0xfff0003f)
//		{
//				return ;
//		}
	
		if(g_MCMS_system.msysState >= 4 || 3 == flag)
		{
				return ;
		}
	
		if(3 == g_FCMS_system[0].msysState)
		{
				//开机，等待3s，再检测告警
				if(0 == flag)
				{
						OSTimeDlyHMSM(0, 0, 3, 0);
						flag = 1 ;
				}
				//主控告警检测
				DCV_Warn_Check() ;	
				DCA_Warn_Check() ;  
				DCW_Warn_Check() ;
//				HYL_Warn_Check() ;
		}
		if(g_MCMS_system.msysState > 3)
		{
				//系统关机，二级告警解除
				g_MCMS_system.msysWarn.word &= ~0xfff00;
		}
//		DDWD_Warn_Check();
//		HJWD_Warn_Check();
		#endif
		Li_SOC_Warn_Check();
		HND_Warn_Check();
		//HJWD_Warn_Check();
//		printf("0x%x,0x%x\n" , fcWarnHandle.sysWarn.word , g_MCMS_system.msysWarn.word);
//		printf("HND\n");
		//主控告警，关机                                       电堆告警，关机
		if((g_MCMS_system.msysWarn.word & 0xfff000ff)  || (g_FCMS_system[0].msysWarn.word & 0xfff0003f))
		{
				//关机
				turnOff();
				flag = 3 ;
//				printf("trun off\n");
				if(g_MCMS_system.msysState == 1)
				{
						g_MCMS_system.msysState = 5 ;
				}
		}
}

#endif

/******************************************************************************
**                            End Of File
******************************************************************************/
