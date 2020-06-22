
/****************************************Copyright (c)****************************************************
**														Changxing  GCLN  Development Co., LTD
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2017-01-04
** Last Version:        V1.0
** Descriptions:        Add ucosⅡ system.
**
**--------------------------------------------------------------------------------------------------------
** Created by:          yuteng
** Created date:        2017-01-04
** Version:             V1.00
** Descriptions:        Add breathing LED.
**
**--------------------------------------------------------------------------------------------------------
** Created by:          yuteng
** Created date:        2017-01-05
** Version:             V1.00
** Descriptions:        The system is divided into three layers.
**
*********************************************************************************************************/
#include "LPC17xx.h"
#include "includes.h"
#include "ucosTask.h"
#include "ioControl.h"
#include "pwmHard.h"

#define Debug 


int main(void)
{
		uint8  os_err; 
		SystemInit();
		pinInit();
		
		FAMEN_1_Control(0);
		FAMEN_2_Control(0);
		FAMEN_3_Control(0);
		FAMEN_4_Control(0);
		FAMEN_5_Control(0);
//		beep_Control(0);	
		PWM_Set_5(1 , 0) ;
		PWM_Set_6(1 , 0) ;
	
		OSInit();
		OS_CPU_SysTickInit();
	
		/* Create the start task.*/
		os_err = OSTaskCreateExt((void (*)(void *)) App_Task_Main,                                 	//函数指针
															 (void          * ) 0,                                          	//参数指针
															 (OS_STK        * )&App_TaskMAINStk[APP_TASK_MAIN_STK_SIZE - 1],  //任务堆栈栈顶指针
															 (uint8           ) APP_TASK_MAIN_PRIO,                          	//任务优先级
															 (uint16          ) APP_TASK_MAIN_PRIO,                          	//任务标识
															 (OS_STK        * )&App_TaskMAINStk[0],                         	//栈底指针
															 (uint32          ) APP_TASK_MAIN_STK_SIZE,                      	//任务堆栈大小
															 (void          * )0,                                           	//数据结构体指针，作为TCB的扩展
															 (uint16          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));	//任务设定选项

		 os_err=os_err;

		 OSStart();                   
}


