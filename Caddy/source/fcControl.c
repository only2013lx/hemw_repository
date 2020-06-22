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
#include "fcControl.h"
#include "iocontrol.h"
#include "pwmDriver.h"
#include "ram.h"
#include "IAPFlashDriver.h"
#include "finiteStateMachine.h"

#include "canSendQueue.h"
#include <stdio.h>
#include "am_softimer.h"
#include "am_vdebug.h"


static PID pid ;

static am_softimer_t __g_ControlTimer ;
static uint8_t __g_pid_timer = 0 ;
static uint8_t __g_A_Inter_timer = 0 ;


float jueduizhi(float d)
{
    if(d < 0)
    {
        d = 0 - d ;
    }
    return d ;
}

float fuzhi(float d)
{
    if(d > 0)
    {
        d = 0 - d ;
    }
    return d ;
}

//ee ：电堆温度 - 目标温度
//0 - 5 - 7A 阶段
void fuzzy_PID(float ee)
{
//		MessageDetail  SendCANDATA ;
		//环境温度在25℃左右的情况
	  //温差
		float tmp = 0 - ee;
	
	  // 50
		if(g_FCMS_runpara.mF_A.flaotword >= 40)
		{
			/*
				if(tmp > 3)
				{
						pid.kp = 5 ;  //5 , 7 , 10
						pid.ki = 0.3 ;//0.2   , 0.3
						pid.kd = 1.5 ;//1.5 	 , 2.5	
				}
				else
				{
						pid.kp = 4 ;  //5 , 7 , 10
						pid.ki = 0.3 ;//0.2   , 0.3
						pid.kd = 1.5 ;//1.5 	 , 2.5							
				}
				*/
				pid.kp = 7 ;  //5 , 7 , 10
				pid.ki = 0.3 ;//0.2   , 0.3
				pid.kd = 1.5 ;//1.5 	 , 2.5	
		}
		else if(g_FCMS_runpara.mF_A.flaotword >= 20)  //30
		{
				//9 - 20
				if(tmp > 9)
				{
						// 0.3 ， 0 ， 0
						pid.kp = 0.5 ;
						pid.ki = 0.1 ;
						pid.kd = 0 ;
				}
				else
				{
						pid.kp = 3 ;  //2.8
						pid.ki = 0.2 ;//0.2
						pid.kd = 1.5 ;
				}
		} //温度达到临界点，温度迅速上升
			//室温： 8° ：28 
			//       15°：15
		else if(g_FCMS_runpara.mF_A.flaotword >= 15)
		{
				if(tmp > 8)
				{
						// 0.3 ， 0 ， 0
						pid.kp = 0.5 ;
						pid.ki = 0.1 ;
						pid.kd = 0 ;
				}
				else
				{
						//环境温度15°
						//2.0:30A，2.6°，s   40A，1.5°，s
						//2.8:30A, 1.8°，s   40A, 1.8°，s  50A，0.5°，s  60A-70A,0.5°
						//3.5:30A, 1.6°，78s   40A, 0.6°,30s
					  //4.0:30A, 1.2°，58s   40A, 0.3°,35s
					  pid.kp = 4 ;  
						pid.ki = 0.2  ;     
						pid.kd = 1 ;						
				}
		}
		else
		{
				//需要升温
				if(tmp >= 0)
				{
						pid.kp = 0.5 ; // 3   10的时候，降得太慢
						pid.ki = 0.1 ;
						pid.kd = 0 ;
				}
				else //需要降温
				{
						pid.kp = 0.5 ; 
						pid.ki = 0 ;
						pid.kd = 0 ;						
				}
		}

//		SendCANDATA.LEN = 8 ;	   //该域包含了当前接收到的信息的数据长度代码
//		SendCANDATA.RTR = 0 ;	   //0 表明接收到一个数据帧
//		SendCANDATA.FF  = 1 ;    //是否是标准帧		
//		
//		SendCANDATA.ID.Address   =  0x10220102 ;				
//		SendCANDATA.D[0].DATA[0] =  fc_sdata.Bits.kp ;	  
//		SendCANDATA.D[0].DATA[1] =  fc_sdata.Bits.ki ;	   
//		SendCANDATA.D[0].DATA[2] =  fc_sdata.Bits.kd ;	     								
//		SendCANDATA.D[0].DATA[3] =  fc_sdata.Bits.kT ;     
//		SendCANDATA.D[1].word 	 = 0xFFFFFFFF;				
//		CAN_SEND(0 , SendCANDATA);

//		DelayMS(5); 
//		SendCANDATA.ID.Address = 0x11111122;	
//		SendCANDATA.D[0].floatword =  tmp ;    
//		SendCANDATA.D[1].floatword =  0 ; 
//			
//		CAN_SEND(0 , SendCANDATA);	
}

/******************************************
*								计算增量
*1、对控制量的增量进行限幅
*******************************************/
float pid_calc(PID *pidLocal)  
{  
    float out;  
    float ep, ei, ed;  
      
		//风扇是散热，用电堆温度 - 目标温度
    pidLocal->e_0 = pidLocal->feedback - pidLocal->target;

		//计算pID
		fuzzy_PID(pidLocal->e_0) ;
	
    ep = pidLocal->e_0  - pidLocal->e_1;  
    ei = pidLocal->e_0;  
    ed = pidLocal->e_0 - 2*pidLocal->e_1 + pidLocal->e_2;  	

	  out = pidLocal->kp * ep + pidLocal->ki * ei + pidLocal->kd * ed; 
		
    pidLocal->e_2 = pidLocal->e_1;  
    pidLocal->e_1 = pidLocal->e_0;  
//		printf("ee:%f,%f,%f ,%f\n" , ep, ei , ed , out);
//		printf("pid:%f,%f,%f\n" , pidLocal->kp, pidLocal->ki , pidLocal->kd);
		
//		SendCANDATA.LEN = 8 ;	   //该域包含了当前接收到的信息的数据长度代码
//		SendCANDATA.RTR = 0 ;	   //0 表明接收到一个数据帧
//		SendCANDATA.FF  = 1 ;    //是否是标准帧
//		DelayMS(5);
//		SendCANDATA.ID.Address = 0x11111110;	
//		
//		SendCANDATA.D[0].floatword =  ep ;    
//		SendCANDATA.D[1].floatword =  ei ; 
//			
//		CAN_SEND(0 , SendCANDATA);
//		DelayMS(10);

//		SendCANDATA.ID.Address = 0x11111111;	
//		
//		SendCANDATA.D[0].floatword =  ed ;    
//		SendCANDATA.D[1].floatword =  out ; 
//			
//		CAN_SEND(0 , SendCANDATA);	
	
    return out;  
}  

/******************************************
*1、创建一个周期任务
*2、通过调用pid_calc()获得控制量的增量，
*3、对控制量进行限幅
*******************************************/
float pid_control(void)
{
		float inc_val = 0 ;        //增量
		
	  pid.feedback = g_FCMS_runpara.mF_CKWD.flaotword ;
		pid.target   = g_FCMS_runpara.mF_MBWD.flaotword ;
	
		inc_val = pid_calc(&pid);
//		printf("inc_val=%lf\n" , inc_val);
		//对控制量进行限幅
		//inc_val = range(inc_val, MIN_LIMIT, MAX_LIMIT); 
		if(inc_val <= -5)
		{
				inc_val = 0 ;
		}
		if(inc_val >= 5)
		{
				inc_val = 0 ;
		}
		return inc_val ;
}

//风扇最低转速计算
uint8_t Fan_Lowest_Calc(void)
{
		uint8_t speed ;
		speed = g_FCMS_runpara.mF_A.flaotword * FLASHMEMORYMAP->Bits.floatBites.Fan_Lowerest_0 + FLASHMEMORYMAP->Bits.floatBites.Fan_Lowerest_1 ;
		return speed ;
}
//累计电流计算
//1s累加一次
void A_Inter_Calc(void)
{
		static float A_Inter = 0 ;
		static uint8_t times = 0 ;
		g_FCMS_runpara.mF_A.flaotword = 10 ;
	
		A_Inter += g_FCMS_runpara.mF_A.flaotword ;  
		times++ ;
		AM_DBG_INFO("%d,%f\n" , FLASHMEMORYMAP->Bits.FCrunBites.A_Integral,A_Inter);
		if(A_Inter >= FLASHMEMORYMAP->Bits.FCrunBites.A_Integral || times >= 120)
		{
				//排气
				PaiQiSelfComplex(300) ; //FLASHMEMORYMAP->Bits.FCrunBites.PaiH_times
				A_Inter = 0 ;
				times = 0 ;
		}
}

//多长时间计算一次 ？
//计算控制目标温度
void FT_Target_Calc(void)
{
		float FA_Now = g_FCMS_runpara.mF_A.flaotword ;
		g_FCMS_runpara.mF_MBWD.flaotword = FA_Now * FA_Now * FLASHMEMORYMAP->Bits.floatBites.Fan_Set_0 + FA_Now * FLASHMEMORYMAP->Bits.floatBites.Fan_Set_1 + FLASHMEMORYMAP->Bits.floatBites.Fan_Set_2 ;
//		printf("MBWD:%f" , g_FCMS_runpara.mF_MBWD.flaotword);
}

void FC_FJ_calc(void)
{
		float result = 0 ;
		static float temp = 10 ;
		//PID计算 	
		if(1 == __g_pid_timer)
		{
				__g_pid_timer = 0 ;
				result = pid_control();
					
				temp = temp + result ;

				g_FCMS_runpara.mF_FJ = temp ;

				//风扇最大值
				if(g_FCMS_runpara.mF_FJ >= 100)
				{
						g_FCMS_runpara.mF_FJ = 100 ;
				}
				
				//风扇最小转速
				if(g_FCMS_runpara.mF_FJ < (FLASHMEMORYMAP->Bits.floatBites.Fan_Lowerest_0 * g_FCMS_runpara.mF_A.flaotword 
									+ FLASHMEMORYMAP->Bits.floatBites.Fan_Lowerest_1))
				{
						g_FCMS_runpara.mF_FJ = FLASHMEMORYMAP->Bits.floatBites.Fan_Lowerest_0 * g_FCMS_runpara.mF_A.flaotword
									+ FLASHMEMORYMAP->Bits.floatBites.Fan_Lowerest_1 ;
						temp = g_FCMS_runpara.mF_FJ ;
				}
				
				//电堆温度保护
				if(g_FCMS_runpara.mF_CKWD.flaotword >= g_FCMS_runpara.mF_MBWD.flaotword + 2)
				{
						FC_FJControl(g_FCMS_runpara.mF_FJ + 15);
				}
				else  
				{
						FC_FJControl(g_FCMS_runpara.mF_FJ);
				}
			}
}

void FC_Control_Task(void)
{
		if(1 == __g_A_Inter_timer)
		{
				A_Inter_Calc() ;	 //放在回调函数里,IO没有响应
				__g_A_Inter_timer = 0 ;
		}
	
		if(2 == g_FCMS_system.msysControlMode)
		{	
			return ;
		}
		FC_FJ_calc();		
}

static void __fcControl_timer_callback(void *p_arg)
{
		__g_pid_timer     = 1 ;	
		__g_A_Inter_timer = 1 ;
}

void fc_control_timer_start(void)
{
		am_softimer_start(&__g_ControlTimer , 1000);
}

void fc_control_timer_stop(void)
{
		am_softimer_stop(&__g_ControlTimer);
}

void pid_Init(void)
{
		pid.e_0 = g_FCMS_runpara.mF_CKWD.flaotword ;
		pid.e_1 = g_FCMS_runpara.mF_CKWD.flaotword ;
		pid.e_2 = g_FCMS_runpara.mF_CKWD.flaotword ;
	
		pid.kt = FLASHMEMORYMAP->Bits.FCrunBites.kT ;
		pid.kp = FLASHMEMORYMAP->Bits.FCrunBites.kp / 10.0;
		pid.ki = FLASHMEMORYMAP->Bits.FCrunBites.ki / 10.0;
		pid.kd = FLASHMEMORYMAP->Bits.FCrunBites.kd / 10.0;		
	
		am_softimer_init(&__g_ControlTimer , __fcControl_timer_callback , NULL);
		fc_control_timer_start();
}

/******************************************************************************
**                            End Of File
******************************************************************************/
