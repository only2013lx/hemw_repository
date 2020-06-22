
#ifndef  RAM_H
#define  RAM_H

#include <stdint.h>
#include "warn.h"
#include "IAPFlashHard.h"

#define FC_MODE   8

//#pragma pack(8)

__align(8) typedef struct
{
	float mBMS_SOC[10];    //���㻺����
	float SOC_rate;        //��������
	float ChargeTime;      //��ŵ�ʱ��
	uint8_t CompleteFlag;
	uint8_t ChangeGradeFlag;
	uint8_t LoadGrade;         //���õȼ�
	uint8_t mLoadGradeCurrent; //��ǰ�ȼ�
}st_SOC;


typedef  union
{
    uint16_t word;
    struct
		{
        uint16_t	DATA0	:8;
        uint16_t	DATA1	:8;
    }Bits;
}PARA_VALUE_16;


typedef  union  
{
	float    flaotword;
	uint32_t sendWord ;
	struct
	{
			uint8_t DATA0 ;
			uint8_t DATA1 ;
			uint8_t DATA2 ;
			uint8_t DATA3 ;
	}Bits ;
}PARA_VALUE ,*PPARA_VALUE;

typedef struct 
{
		uint8_t msysState ;						//ϵͳ����״̬
		uint8_t msysControlMode ;    	//ϵͳ����ģʽ
		uint8_t msysOnOffOrder ;			//ϵͳ��/�ػ�����
		uWARN   msysWarn ;            //ȼ�ϵ��ģ��澯
}SystemState;

typedef struct 
{
		float mBMS_V ;	
		float mBMS_A ;
		float mBMS_SOC ;
		float mBMS_Temp;  //�¶�
		float mBMS_P ;
		uint8_t mBMS_Warn;
		uint8_t mBMS_State;
		uint8_t mRelayState ;
}BMSRunpara;

__align(8) typedef struct 
{
		PARA_VALUE mF_V;						//��ѵ�ѹ
		PARA_VALUE mF_A;            //��ѵ���
		PARA_VALUE mF_CKWD;         //��ѳ����¶�
		PARA_VALUE mF_JKWD;         //��ѽ����¶�
		PARA_VALUE mF_MBWD;         //���Ŀ���¶�
		PARA_VALUE mH_ND;		        //����Ũ��
		PARA_VALUE mH_YL;           //����ѹ��
		uint8_t 	 mF_FJ; 					//����ת��
		uint8_t    mF_Purg;					//��ɨ���
}Fcpara;

__align(8) typedef struct 
{
		PARA_VALUE mF_HJWD;         //���ڻ����¶�
		PARA_VALUE mH_ND;		        //����Ũ��
		PARA_VALUE mH_YL;           //����ѹ��
		uint8_t 	 mF_FJ; 					//����ת��
		uint8_t    mF_Purg;					//��ɨ���
}MCpara;

__align(8) typedef struct 
{
		uint8_t	 mD_state; 					
		uint8_t  mD_Warn;				
		float  	 mD_V ;
		float  	 mD_A ;
		float    mD_V_IN ;
		float    mD_P ;
}DCpara;

__align(8) typedef struct 
{
		float   mD_V ;
		float   mD_A ;
		float   mD_P ;
}DCSetpara;

//�����
__align(8) typedef struct 
{
	  uint8_t mDCDCState ;      //1:����ģʽ , 2:����ģʽ, 3:����ģʽ
		uint8_t mBMSState ;       //1:����ģʽ , 2: ���ģʽ
		uint8_t mPowerLimitMode ; //����ģʽ
		uint8_t mPowerMixSport ;  //BMS �� ȼ�ϵ�� �춯ģʽ
		uint8_t mTempMode ;  			//�ߵ���ģʽ
		float   mPowerMax ;  			//�����
		float   mPowerMin ; 	    //��С����
}EnergyManagement;


extern 	SystemState  g_FCMS_system[FC_MODE] ;   //���ϵͳ״̬
extern 	Fcpara  		 g_FCMS_runpara[FC_MODE];   //�����������
extern  DCpara       g_DCMS_runpara[FC_MODE] ;  //DCDC����

extern  BMSRunpara   g_FCMS_BMSpara;      //BMS����
extern  DCSetpara    g_DCMS_setpara ;    	//DCDC��������


extern  SystemState  g_MCMS_system ;      //����ϵͳ״̬
extern  EnergyManagement  g_MCMS_Power ;  //����ϵͳ��������
extern  MCpara       g_MCMS_runpara ;     //������������

extern  st_SOC       g_RateSOC;           //SOC�����ʼ���

extern void ramInit(void) ;



#define  CAN_PC_ADDRESS            0x01
#define  CAN_BMS_ADDRESS           0x05 
#define  CAN_MAINBORD_ADDRESS      0x50  
#define  CAN_FCBORD_ADDRESS     	 0x8C    


#define  DCDC_LOAD_STEP    50      //���� ����
#define  DCDC_SHED_STEP    -300     //���� ����
#define  DCDC_SETV_LOAD    0.2     //DCDC���ߵ�ѹ


#define  BMS_SOC_CHARGE_JUDGMENT    70   //���ģʽ�ж�
#define  DDDL_CURRENT_LIMITING      44   //��ѵ�ѹ������, 49v
#define  POWER_RATING               3000 //��   2800
#define  POWER_MIN                  250  //DCDC�������� 300w

#define  POWER_MIXSPORT_RATING      0  

#define  DCSET_OVER_V               330




#define FLASH_SYS_WARN_BASE      (0u)
#define FLASH_FLOAT_BASE         (64u)
#define FLASH_FCRUN_BASE         (128u)

#define FLASH_FLOAT_FAN_SET     				 (FLASH_FLOAT_BASE)
#define FLASH_FLOAT_BLOWER_SET      		 (FLASH_FLOAT_BASE + 12u)
#define FLASH_FLOAT_V_SENSORCOE_SET      (FLASH_FLOAT_BASE + 12u + 12u)
#define FLASH_FLOAT_A_SENSORCOE_SET      (FLASH_FLOAT_BASE + 12u + 12u + 8u)
#define FLASH_FLOAT_FAN_LOWEST_SET       (FLASH_FLOAT_BASE + 12u + 12u + 8u + 8u)

#define FLASH_FCRUN_PID_SET     				 (FLASH_FCRUN_BASE)
#define FLASH_FCRUN_RUNP_SET     				 (FLASH_FCRUN_BASE + 4u)

typedef union
{
		uint8_t mflashData[512];
		struct
		{
				//64�ֽ�
				struct
				{
						uint16_t  Warn_DCV_L_Over;     
						uint16_t  Warn_DCV_H_Over;
						uint16_t  Warn_DCA_H_Over;     
						uint16_t  Warn_DCW_H_Over;													
						uint16_t  Warn_HYL_H;     
						uint16_t  Warn_HYL_H_Over;
						uint16_t  Warn_HYL_L;     
						uint16_t  Warn_HYL_L_Over;													
						uint16_t  Warn_HND_H;     
						uint16_t  Warn_HND_H_Over;
						uint16_t  Warn_SOC_L_Over;     
						uint16_t  Warn_SOC_H_Over;							
						uint16_t  Warn_HJWD_H;     
						uint16_t  Warn_HJWD_H_Over;													
						uint16_t  Warn_SOC_L;     
						uint16_t  Warn_SOC_H;	
						uint16_t  warnReverse17;      
						uint16_t  warnReverse18; 														
						uint16_t  warnReverse19;      
						uint16_t  warnReverse20; 
						uint16_t  warnReverse21;      
						uint16_t  warnReverse22; 
						uint16_t  warnReverse1 ;
						uint16_t  warnReverse2 ;
						uint16_t  warnReverse3 ;
						uint16_t  warnReverse4 ;
						uint16_t  warnReverse5 ;
						uint16_t  warnReverse6 ;
						uint16_t  warnReverse7 ;
						uint16_t  warnReverse8 ;	
					  uint16_t  warnReverse9 ;
						uint16_t  warnReverse10;
						uint16_t  warnReverse11;
						uint16_t  warnReverse12;	
				}warnBites;
				struct
				{
						float  Fan_Set_0;     
						float  Fan_Set_1;
						float  Fan_Set_2;     
						float  Blower_Set_0;													
						float  Blower_Set_1;     
						float  Blower_Set_2;
						float  V_SensorCoe_0;     
						float  V_SensorCoe_1;													
						float  A_SensorCoe_0;     
						float  A_SensorCoe_1;
						float  Fan_Lowerest_0;     
						float  Fan_Lowerest_1;														
						float  Reverse1;     
						float  Reverse2;
						float  Reverse3;      
						float  Reverse4; 														
				}floatBites;
				struct
				{
						uint8_t kp ; uint8_t ki ; uint8_t kd ; uint8_t kT ;  //PIDֵ    
						uint16_t  PaiH_times ;   uint16_t  A_Integral ;
						uint16_t  FC_KaiLuV  ;   uint16_t  FC_OffV ;  //��·��ѹ   �ػ���ѹ    
						uint32_t  Reverse1;													
						uint32_t  Reverse2;     
						uint32_t  Reverse3;
						uint32_t  Reverse4;     
						uint32_t  Reverse5;													
						uint32_t  Reverse6;     
						uint32_t  Reverse7;
						uint32_t  Reverse8;     
						uint32_t  Reverse9;														
						uint32_t  Reverse10;     
						uint32_t  Reverse11;
						uint32_t  Reverse12;      
						uint32_t  Reverse13; 														
				}FCrunBites;
				uint8_t Reverse4[64];
				uint8_t Reverse5[64];
				uint8_t Reverse6[64];
				uint8_t Reverse7[64];
				uint8_t Reverse8[64];
		}Bits;
}FLash_data;

//��512�ֽ� ����ӳ��ɽṹ����ʽ
#define FLASHMEMORYMAP   ((FLash_data *)(&GucIapData)) 





#endif

