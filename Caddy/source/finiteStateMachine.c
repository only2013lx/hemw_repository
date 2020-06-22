
#include "finiteStateMachine.h"
#include "ioControl.h"
#include "pwmDriver.h"
#include "ucos_ii.h"
#include "ram.h"
#include "IAPFlashDriver.h"
#include "warn.h"
#include "timerDriver.h"
#include "canSendQueue.h"
#include "canTask.h"
#include "powerManage.h"
#include <stdio.h>
#include "valveControl.h"
#include "am_vdebug.h"


//static enum _T_mode HJ_Mode ;

//系统复位
void FCSystemSoftReset(void)
{
		j1939_message sendMessage ;
		sendMessage.channel 	     = CAN2 ;
		sendMessage.FF      			 = 1 ;
		sendMessage.LEN     			 = 2 ;		
		sendMessage.std_id.ID 		 = 0x8148c01 ;
		sendMessage.mCANDF.DATA[0] = CONTROL_SYSRESET;
		sendMessage.mCANDF.DATA[1] = 0x77;
		WriteCANSedCyBuf(CAN2 , &sendMessage);	
}

//系统开关机
void Trun_OnOff(uint8_t order)
{
		j1939_message sendMessage ;
		//if(1 == g_MCMS_system.msysOnOffOrder || 2 == g_MCMS_system.msysOnOffOrder)
		{
				sendMessage.channel 	     = CAN2 ;
				sendMessage.FF      			 = 1 ;
				sendMessage.LEN     			 = 2 ;		
				sendMessage.std_id.ID 		 = 0x8148c01 ;
				sendMessage.mCANDF.DATA[0] = CONTROL_ONOFF;
				sendMessage.mCANDF.DATA[1] = order;
				WriteCANSedCyBuf(CAN2 , &sendMessage);	
//				sendMessage.std_id.ID 		 = 0x8148d01 ;
//				WriteCANSedCyBuf(CAN2 , &sendMessage);	
				g_MCMS_system.msysOnOffOrder = 0 ;
		}
}

//系统状态切换
void sysModeChange(uint8_t mode)
{
		g_MCMS_system.msysState = mode ;
}

/*
*   1、检测传感器是否连接状态
*   2、检测基本的告警数据
*/
void sys_Warn_Check(uint32_t warnData)
{	
		g_MCMS_system.msysWarn.word = (warnData & fcWarnHandle.sysWarn.word) ;
//	printf("%x , %x\n" , g_MCMS_system.msysWarn.word , fcWarnHandle.sysWarn.word);
//		if(g_MCMS_system.msysWarn.word)  //告警，则进入关机状态6++
//				printf("Warning.....\n");
//				sysModeChange(5);
//				return ;
}

int HJ_Control(void)
{
		//等待环境控制完成
		//开启加热器 , 目标 电堆温度  达到  5° 
		printf("warming:%f\n" , g_MCMS_runpara.mF_HJWD.flaotword);
		if(g_MCMS_runpara.mF_HJWD.flaotword > HJWD_MODE_LOW) //加热到零度之上
		{
				return 1 ;
		}
		else 
		{
				return 0 ;
		}
}

void HJ_judge(void)
{
			//环境温度控制
		if(g_MCMS_runpara.mF_HJWD.flaotword > HJWD_MODE_HIGH)
		{
				AM_DBG_INFO("HIGH_T_MOde\n");
				g_MCMS_Power.mTempMode = T_High_mode ;  //高温模式
		} 
		else if(g_MCMS_runpara.mF_HJWD.flaotword < HJWD_MODE_LOW)  //-1
		{
				AM_DBG_INFO("LOw_T_MOde\n");
			  g_MCMS_Power.mTempMode = T_Low_mode ;  //低温模式
		}
		else
		{
				AM_DBG_INFO("Normal_T_MOde\n");
				g_MCMS_Power.mTempMode = T_Normal_mode ;  //常温模式
		}
}
/**********************************************
*								状态机任务
*							0 ：开机检测
*   1、检测告警
*   2、等待12s  , 才进入下一状态
***********************************************/
void stateSysSelfInspection(void)
{
		socRate_timer_start();         //启动soc消耗率计算
		if(g_FCMS_BMSpara.mBMS_SOC < 5)
		{
				OSTimeDlyHMSM(0, 0, 5, 0); 
		}
		else
		{
			  OSTimeDlyHMSM(0, 0, 3, 0); 
		}

		sys_Warn_Check(WARN_STATESYSSELFINSPECTION) ;
		
		HJ_judge(); //初始环境判断
		
		if(T_Low_mode == g_MCMS_Power.mTempMode)
		{
				if(g_FCMS_BMSpara.mBMS_SOC < LOW_T_MODE_SOC)
				{
						sysModeChange(5);	  //禁止开机
				}
				else
				{
						sysModeChange(1);				
				}
		}
		else
		{
				sysModeChange(1);				
		}

//		if(1 == g_FCMS_system[0].msysState || 1 == g_FCMS_system[1].msysState)
//		{
//				sysModeChange(1);				
//		}
//		
//		if(3 == g_FCMS_system[0].msysState || 3 == g_FCMS_system[1].msysState)
//		{
//				sysModeChange(1);				
//		}		
//		
//		if(5 == g_FCMS_system[0].msysState || 5 == g_FCMS_system[1].msysState)
//		{
//				sysModeChange(5);				
//		}
}

/**********************************************
*								状态机任务
*				  1 ：自检完成，等待开机
*		1、等待开机信号
*		2、检测告警
***********************************************/
void stateSysWaitStart(void)
{
//		printf("\n\t  stateSysWaitStart\n\t");
		sys_Warn_Check(WARN_STATESYSWAITSTART);

		HJ_judge(); //初始环境判断
	
		if(1 == g_MCMS_system.msysOnOffOrder) //等待开机命令
		{
				if(T_Low_mode == g_MCMS_Power.mTempMode) //低温
				{
						PC_Control_Relay(1);
						MCalorifierSpeedSet(1);
						sysModeChange(2);
//						//SOC过低，关闭系统
//						if(g_FCMS_BMSpara.mBMS_SOC >= LOW_T_MODE_SOC)
//						{
//								if(1 == HJ_Control()) //等待环境控制完成
//								{
//										Trun_OnOff(g_MCMS_system.msysOnOffOrder); //发送开机命令	
//										sysModeChange(2);
//								}
//						}
//						else
//						{
//								MasterContactor(0);
//								MCalorifierSpeedSet(0);
//								sysModeChange(4);
//						}
				}
				else //正常 - 高温
				{
						//满足条件： 切换至 开机中
						if(g_FCMS_BMSpara.mBMS_SOC > DISCHARGE_SOC)  //放电
						{
								AM_DBG_INFO("discharge\n");
								//打开主接触器，放电  
								MasterContactor(1);
								PC_Control_Relay(1); 
								sysModeChange(2);
						}
						else		//正常 ， 充电    if(g_FCMS_BMSpara.mBMS_SOC < 30) 
						{
								AM_DBG_INFO("charge\n");
								//发送开机命令
								sysModeChange(2);
								PC_Control_Relay(1); 
								Trun_OnOff(g_MCMS_system.msysOnOffOrder); //发送开机命令			
						}
				}
		}
		//燃料电池系统启动中
//		if(2 == g_FCMS_system[0].msysState || 2 == g_FCMS_system[1].msysState )  
//		{
//				sysModeChange(2);
//		}
//		
//		if(3 == g_FCMS_system[0].msysState || 3 == g_FCMS_system[1].msysState)
//		{
//				sysModeChange(2);
//		}		
}
/**********************************************
*									状态机任务
*							2	：启动中。。。 
*		1、判断是否开机完成
*		2、检测告警
***********************************************/
void stateSysStarting(void)
{
		static int flag = 1 ;
		sys_Warn_Check(WARN_STATESYSSTARTING);
//		OSTimeDlyHMSM(0, 0, 3, 0);   //等待3s，运行完成
//		sys_Warn_Check(WARN_STATESYSSTARTING);
	  
		if(T_Low_mode == g_MCMS_Power.mTempMode)
		{
				if(1 == HJ_Control() && (1 == flag)) //等待环境控制完成
				{
						flag = 0 ;
						Trun_OnOff(g_MCMS_system.msysOnOffOrder); //发送开机命令	
				}
			
				if(3 == g_FCMS_system[0].msysState)
				{
							MasterContactor(1);
							sysModeChange(3);								
				}
				else if(5 == g_FCMS_system[0].msysState)
				{
						//关机
						Trun_OnOff(2);
						sysModeChange(4);	
				}
		}
		else
		{
				if(g_FCMS_BMSpara.mBMS_SOC >= CHARGE_SOC && g_FCMS_BMSpara.mBMS_SOC <= DISCHARGE_SOC)
				{	
						//正常运行   
						if(1 == g_FCMS_system[0].msysState)
						{
								AM_DBG_INFO("send start - 1\n");
								Trun_OnOff(1); //发送开机命令	
						}
						else if(3 == g_FCMS_system[0].msysState)
						{
								MasterContactor(1);
								PC_Control_Relay(1); 
								sysModeChange(3);		
						}
						else if((4 == g_FCMS_system[0].msysState) || (5 == g_FCMS_system[0].msysState))
						{
								AM_DBG_INFO("send stop - 1\n");
								Trun_OnOff(2); //发送开机命令	
								sysModeChange(4);		
						}
				}
		}
		
		//DCDC缓加载
		if(3 == g_FCMS_system[0].msysState)
			{
					if(flag)
					{
							if(1 == flag)
							{
									flag = 2; 
									OSTimeDlyHMSM(0, 0, 40, 0);     //等待60s , 等待DCDC稳定
							}
							
							if(g_DCMS_runpara[2].mD_P < 1000)
							{
									g_DCMS_setpara.mD_V = 28.6 ;
									DCDCSetPower(50);
									OSTimeDlyHMSM(0, 0, 0, 1000);		
							}
							else
							{
									flag = 0 ; 
							}
					}
			}		
		
		//系统关机
		if(2 == g_MCMS_system.msysOnOffOrder || (g_FCMS_system[0].msysWarn.word & 0xfff000ff))
		{
				//关机
				AM_DBG_INFO("send stop - 1-1\n");
				Trun_OnOff(2); 
				sysModeChange(4);		
				MasterContactor(0);
				PC_Control_Relay(0); 								
		}
		
		
/*	
		if(3 == g_FCMS_system[0].msysState || 3 == g_FCMS_system[1].msysState)
		{
			  PC_Control_Relay(1) ;   //打开锂电池输出继电器
				OSTimeDlyHMSM(0, 0, 5, 0);
				MasterContactor(1);      //开启主接触器
				OSTimeDlyHMSM(0, 0, 2, 0);
				sysModeChange(3);
		}
		if(4 == g_FCMS_system[0].msysState || 4 == g_FCMS_system[1].msysState)
		{
				sysModeChange(4);
		}		
		if(5 == g_FCMS_system[0].msysState || 5 == g_FCMS_system[1].msysState)
		{ 
				sysModeChange(5);
		}				
*/
}
/**********************************************
*									状态机任务
*						3	：系统正常运行中。。。
*   1、监测所有的报警
*		2、等待关机
***********************************************/
void stateSysRuning(void)
{	
		if(3 == g_FCMS_system[0].msysState || 3 == g_FCMS_system[1].msysState)
		{
				sys_Warn_Check(WARN_STATESYSRUNING);			
		}
		else
		{
				sys_Warn_Check(WARN_STATESYSRUNING);	
		}		

		if(T_Normal_mode == g_MCMS_Power.mTempMode)
		{
				if(g_FCMS_BMSpara.mBMS_SOC > CLOSE_FC_SOC)  //关闭FC
				{
						//发送关机命令
						Trun_OnOff(2);
						AM_DBG_INFO("send stop - 2\n");
				}
				else if(g_FCMS_BMSpara.mBMS_SOC < OPEN_FC_SOC) //打开FC
				{
						if(g_FCMS_system[0].msysWarn.word & 0xfff000ff)
						{
								; //关机告警
						}
						else
						{
								if(5 == g_FCMS_system[0].msysState)  
								{
										//发送 复位，开机命令
										PC_Control_Resrt_FC();
										AM_DBG_INFO("send reset\n");
								}
								else if(1 == g_FCMS_system[0].msysState)   
								{
										//发送开机命令
										Trun_OnOff(1) ;		 
										AM_DBG_INFO("send start - 2\n");
								}
						}
				}
		}
		else if(T_Low_mode == g_MCMS_Power.mTempMode)
		{
				//低温模式  降额
				if(g_FCMS_runpara[0].mF_V.flaotword * g_FCMS_runpara[0].mF_A.flaotword >= 1500)
				{
						MCalorifierSpeedSet(0);  //关闭加热
				}
		}
		else if(T_High_mode == g_MCMS_Power.mTempMode)
		{
				//高温模式  降额
		}

		//能量管理     //电堆输出, 才有能量管理
		if(3 == g_FCMS_system[0].msysState)
		{
				//高、低温降额在此实现
//				powerMange();				
		}		
		
		//关机
		if(2 == g_MCMS_system.msysOnOffOrder || (g_FCMS_system[0].msysWarn.word & 0xfff000ff)) // || g_FCMS_system[0].msysState >= 4 || g_FCMS_system[1].msysState >= 4
		{
				MasterContactor(0);    //关闭主接触器
				if(T_Low_mode == g_MCMS_Power.mTempMode)
				{
						//打开加热器
						MCalorifierSpeedSet(1);
				}
				DCDCSetCAN(DCSET_OVER_V ,  286 , 90);  //设置放电量
				OSTimeDlyHMSM(0, 0, 3, 0);				
				Trun_OnOff(2) ;					
				sysModeChange(4);
		}	
/*		
		if(g_FCMS_system[0].msysState >= 4 || g_FCMS_system[1].msysState >= 4)
		{
				MasterContactor(0);    //关闭主接触器
				CirclePumpControl(0);  //断开循环泵
			  Trun_OnOff(2) ;	
				sysModeChange(4);
		}
*/
}
/**********************************************
*								状态机任务
*							4	：关机中。。。
*		1、判断是否进入【关机完成状态】
***********************************************/
void stateSysStoping(void)
{
		//6A ,放电 
		if(4 == g_FCMS_system[0].msysState)
		{
				DCDCSetPower(300); 
		}
		
		if(5 == g_FCMS_system[0].msysState /*&& 5 == g_FCMS_system[1].msysState*/)
		{
				socRate_timer_stop();
				PC_Control_Relay(2);  //关闭 次 接触器
				sysModeChange(5);
			
				if(T_Low_mode == g_MCMS_Power.mTempMode)
				{
						//关闭加热器
						MCalorifierSpeedSet(0);
				}
		}
		else if(1 == g_FCMS_system[0].msysState)
		{
				socRate_timer_stop();
				PC_Control_Relay(2);  //关闭 次 接触器
				sysModeChange(5);				
		}
}

/**********************************************
*								状态机任务
*						5	：关机完成，系统锁定
*   1、系统进入死锁状态 ， 等待系统复位
***********************************************/
void stateSysStoped(void)
{
//		printf("\n\t stateSysStoped\n\t");
//		FC_FJControl(0);				//关闭风扇
}

/**********************************************
*									模式切换
*			mode :切换的模式
***********************************************/
//建立状态机任务
const finiteStateMachine  stateMachineTable[] =
{
   	{0 ,(*stateSysSelfInspection)},     //开机自检
   	{1 ,(*stateSysWaitStart)},          //等待开机
		{2 ,(*stateSysStarting)},           //正在开机
   	{3 ,(*stateSysRuning)},             //正在运行
		{4 ,(*stateSysStoping)},            //正在关机
   	{5 ,(*stateSysStoped)}              //关机完成
};

void finiteStateMachineTask(void)
{
		void (*current_operation_index)(void) = NULL ;
		
		//状态机任务	
		current_operation_index = stateMachineTable[g_MCMS_system.msysState].current_operation ;
		(*current_operation_index)();
}
/*********************************************************************************************************
																				End Of File
*********************************************************************************************************/
