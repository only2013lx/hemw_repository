
#ifndef  UCOSTASK_H
#define  UCOSTASK_H

#include "includes.h"

/**********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
**********************************************************************************************************/
// 1、任务函数
// 2、任务优先级
// 3、任务堆栈
// 4、任务堆栈大小
/**********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
**********************************************************************************************************/

extern  OS_STK  App_TaskLEDStk[APP_TASK_LED_STK_SIZE];

extern  OS_STK  App_Task1Stk[APP_TASK_1_STK_SIZE];
extern  OS_STK  App_Task2Stk[APP_TASK_2_STK_SIZE];
extern  OS_STK  App_Task3Stk[APP_TASK_3_STK_SIZE];
extern  OS_STK  App_Task4Stk[APP_TASK_4_STK_SIZE];
extern  OS_STK  App_Task5Stk[APP_TASK_5_STK_SIZE];
extern  OS_STK  App_Task6Stk[APP_TASK_6_STK_SIZE];

extern  OS_STK  App_TaskMAINStk[APP_TASK_MAIN_STK_SIZE];


/*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************/

extern  void   App_Task_Main(void *p_arg);

static void App_Task_1(void *p_arg);      // IO设置
static void App_Task_2(void *p_arg);      //
static void App_Task_3(void *p_arg);
static void App_Task_4(void *p_arg);
static void App_Task_5(void *p_arg);
static void App_Task_6(void *p_arg);
static void App_Task_LED (void *p_arg);
extern void App_Init(void) ;

	
#endif

