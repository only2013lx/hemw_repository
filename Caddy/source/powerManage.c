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
#include "powerManage.h"
#include "ram.h"
#include "timerDriver.h"
#include "canSendQueue.h"
#include "canTask.h"
//#include "types.h"
#include "ucos_ii.h"
#include "am_softimer.h"
#include "am_vdebug.h"


/************************************************
@brief: ���ڶ��д洢10��SOCֵ
@para:  buff��Ҫ�洢��buff��dt�ǵ�ǰSOCֵ
@retval:void
************************************************/
void InsertQuere(float buff[],float dt)
{
	//��֮�������
	unsigned char i;
	for(i=1;i<10;i++)
	{
		buff[i-1]=buff[i];
	}
	buff[9] = dt;
}

//���ݵ�ǰ����ȼ�����   
void LoadGradeCurrentCalc(float p)
{
		if(p >= 2700)
		{
				g_RateSOC.mLoadGradeCurrent = 4 ;
		}
		else if(p >= 2400)
		{
				g_RateSOC.mLoadGradeCurrent = 3 ;				
		}
		else if(p >= 1900)
		{
				g_RateSOC.mLoadGradeCurrent = 2 ;
		}
		else if(p >= 1400)
		{
				g_RateSOC.mLoadGradeCurrent = 1 ;
		}
		else
		{
				g_RateSOC.mLoadGradeCurrent = 0 ;
		}
//		printf("V-L:%f,%d\n", p , g_RateSOC.mLoadGradeCurrent);
}

//��ŵ�ʱ�����
//�ȼ�ȷ��
//SOC < 20% , ������Ϊ0 ��   �᲻��ﵽ�������
void Cal_SOC_Charge_Time(void)
{
		LoadGradeCurrentCalc(g_DCMS_runpara[2].mD_P) ;	  //���㵱ǰ�ȼ�
		
		if(1 == g_RateSOC.CompleteFlag)
		{ 			
			g_RateSOC.CompleteFlag = 0 ;
			//﮵�� ���� soc��60%-70%  , �ŵ�ʱ�����Ϊ60%ʱ, �л����ߵȼ�
			//���ʣ��Ϊ30%ʱ,�л����͵ȼ�
			//300w��������� -0.84 ��30%  35min			||  300w�ŵ������� 0.96  �� 60%  62.5min
			//60% , 200w�ŵ�   ||   70% , 300w���
			if(g_RateSOC.ChargeTime < -5 && g_RateSOC.ChargeTime > -50)    //300w���ʱ��  S0C = 80%
			{
					SetPerisSpeed(-1);//�л����͵ȼ�
			}
			else if(g_RateSOC.ChargeTime > 5 && g_RateSOC.ChargeTime < 150 && g_FCMS_BMSpara.mBMS_SOC >= 60)//�ŵ�  62.5
			{
					SetPerisSpeed(1);//�л����ߵȼ�
			}
			else if(g_RateSOC.ChargeTime > 5 && g_RateSOC.ChargeTime < 200 && g_FCMS_BMSpara.mBMS_SOC < 60)
			{
					SetPerisSpeed(1);//�л����ߵȼ�
			}
		}
//		printf("Grade-qian:%d,%d,%f\n",g_RateSOC.mLoadGradeCurrent,g_RateSOC.LoadGrade,g_FCMS_BMSpara.mBMS_A);
		//﮵�ع���      ȼ�ϵ�ص�ѹ̫��   ����
		if(g_FCMS_BMSpara.mBMS_A <= -20 || g_FCMS_runpara[0].mF_V.flaotword < 47)
		{
//				printf("-20\n");
				SetPerisSpeed(-1);
		}
//		printf("Grade-hou:%d,%d\n",g_RateSOC.mLoadGradeCurrent,g_RateSOC.LoadGrade);
}

/************************************************
@brief: ����SOC��������
@para:  void 
@retval:void 
************************************************/
static void __Cal_SOC_Rate_callback(void *p_arg)
{ 
		static uint8_t timings = 0 ;
	
		InsertQuere(g_RateSOC.mBMS_SOC,g_FCMS_BMSpara.mBMS_SOC);
		if(g_RateSOC.mBMS_SOC[0]>0)
		{
			g_RateSOC.SOC_rate = (g_RateSOC.mBMS_SOC[0] - g_RateSOC.mBMS_SOC[9])/1.5;//��10�Σ�ÿ��0.25min����ʱ��2.5min
			if(timings++ >= 9)
			{
				 g_RateSOC.CompleteFlag = 1;
				 timings = 0 ;
				
					//������Ϊ0��ʱ���Ƿ�Ҫ�л��ȼ��أ���  
					//������Ϊ0ʱ����ŵ�ʱ���ʾ��ô��ʾ��
					if(g_RateSOC.SOC_rate > 0)//�ŵ�
					{
							g_RateSOC.ChargeTime = g_FCMS_BMSpara.mBMS_SOC/g_RateSOC.SOC_rate;
					}
					else//���
					{
							g_RateSOC.ChargeTime = (100-g_FCMS_BMSpara.mBMS_SOC)/g_RateSOC.SOC_rate;
					}
			}
		}
}

/************************************************
@brief: ����SOC��������,��������صȼ�
@para:  void
@retval:0:160W��1:300W
************************************************/
uint8_t CalPowerGrade(void)
{
	if(g_RateSOC.SOC_rate > 50)
	{
		g_RateSOC.LoadGrade =  1;
	}
	else if(g_RateSOC.SOC_rate >0 &&g_RateSOC.SOC_rate < 50)
	{
		g_RateSOC.LoadGrade =  0;
	}
  return g_RateSOC.LoadGrade;
}

/************************************************
@brief: ����SOC��������,��������صȼ�
@para:  gradeΪ0��ʾ160w,1��ʾ300w
@retval:void
************************************************/
void SetPerisSpeed(int grade)
{
	//�ȼ����� , ��4�ȼ�
	int tmp = g_RateSOC.mLoadGradeCurrent ;
	tmp += grade ;
		
	if(tmp < 0)
	{
			tmp = 0 ;
	}
	else if(tmp > 4)
	{
			tmp = 4 ;
	}
	g_RateSOC.LoadGrade =  tmp ;
//	AM_DBG_INFO("Grade:%d,%d",g_RateSOC.mLoadGradeCurrent,g_RateSOC.LoadGrade);
}

static am_softimer_t __g_SOCRateTimer ;

//��ʱ
//void __power_callback(void *p_arg)
//{
//		
//}

void socRate_timer_start(void)
{
		am_softimer_start(&__g_SOCRateTimer , 9000); 		//9s
}

void socRate_timer_stop(void)
{
		am_softimer_stop(&__g_SOCRateTimer); 		
}

void powerInit(void)
{
		am_softimer_init(&__g_SOCRateTimer , __Cal_SOC_Rate_callback , NULL);
}

void DCDCSetSendPC(uint16_t SV_IN ,uint16_t SV , uint16_t SA)
{
		j1939_message sendMessage ;
		sendMessage.channel 	= CAN1 ;
		sendMessage.FF      	= 1 ;
		sendMessage.LEN     	= 8;		
		sendMessage.std_id.ID = 0x1822508C ;
		
		sendMessage.mCANDF.DATA[0] = 0x78 ;
		sendMessage.mCANDF.DATA[1] = 0x20 ;
		//��λ��ǰ����λ�ں�
		sendMessage.mCANDF.DATA[2] = SV_IN >> 8;
		sendMessage.mCANDF.DATA[3] = SV_IN;
		sendMessage.mCANDF.DATA[4] = SV >> 8;
		sendMessage.mCANDF.DATA[5] = SV;	
		sendMessage.mCANDF.DATA[6] = SA >> 8;
		sendMessage.mCANDF.DATA[7] = SA;	
		WriteCANSedCyBuf(CAN1 , &sendMessage);	 		//���͵���λ��
}

void DCDCSetCAN(uint16_t SV_IN ,uint16_t SV , uint16_t SA)
{
		j1939_message sendMessage ;
		sendMessage.channel 	= CAN2 ;
		sendMessage.FF      	= 1 ;
		sendMessage.LEN     	= 8;		
		sendMessage.std_id.ID = 0xc408c50 ;
		
		sendMessage.mCANDF.DATA[0] = 0x78 ;
		sendMessage.mCANDF.DATA[1] = 0x20 ;
		//��λ��ǰ����λ�ں�
		sendMessage.mCANDF.DATA[2] = (uint8_t)(SV_IN >> 8);
		sendMessage.mCANDF.DATA[3] = (uint8_t)SV_IN;
		sendMessage.mCANDF.DATA[4] = (uint8_t)(SV >> 8);
		sendMessage.mCANDF.DATA[5] = (uint8_t)SV;	
		sendMessage.mCANDF.DATA[6] = (uint8_t)(SA >> 8);
		sendMessage.mCANDF.DATA[7] = (uint8_t)SA;	
		WriteCANSedCyBuf(CAN2 , &sendMessage);	 		//���͵�DC/DC
		printf("VA:%d,%d,%d\n" , SV_IN,SV,SA);
		DCDCSetSendPC(SV_IN,SV,SA);                 //���͵���λ��
}

void DCDCSetPower(float power)
{
		static uint8_t timings = 0 ;
		//printf("%f,%f,%f\n" , g_DCMS_runpara[0].mD_A , g_DCMS_runpara[1].mD_A , g_DCMS_runpara[2].mD_A);
		g_DCMS_setpara.mD_P = g_DCMS_runpara[2].mD_P + power ;
//		printf("mD_P:%f\n" , g_DCMS_setpara.mD_P);
		if(g_DCMS_setpara.mD_P > POWER_RATING)  //g_MCMS_Power.mPowerMax
		{
				g_DCMS_setpara.mD_P = POWER_RATING ;
		}
		/*
		if(1 == g_MCMS_Power.mPowerMixSport)
		{
				if(g_DCMS_setpara.mD_P > POWER_MIXSPORT_RATING)
				{
						g_DCMS_setpara.mD_P = POWER_MIXSPORT_RATING + 150 ;
				}	
		}
		*/
		if(g_DCMS_setpara.mD_P < POWER_MIN)
		{
				g_DCMS_setpara.mD_P = POWER_MIN ;
		}
		
		g_DCMS_setpara.mD_A  = g_DCMS_setpara.mD_P / g_DCMS_runpara[0].mD_V ;  //�������õ���   
		
		//g_DCMS_setpara.mD_V = 27 ;
		
//		if(g_DCMS_runpara[0].mD_V < 10)
//		{
//				DCDCSetCAN(g_DCMS_setpara.mD_V_IN * 10 , g_DCMS_setpara.mD_V * 10 , 6 * 5);
//				return ;
//		}
		
//		if(g_DCMS_setpara.mD_A > 90)
//		{
//				g_DCMS_setpara.mD_A = 90 ;
//		}
		
		//��С�������   3A �� ÿ��DC/DC
		if(g_DCMS_setpara.mD_A < 9)
		{
				g_DCMS_setpara.mD_A = 9; 
		}

		DCDCSetCAN(DCSET_OVER_V ,  g_DCMS_setpara.mD_V * 10 , g_DCMS_setpara.mD_A * 3.3333);  //g_DCMS_setpara.mD_V   
		printf("set:%f,%f��%f\n" , g_DCMS_setpara.mD_P , g_DCMS_setpara.mD_V, g_DCMS_setpara.mD_A);

		OSTimeDlyHMSM(0, 0, 1, 0);
	
		//���ù��� > ʵ��������� ����﮵��������� ����0.5A   
		//﮵��� 
		if(g_DCMS_setpara.mD_P > g_DCMS_runpara[0].mD_V * (g_DCMS_runpara[0].mD_A + g_DCMS_runpara[1].mD_A) && g_FCMS_BMSpara.mBMS_A > 0.5)
		{
				timings++ ;
		}
		if(timings >= 5)
		{
				timings = 0 ;
				g_DCMS_setpara.mD_V += 0.2 ;  
				DCDCSetCAN(DCSET_OVER_V ,  g_DCMS_setpara.mD_V * 10 , g_DCMS_setpara.mD_A  * 3.3333);						
		}
}

void setLiOutput(uint8_t flag)
{
		FAMEN_4_Control(flag);
}

/************************************************
@brief: ���ݸ��صȼ�,�������,ʹ�ﵽĿ�깦��
@para:  grd:���صȼ�
@retval:0:���ڣ�1:С�� 2:����
************************************************/
uint8_t JudgeDestPower(uint8_t grd)
{
	uint16_t dstpower;

	switch(grd)
	{
		case 0:
			dstpower = PURE_H_POWER1;
			break ;
		case 1:
			dstpower = PURE_H_POWER2;
			break;
		case 2:
			dstpower = PURE_H_POWER3;
			break ;
		case 3:
			dstpower = PURE_H_POWER4;
			break ;
		case 4:
			dstpower = PURE_H_POWER5;
			break ;
		default:
			break ;
	}
	
	if(g_DCMS_runpara[2].mD_P == dstpower)
	{
//		return 0;
	}
	else if(g_DCMS_runpara[2].mD_P < dstpower)
	{
		DCDCSetPower(DCDC_LOAD_STEP);
//		return 1;
	}
	else if(g_DCMS_runpara[2].mD_P > (dstpower + 100))  //����
	{
		DCDCSetPower(DCDC_SHED_STEP);
//		return 2;
	}		
// 	printf("DP:%f\n" , g_DCMS_runpara[2].mD_P);
	return 0;
}


/**************************
*			power Manage
*1����ȼ�ϵ�ؿ����ɹ�����   �����������
*2������DC/DC�����ѹΪ﮵�ص�ѹ + xV ,����Ϊ 5A
*3������DC/DCб�ʼ���
**************************/
void powerMange(void)
{	
		static uint8_t flag = 0 ; 

		//��������̬
		if(3 == g_MCMS_system.msysState)
		{
				//DC/DC  ������ʼ������
				if(0 == flag)
				{
						//����DCDC�����ʼֵ
						flag = 1; 
						//�ȴ�3s
						OSTimeDlyHMSM(0, 0, 3, 0);
//						g_DCMS_setpara.mD_V_IN = 33 ;
		
					  g_DCMS_setpara.mD_V    = 28.6;  //+0.3V    
					
						if(g_DCMS_setpara.mD_V >= 28.6)
						{
								g_DCMS_setpara.mD_V = 28.6;
						}
						
						if(g_DCMS_setpara.mD_V <= 22)
						{
								g_DCMS_setpara.mD_V = 22 ;
						}						
						g_DCMS_setpara.mD_A    = 9 ;
				}
				Cal_SOC_Charge_Time();                //���㸺�صȼ�
				JudgeDestPower(g_RateSOC.LoadGrade);  //����
		}
		
		//���ڹػ����ر�﮵�����
		if(4 == g_MCMS_system.msysState)
		{ 
				if(1 == flag)
				{
						flag = 2 ;
						setLiOutput(0) ;  
//						timer_stop(POWER_TIMER_NO);
				}
		}
}

/******************************************************************************
**                            End Of File
******************************************************************************/
