
/****************************************Copyright (c)****************************************************
**														Changxing  GCLN  Development Co., LTD
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2017-01-04
** Last Version:        V1.0
** Descriptions:        Add ucos�� system.
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
		os_err = OSTaskCreateExt((void (*)(void *)) App_Task_Main,                                 	//����ָ��
															 (void          * ) 0,                                          	//����ָ��
															 (OS_STK        * )&App_TaskMAINStk[APP_TASK_MAIN_STK_SIZE - 1],  //�����ջջ��ָ��
															 (uint8           ) APP_TASK_MAIN_PRIO,                          	//�������ȼ�
															 (uint16          ) APP_TASK_MAIN_PRIO,                          	//�����ʶ
															 (OS_STK        * )&App_TaskMAINStk[0],                         	//ջ��ָ��
															 (uint32          ) APP_TASK_MAIN_STK_SIZE,                      	//�����ջ��С
															 (void          * )0,                                           	//���ݽṹ��ָ�룬��ΪTCB����չ
															 (uint16          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));	//�����趨ѡ��

		 os_err=os_err;

		 OSStart();                   
}


