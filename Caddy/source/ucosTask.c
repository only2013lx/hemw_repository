
//#include "LPC17xx.h"
#include "ucosTask.h"
//#include "ioControl.h"
#include "uartDriver.h"
#include "adcDriver.h"
#include "pwmDriver.h"
#include "timerDriver.h"

#include "EEPROMDriver.h"
#include "canTask.h"
#include "j1939Stack.h"
#include "canSendQueue.h"
#include "IAPFlashDriver.h"
#include "ram.h"
//#include "IAPFlashDriver.h"
#include "finiteStateMachine.h"
#include "warn.h"
#include "fcControl.h"
//#include "stdio.h"
//#include "text.h"


#include "am_lpc17xx_int.h"
#include "am_softimer.h"
//#include "am_wait.h"
#include "am_vdebug.h"
#include "adcTask.h"

#include "powerManage.h"


//8字节对齐
__align(8) OS_STK         App_TaskMAINStk[APP_TASK_MAIN_STK_SIZE];
__align(8) OS_STK         App_Task1Stk[APP_TASK_1_STK_SIZE];
__align(8) OS_STK         App_Task2Stk[APP_TASK_2_STK_SIZE];
__align(8) OS_STK         App_Task3Stk[APP_TASK_3_STK_SIZE];
//__align(8) OS_STK         App_Task4Stk[APP_TASK_4_STK_SIZE];
//__align(8) OS_STK         App_Task5Stk[APP_TASK_5_STK_SIZE];


static void StartTaskAll(void)
{

		#if 1 == Start_OSStatInit		   //开始统计CPU使用率
    OSStatInit();                /* Create the Statistic Task                */
    #endif
		/*
		OSTaskCreateExt((void (*)(void *)) App_Task_LED,                                 
									 (void          * ) 0,                                         
									 (OS_STK        * )&App_TaskLEDStk[APP_TASK_LED_STK_SIZE - 1], 
									 (uint8           ) APP_TASK_LED_PRIO,                         
									 (uint16          ) APP_TASK_LED_PRIO,                         
									 (OS_STK        * )&App_TaskLEDStk[0],                         
									 (uint32          ) APP_TASK_LED_STK_SIZE,                     
									 (void          * )0,                                          
									 (uint16          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
	*/
		OSTaskCreateExt((void (*)(void *)) App_Task_1,                            
									 (void          * ) 0,                                      
									 (OS_STK        * )&App_Task1Stk[APP_TASK_1_STK_SIZE - 1],  
									 (uint8           ) APP_TASK_1_PRIO,                        
									 (uint16          ) APP_TASK_1_PRIO,                        
									 (OS_STK        * )&App_Task1Stk[0],                        
									 (uint32          ) APP_TASK_1_STK_SIZE,                    
									 (void          * )0,                                       
									 (uint16          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

		OSTaskCreateExt((void (*)(void *)) App_Task_2,                            
									 (void          * ) 0,                                      
									 (OS_STK        * )&App_Task2Stk[APP_TASK_2_STK_SIZE - 1],  
									 (uint8           ) APP_TASK_2_PRIO,                        
									 (uint16          ) APP_TASK_2_PRIO,                        
									 (OS_STK        * )&App_Task2Stk[0],                        
									 (uint32          ) APP_TASK_2_STK_SIZE,                    
									 (void          * )0,                                       
									 (uint16          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

		OSTaskCreateExt((void (*)(void *)) App_Task_3,                            
									 (void          * ) 0,                                      
									 (OS_STK        * )&App_Task3Stk[APP_TASK_3_STK_SIZE - 1],  
									 (uint8           ) APP_TASK_3_PRIO,                        
									 (uint16          ) APP_TASK_3_PRIO,                        
									 (OS_STK        * )&App_Task3Stk[0],                        
									 (uint32          ) APP_TASK_3_STK_SIZE,                    
									 (void          * )0,                                       
									 (uint16          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
/*
		OSTaskCreateExt((void (*)(void *)) App_Task_4,                           
									 (void          * ) 0,                                     
									 (OS_STK        * )&App_Task4Stk[APP_TASK_4_STK_SIZE - 1], 
									 (uint8           ) APP_TASK_4_PRIO,                       
									 (uint16          ) APP_TASK_4_PRIO,                       
									 (OS_STK        * )&App_Task4Stk[0],                       
									 (uint32          ) APP_TASK_4_STK_SIZE,                   
									 (void          * )0,                                      
									 (uint16          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

		OSTaskCreateExt((void (*)(void *)) App_Task_5,                           
									 (void          * ) 0,                                     
									 (OS_STK        * )&App_Task5Stk[APP_TASK_5_STK_SIZE - 1], 
									 (uint8           ) APP_TASK_5_PRIO,                       
									 (uint16          ) APP_TASK_5_PRIO,                       
									 (OS_STK        * )&App_Task5Stk[0],                       
									 (uint32          ) APP_TASK_5_STK_SIZE,                   
									 (void          * )0,                                      
									 (uint16          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
									 */
}

void App_Init(void)
{
		int old;
		old = am_int_cpu_lock();
	
		canTaskInit();
		ioControlInit() ;
		uartInit() ;
	
		Timer0_Init_Driver() ;
		pwmInit_Driver(0) ;
		ADC_Init() ;
//		i2c2Init(100000);	
		am_softimer_init_zyt(); 
		EEROM_Init_Driver() ;
	
		pwmStart_Driver() ;
		
		j1939_init() ;
		
		CANSedCyInit() ;
		FlashDataInit() ;
		warn_Timer_Init();
		powerInit();
		
		am_int_cpu_unlock(old);
}	

void App_Task_Main(void *p_arg)
{
		(void)p_arg;
		App_Init();
		
		AM_DBG_INFO("Into Main\n");

		StartTaskAll();
		while(1)
		{ 
#if 1			
				canSeverHandle() ;    			//服务数据
				canProcessHandle()  ; 			//过程数据
				canInternetHandle() ; 			//网路数据			
				OSTimeDlyHMSM(0, 0, 0, 5);  //10ms 丢数据    8ms不丢数据
#else 
				AM_DBG_INFO("TAsk0\n");
				OSTimeDlyHMSM(0, 0, 0, 3000);
#endif			
		}
}

//IO控制
static void App_Task_1(void *p_arg)
{
    (void)p_arg;
		while(1)
		{
#if 1			
				ADC_Task() ;     //与can写缓存放在一起 
				canDataSend();		 
				OSTimeDlyHMSM(0, 0, 0, 300);
#else
				AM_DBG_INFO("TAsk1\n");
				OSTimeDlyHMSM(0, 0, 0, 500);
#endif			
		}
}

//IO控制
static void App_Task_2(void *p_arg)
{
    (void)p_arg;
		while(1)
		{
#if 1			
				finiteStateMachineTask() ;
				Warnning_Check_Task() ;				          	
				OSTimeDlyHMSM(0, 0, 0, 500);
#else
				AM_DBG_INFO("TAsk2\n");
				OSTimeDlyHMSM(0, 0, 0, 500);
#endif			
		}
}

//IO控制
static void App_Task_3(void *p_arg)
{
    (void)p_arg;
		while(1)
		{
#if 1			
				breathingLED();
//				OSTimeDlyHMSM(0, 0, 0, 500);
#else
				AM_DBG_INFO("TAsk3\n");
				OSTimeDlyHMSM(0, 0, 0, 500);
#endif			
		}
}

#if 0

//指令接收上传
static void App_Task_4(void *p_arg)
{
    (void)p_arg;
		while(1)
		{
#if 1			
				OSTimeDlyHMSM(0, 0, 0, 1000);
#else
				AM_DBG_INFO("TAsk4\n");
				OSTimeDlyHMSM(0, 0, 0, 500);
#endif			
		}
}

//呼吸灯  
static void App_Task_5(void *p_arg)
{
    (void)p_arg;
		while(1)
		{		
#if 1			
				
#else
				AM_DBG_INFO("TAsk5\n");
				OSTimeDlyHMSM(0, 0, 0, 500);
#endif			
		}
}



//ADC检测
static void App_Task_4(void *p_arg)
{
    (void)p_arg;
		while(1)
		{
//				finiteStateMachineTask() ;
//				Warnning_Check_Task() ;		
				OSTimeDlyHMSM(0, 0, 0, 100);
		}
}
//数据上传
static void App_Task_5(void *p_arg)
{
    (void)p_arg;
		while(1)
		{
//				breathingLED();
				OSTimeDlyHMSM(0, 0, 0, 160);
		}
}

//系统业务
static void App_Task_6(void *p_arg)
{
    (void)p_arg;
		while(1)
		{
				OSTimeDlyHMSM(0, 0, 0, 300);
		}
}

//系统运行时间、喂狗、CPU利用率
static void  App_Task_LED (void *p_arg)
{
		(void)p_arg;
		while(1)
		{	
				OSTimeDlyHMSM(0, 0, 0, 2000);
		}
}
#endif
