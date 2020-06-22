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
#ifndef __FCCONTROL_H 
#define __FCCONTROL_H

//#include "system_LPC17xx.h"
#include "lpc17xx.h"


//#define max(a, b)           (a>b? a:b)  
//#define min(a, b)           (a<b? a:b)  

#define range(x, a, b)      (min(max(x, a), b))  
#define exchange(a, b, tmp) (tmp=a, a=b, b=tmp)  
#define myabs(x)            ((x<0)? -x:x)  

#define MIN_LIMIT   -5
#define MAX_LIMIT   5


/****************************************************************
 *	  									PID
 *****************************************************************/
typedef struct{  
		float kp ; 
		float ki ; 
		float kd ; 
		float kt ; 
    float limit_max;    //输出限幅
		float limit_mix;    //输出限幅
    float target;       //目标输出量  
    float feedback;     //实际输出量
	
    float e_0;          //当前误差
    float e_1;          //上一次误差
    float e_2;          //上上次误差
}PID;  


extern void FC_Control_Task(void);
extern void pid_Init(void) ;
extern void fc_control_timer_start(void) ;
extern void fc_control_timer_stop(void) ;

#endif 
/****************************************************************************
**                            End Of File
****************************************************************************/
