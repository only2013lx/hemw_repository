
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

//ϵͳ��λ
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

//ϵͳ���ػ�
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

//ϵͳ״̬�л�
void sysModeChange(uint8_t mode)
{
		g_MCMS_system.msysState = mode ;
}

/*
*   1����⴫�����Ƿ�����״̬
*   2���������ĸ澯����
*/
void sys_Warn_Check(uint32_t warnData)
{	
		g_MCMS_system.msysWarn.word = (warnData & fcWarnHandle.sysWarn.word) ;
//	printf("%x , %x\n" , g_MCMS_system.msysWarn.word , fcWarnHandle.sysWarn.word);
//		if(g_MCMS_system.msysWarn.word)  //�澯�������ػ�״̬6++
//				printf("Warning.....\n");
//				sysModeChange(5);
//				return ;
}

int HJ_Control(void)
{
		//�ȴ������������
		//���������� , Ŀ�� ����¶�  �ﵽ  5�� 
		printf("warming:%f\n" , g_MCMS_runpara.mF_HJWD.flaotword);
		if(g_MCMS_runpara.mF_HJWD.flaotword > HJWD_MODE_LOW) //���ȵ����֮��
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
			//�����¶ȿ���
		if(g_MCMS_runpara.mF_HJWD.flaotword > HJWD_MODE_HIGH)
		{
				AM_DBG_INFO("HIGH_T_MOde\n");
				g_MCMS_Power.mTempMode = T_High_mode ;  //����ģʽ
		} 
		else if(g_MCMS_runpara.mF_HJWD.flaotword < HJWD_MODE_LOW)  //-1
		{
				AM_DBG_INFO("LOw_T_MOde\n");
			  g_MCMS_Power.mTempMode = T_Low_mode ;  //����ģʽ
		}
		else
		{
				AM_DBG_INFO("Normal_T_MOde\n");
				g_MCMS_Power.mTempMode = T_Normal_mode ;  //����ģʽ
		}
}
/**********************************************
*								״̬������
*							0 ���������
*   1�����澯
*   2���ȴ�12s  , �Ž�����һ״̬
***********************************************/
void stateSysSelfInspection(void)
{
		socRate_timer_start();         //����soc�����ʼ���
		if(g_FCMS_BMSpara.mBMS_SOC < 5)
		{
				OSTimeDlyHMSM(0, 0, 5, 0); 
		}
		else
		{
			  OSTimeDlyHMSM(0, 0, 3, 0); 
		}

		sys_Warn_Check(WARN_STATESYSSELFINSPECTION) ;
		
		HJ_judge(); //��ʼ�����ж�
		
		if(T_Low_mode == g_MCMS_Power.mTempMode)
		{
				if(g_FCMS_BMSpara.mBMS_SOC < LOW_T_MODE_SOC)
				{
						sysModeChange(5);	  //��ֹ����
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
*								״̬������
*				  1 ���Լ���ɣ��ȴ�����
*		1���ȴ������ź�
*		2�����澯
***********************************************/
void stateSysWaitStart(void)
{
//		printf("\n\t  stateSysWaitStart�\\n\t");
		sys_Warn_Check(WARN_STATESYSWAITSTART);

		HJ_judge(); //��ʼ�����ж�
	
		if(1 == g_MCMS_system.msysOnOffOrder) //�ȴ���������
		{
				if(T_Low_mode == g_MCMS_Power.mTempMode) //����
				{
						PC_Control_Relay(1);
						MCalorifierSpeedSet(1);
						sysModeChange(2);
//						//SOC���ͣ��ر�ϵͳ
//						if(g_FCMS_BMSpara.mBMS_SOC >= LOW_T_MODE_SOC)
//						{
//								if(1 == HJ_Control()) //�ȴ������������
//								{
//										Trun_OnOff(g_MCMS_system.msysOnOffOrder); //���Ϳ�������	
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
				else //���� - ����
				{
						//���������� �л��� ������
						if(g_FCMS_BMSpara.mBMS_SOC > DISCHARGE_SOC)  //�ŵ�
						{
								AM_DBG_INFO("discharge\n");
								//�����Ӵ������ŵ�  
								MasterContactor(1);
								PC_Control_Relay(1); 
								sysModeChange(2);
						}
						else		//���� �� ���    if(g_FCMS_BMSpara.mBMS_SOC < 30) 
						{
								AM_DBG_INFO("charge\n");
								//���Ϳ�������
								sysModeChange(2);
								PC_Control_Relay(1); 
								Trun_OnOff(g_MCMS_system.msysOnOffOrder); //���Ϳ�������			
						}
				}
		}
		//ȼ�ϵ��ϵͳ������
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
*									״̬������
*							2	�������С����� 
*		1���ж��Ƿ񿪻����
*		2�����澯
***********************************************/
void stateSysStarting(void)
{
		static int flag = 1 ;
		sys_Warn_Check(WARN_STATESYSSTARTING);
//		OSTimeDlyHMSM(0, 0, 3, 0);   //�ȴ�3s���������
//		sys_Warn_Check(WARN_STATESYSSTARTING);
	  
		if(T_Low_mode == g_MCMS_Power.mTempMode)
		{
				if(1 == HJ_Control() && (1 == flag)) //�ȴ������������
				{
						flag = 0 ;
						Trun_OnOff(g_MCMS_system.msysOnOffOrder); //���Ϳ�������	
				}
			
				if(3 == g_FCMS_system[0].msysState)
				{
							MasterContactor(1);
							sysModeChange(3);								
				}
				else if(5 == g_FCMS_system[0].msysState)
				{
						//�ػ�
						Trun_OnOff(2);
						sysModeChange(4);	
				}
		}
		else
		{
				if(g_FCMS_BMSpara.mBMS_SOC >= CHARGE_SOC && g_FCMS_BMSpara.mBMS_SOC <= DISCHARGE_SOC)
				{	
						//��������   
						if(1 == g_FCMS_system[0].msysState)
						{
								AM_DBG_INFO("send start - 1\n");
								Trun_OnOff(1); //���Ϳ�������	
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
								Trun_OnOff(2); //���Ϳ�������	
								sysModeChange(4);		
						}
				}
		}
		
		//DCDC������
		if(3 == g_FCMS_system[0].msysState)
			{
					if(flag)
					{
							if(1 == flag)
							{
									flag = 2; 
									OSTimeDlyHMSM(0, 0, 40, 0);     //�ȴ�60s , �ȴ�DCDC�ȶ�
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
		
		//ϵͳ�ػ�
		if(2 == g_MCMS_system.msysOnOffOrder || (g_FCMS_system[0].msysWarn.word & 0xfff000ff))
		{
				//�ػ�
				AM_DBG_INFO("send stop - 1-1\n");
				Trun_OnOff(2); 
				sysModeChange(4);		
				MasterContactor(0);
				PC_Control_Relay(0); 								
		}
		
		
/*	
		if(3 == g_FCMS_system[0].msysState || 3 == g_FCMS_system[1].msysState)
		{
			  PC_Control_Relay(1) ;   //��﮵������̵���
				OSTimeDlyHMSM(0, 0, 5, 0);
				MasterContactor(1);      //�������Ӵ���
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
*									״̬������
*						3	��ϵͳ���������С�����
*   1��������еı���
*		2���ȴ��ػ�
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
				if(g_FCMS_BMSpara.mBMS_SOC > CLOSE_FC_SOC)  //�ر�FC
				{
						//���͹ػ�����
						Trun_OnOff(2);
						AM_DBG_INFO("send stop - 2\n");
				}
				else if(g_FCMS_BMSpara.mBMS_SOC < OPEN_FC_SOC) //��FC
				{
						if(g_FCMS_system[0].msysWarn.word & 0xfff000ff)
						{
								; //�ػ��澯
						}
						else
						{
								if(5 == g_FCMS_system[0].msysState)  
								{
										//���� ��λ����������
										PC_Control_Resrt_FC();
										AM_DBG_INFO("send reset\n");
								}
								else if(1 == g_FCMS_system[0].msysState)   
								{
										//���Ϳ�������
										Trun_OnOff(1) ;		 
										AM_DBG_INFO("send start - 2\n");
								}
						}
				}
		}
		else if(T_Low_mode == g_MCMS_Power.mTempMode)
		{
				//����ģʽ  ����
				if(g_FCMS_runpara[0].mF_V.flaotword * g_FCMS_runpara[0].mF_A.flaotword >= 1500)
				{
						MCalorifierSpeedSet(0);  //�رռ���
				}
		}
		else if(T_High_mode == g_MCMS_Power.mTempMode)
		{
				//����ģʽ  ����
		}

		//��������     //������, ������������
		if(3 == g_FCMS_system[0].msysState)
		{
				//�ߡ����½����ڴ�ʵ��
//				powerMange();				
		}		
		
		//�ػ�
		if(2 == g_MCMS_system.msysOnOffOrder || (g_FCMS_system[0].msysWarn.word & 0xfff000ff)) // || g_FCMS_system[0].msysState >= 4 || g_FCMS_system[1].msysState >= 4
		{
				MasterContactor(0);    //�ر����Ӵ���
				if(T_Low_mode == g_MCMS_Power.mTempMode)
				{
						//�򿪼�����
						MCalorifierSpeedSet(1);
				}
				DCDCSetCAN(DCSET_OVER_V ,  286 , 90);  //���÷ŵ���
				OSTimeDlyHMSM(0, 0, 3, 0);				
				Trun_OnOff(2) ;					
				sysModeChange(4);
		}	
/*		
		if(g_FCMS_system[0].msysState >= 4 || g_FCMS_system[1].msysState >= 4)
		{
				MasterContactor(0);    //�ر����Ӵ���
				CirclePumpControl(0);  //�Ͽ�ѭ����
			  Trun_OnOff(2) ;	
				sysModeChange(4);
		}
*/
}
/**********************************************
*								״̬������
*							4	���ػ��С�����
*		1���ж��Ƿ���롾�ػ����״̬��
***********************************************/
void stateSysStoping(void)
{
		//6A ,�ŵ� 
		if(4 == g_FCMS_system[0].msysState)
		{
				DCDCSetPower(300); 
		}
		
		if(5 == g_FCMS_system[0].msysState /*&& 5 == g_FCMS_system[1].msysState*/)
		{
				socRate_timer_stop();
				PC_Control_Relay(2);  //�ر� �� �Ӵ���
				sysModeChange(5);
			
				if(T_Low_mode == g_MCMS_Power.mTempMode)
				{
						//�رռ�����
						MCalorifierSpeedSet(0);
				}
		}
		else if(1 == g_FCMS_system[0].msysState)
		{
				socRate_timer_stop();
				PC_Control_Relay(2);  //�ر� �� �Ӵ���
				sysModeChange(5);				
		}
}

/**********************************************
*								״̬������
*						5	���ػ���ɣ�ϵͳ����
*   1��ϵͳ��������״̬ �� �ȴ�ϵͳ��λ
***********************************************/
void stateSysStoped(void)
{
//		printf("\n\t stateSysStoped\n\t");
//		FC_FJControl(0);				//�رշ���
}

/**********************************************
*									ģʽ�л�
*			mode :�л���ģʽ
***********************************************/
//����״̬������
const finiteStateMachine  stateMachineTable[] =
{
   	{0 ,(*stateSysSelfInspection)},     //�����Լ�
   	{1 ,(*stateSysWaitStart)},          //�ȴ�����
		{2 ,(*stateSysStarting)},           //���ڿ���
   	{3 ,(*stateSysRuning)},             //��������
		{4 ,(*stateSysStoping)},            //���ڹػ�
   	{5 ,(*stateSysStoped)}              //�ػ����
};

void finiteStateMachineTask(void)
{
		void (*current_operation_index)(void) = NULL ;
		
		//״̬������	
		current_operation_index = stateMachineTable[g_MCMS_system.msysState].current_operation ;
		(*current_operation_index)();
}
/*********************************************************************************************************
																				End Of File
*********************************************************************************************************/
