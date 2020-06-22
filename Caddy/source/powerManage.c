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
@brief: ÓÃÓÚ¶ÓÁÐ´æ´¢10¸öSOCÖµ
@para:  buffÊÇÒª´æ´¢µÄbuff£¬dtÊÇµ±Ç°SOCÖµ
@retval:void
************************************************/
void InsertQuere(float buff[],float dt)
{
	//½«Ö®Öð¸öºóÒÆ
	unsigned char i;
	for(i=1;i<10;i++)
	{
		buff[i-1]=buff[i];
	}
	buff[9] = dt;
}

//¸ù¾Ýµ±Ç°Êä³öµÈ¼¶¼ÆËã   
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

//³ä·ÅµçÊ±¼ä¼ÆËã
//µÈ¼¶È·¶¨
//SOC < 20% , ÏûºÄÂÊÎª0 £¿   »á²»»á´ïµ½ÕâÖÖÇé¿ö
void Cal_SOC_Charge_Time(void)
{
		LoadGradeCurrentCalc(g_DCMS_runpara[2].mD_P) ;	  //¼ÆËãµ±Ç°µÈ¼¶
		
		if(1 == g_RateSOC.CompleteFlag)
		{ 			
			g_RateSOC.CompleteFlag = 0 ;
			//ï®µç³Ø ±£³Ö socÔÚ60%-70%  , ·ÅµçÊ±¼ä×îµÍÎª60%Ê±, ÇÐ»»µ½¸ßµÈ¼¶
			//³äµçÊ£ÓàÎª30%Ê±,ÇÐ»»µ½µÍµÈ¼¶
			//300w³äµçÏûºÄÂÊ -0.84 £º30%  35min			||  300w·ÅµçÏûºÄÂÊ 0.96  £º 60%  62.5min
			//60% , 200w·Åµç   ||   70% , 300w³äµç
			if(g_RateSOC.ChargeTime < -5 && g_RateSOC.ChargeTime > -50)    //300w³äµçÊ±¼ä  S0C = 80%
			{
					SetPerisSpeed(-1);//ÇÐ»»µ½µÍµÈ¼¶
			}
			else if(g_RateSOC.ChargeTime > 5 && g_RateSOC.ChargeTime < 150 && g_FCMS_BMSpara.mBMS_SOC >= 60)//·Åµç  62.5
			{
					SetPerisSpeed(1);//ÇÐ»»µ½¸ßµÈ¼¶
			}
			else if(g_RateSOC.ChargeTime > 5 && g_RateSOC.ChargeTime < 200 && g_FCMS_BMSpara.mBMS_SOC < 60)
			{
					SetPerisSpeed(1);//ÇÐ»»µ½¸ßµÈ¼¶
			}
		}
//		printf("Grade-qian:%d,%d,%f\n",g_RateSOC.mLoadGradeCurrent,g_RateSOC.LoadGrade,g_FCMS_BMSpara.mBMS_A);
		//ï®µç³Ø¹ý³å      È¼ÁÏµç³ØµçÑ¹Ì«µÍ   ½µÔØ
		if(g_FCMS_BMSpara.mBMS_A <= -20 || g_FCMS_runpara[0].mF_V.flaotword < 47)
		{
//				printf("-20\n");
				SetPerisSpeed(-1);
		}
//		printf("Grade-hou:%d,%d\n",g_RateSOC.mLoadGradeCurrent,g_RateSOC.LoadGrade);
}

/************************************************
@brief: ¼ÆËãSOCµÄÏûºÄÂÊ
@para:  void 
@retval:void 
************************************************/
static void __Cal_SOC_Rate_callback(void *p_arg)
{ 
		static uint8_t timings = 0 ;
	
		InsertQuere(g_RateSOC.mBMS_SOC,g_FCMS_BMSpara.mBMS_SOC);
		if(g_RateSOC.mBMS_SOC[0]>0)
		{
			g_RateSOC.SOC_rate = (g_RateSOC.mBMS_SOC[0] - g_RateSOC.mBMS_SOC[9])/1.5;//²É10´Î£¬Ã¿´Î0.25min£¬×ÜÊ±¼ä2.5min
			if(timings++ >= 9)
			{
				 g_RateSOC.CompleteFlag = 1;
				 timings = 0 ;
				
					//ÏûºÄÂÊÎª0µÄÊ±ºòÊÇ·ñÒªÇÐ»»µÈ¼¶ÄØ£¿£¿  
					//ÏûºÄÂÊÎª0Ê±£¬³ä·ÅµçÊ±¼ä±íÊ¾ÔõÃ´±íÊ¾£¿
					if(g_RateSOC.SOC_rate > 0)//·Åµç
					{
							g_RateSOC.ChargeTime = g_FCMS_BMSpara.mBMS_SOC/g_RateSOC.SOC_rate;
					}
					else//³äµç
					{
							g_RateSOC.ChargeTime = (100-g_FCMS_BMSpara.mBMS_SOC)/g_RateSOC.SOC_rate;
					}
			}
		}
}

/************************************************
@brief: ¸ù¾ÝSOCµÄÏûºÄÂÊ,¼ÆËã³ö¸ºÔØµÈ¼¶
@para:  void
@retval:0:160W£¬1:300W
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
@brief: ¸ù¾ÝSOCµÄÏûºÄÂÊ,¼ÆËã³ö¸ºÔØµÈ¼¶
@para:  gradeÎª0±íÊ¾160w,1±íÊ¾300w
@retval:void
************************************************/
void SetPerisSpeed(int grade)
{
	//µÈ¼¶¼ÆËã , ¹²4µÈ¼¶
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

//¶¨Ê±
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
		//¸ßÎ»ÔÚÇ°£¬µÍÎ»ÔÚºó
		sendMessage.mCANDF.DATA[2] = SV_IN >> 8;
		sendMessage.mCANDF.DATA[3] = SV_IN;
		sendMessage.mCANDF.DATA[4] = SV >> 8;
		sendMessage.mCANDF.DATA[5] = SV;	
		sendMessage.mCANDF.DATA[6] = SA >> 8;
		sendMessage.mCANDF.DATA[7] = SA;	
		WriteCANSedCyBuf(CAN1 , &sendMessage);	 		//·¢ËÍµ½ÉÏÎ»»ú
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
		//¸ßÎ»ÔÚÇ°£¬µÍÎ»ÔÚºó
		sendMessage.mCANDF.DATA[2] = (uint8_t)(SV_IN >> 8);
		sendMessage.mCANDF.DATA[3] = (uint8_t)SV_IN;
		sendMessage.mCANDF.DATA[4] = (uint8_t)(SV >> 8);
		sendMessage.mCANDF.DATA[5] = (uint8_t)SV;	
		sendMessage.mCANDF.DATA[6] = (uint8_t)(SA >> 8);
		sendMessage.mCANDF.DATA[7] = (uint8_t)SA;	
		WriteCANSedCyBuf(CAN2 , &sendMessage);	 		//·¢ËÍµ½DC/DC
		printf("VA:%d,%d,%d\n" , SV_IN,SV,SA);
		DCDCSetSendPC(SV_IN,SV,SA);                 //·¢ËÍµ½ÉÏÎ»»ú
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
		
		g_DCMS_setpara.mD_A  = g_DCMS_setpara.mD_P / g_DCMS_runpara[0].mD_V ;  //¼ÆËãÉèÖÃµçÁ÷   
		
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
		
		//×îÐ¡Êä³öµçÁ÷   3A £¬ Ã¿¸öDC/DC
		if(g_DCMS_setpara.mD_A < 9)
		{
				g_DCMS_setpara.mD_A = 9; 
		}

		DCDCSetCAN(DCSET_OVER_V ,  g_DCMS_setpara.mD_V * 10 , g_DCMS_setpara.mD_A * 3.3333);  //g_DCMS_setpara.mD_V   
		printf("set:%f,%f£¬%f\n" , g_DCMS_setpara.mD_P , g_DCMS_setpara.mD_V, g_DCMS_setpara.mD_A);

		OSTimeDlyHMSM(0, 0, 1, 0);
	
		//ÉèÖÃ¹¦ÂÊ > Êµ¼ÊÊä³ö¹¦ÂÊ £¬ÇÒï®µç³ØÊä³öµçÁ÷ ´óÓÚ0.5A   
		//ï®µç³äµç 
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
@brief: ¸ù¾Ý¸ºÔØµÈ¼¶,ÉèÖÃÊä³ö,Ê¹´ïµ½Ä¿±ê¹¦ÂÊ
@para:  grd:¸ºÔØµÈ¼¶
@retval:0:µÈÓÚ£¬1:Ð¡ÓÚ 2:´óÓÚ
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
	else if(g_DCMS_runpara[2].mD_P > (dstpower + 100))  //ÓàÁ¿
	{
		DCDCSetPower(DCDC_SHED_STEP);
//		return 2;
	}		
// 	printf("DP:%f\n" , g_DCMS_runpara[2].mD_P);
	return 0;
}


/**************************
*			power Manage
*1¡¢µ±È¼ÁÏµç³Ø¿ª»ú³É¹¦£¬´ò¿ª   ®µç³ØÊä³ö¿ª¹Ø
*2¡¢ÉèÖÃDC/DCÊä³öµçÑ¹Îªï®µç³ØµçÑ¹ + xV ,µçÁ÷Îª 5A
*3¡¢ÉèÖÃDC/DCÐ±ÂÊ¼ÓÔØ
**************************/
void powerMange(void)
{	
		static uint8_t flag = 0 ; 

		//ÕýÔÚÔËÐÐÌ¬
		if(3 == g_MCMS_system.msysState)
		{
				//DC/DC  ¿ª»ú³õÊ¼»¯ÉèÖÃ
				if(0 == flag)
				{
						//ÉèÖÃDCDCÊä³ö³õÊ¼Öµ
						flag = 1; 
						//µÈ´ý3s
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
				Cal_SOC_Charge_Time();                //¼ÆËã¸ºÔØµÈ¼¶
				JudgeDestPower(g_RateSOC.LoadGrade);  //¼ÓÔØ
		}
		
		//ÕýÔÚ¹Ø»ú£¬¹Ø±Õï®µç³ØÊä³ö
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
