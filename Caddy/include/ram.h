
#ifndef  RAM_H
#define  RAM_H

#include <stdint.h>
#include "warn.h"
#include "IAPFlashHard.h"

#define FC_MODE   8

//#pragma pack(8)

__align(8) typedef struct
{
	float mBMS_SOC[10];    //计算缓存区
	float SOC_rate;        //消耗速率
	float ChargeTime;      //充放电时间
	uint8_t CompleteFlag;
	uint8_t ChangeGradeFlag;
	uint8_t LoadGrade;         //设置等级
	uint8_t mLoadGradeCurrent; //当前等级
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
		uint8_t msysState ;						//系统运行状态
		uint8_t msysControlMode ;    	//系统控制模式
		uint8_t msysOnOffOrder ;			//系统开/关机命令
		uWARN   msysWarn ;            //燃料电池模块告警
}SystemState;

typedef struct 
{
		float mBMS_V ;	
		float mBMS_A ;
		float mBMS_SOC ;
		float mBMS_Temp;  //温度
		float mBMS_P ;
		uint8_t mBMS_Warn;
		uint8_t mBMS_State;
		uint8_t mRelayState ;
}BMSRunpara;

__align(8) typedef struct 
{
		PARA_VALUE mF_V;						//电堆电压
		PARA_VALUE mF_A;            //电堆电流
		PARA_VALUE mF_CKWD;         //电堆出口温度
		PARA_VALUE mF_JKWD;         //电堆进口温度
		PARA_VALUE mF_MBWD;         //电堆目标温度
		PARA_VALUE mH_ND;		        //氢气浓度
		PARA_VALUE mH_YL;           //氢气压力
		uint8_t 	 mF_FJ; 					//风扇转速
		uint8_t    mF_Purg;					//吹扫风机
}Fcpara;

__align(8) typedef struct 
{
		PARA_VALUE mF_HJWD;         //箱内环境温度
		PARA_VALUE mH_ND;		        //氢气浓度
		PARA_VALUE mH_YL;           //氢气压力
		uint8_t 	 mF_FJ; 					//风扇转速
		uint8_t    mF_Purg;					//吹扫风机
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

//最大功率
__align(8) typedef struct 
{
	  uint8_t mDCDCState ;      //1:加载模式 , 2:降载模式, 3:正常模式
		uint8_t mBMSState ;       //1:正常模式 , 2: 充电模式
		uint8_t mPowerLimitMode ; //限流模式
		uint8_t mPowerMixSport ;  //BMS 和 燃料电池 混动模式
		uint8_t mTempMode ;  			//高低温模式
		float   mPowerMax ;  			//最大功率
		float   mPowerMin ; 	    //最小功率
}EnergyManagement;


extern 	SystemState  g_FCMS_system[FC_MODE] ;   //电堆系统状态
extern 	Fcpara  		 g_FCMS_runpara[FC_MODE];   //电堆运行数据
extern  DCpara       g_DCMS_runpara[FC_MODE] ;  //DCDC数据

extern  BMSRunpara   g_FCMS_BMSpara;      //BMS数据
extern  DCSetpara    g_DCMS_setpara ;    	//DCDC设置数据


extern  SystemState  g_MCMS_system ;      //主控系统状态
extern  EnergyManagement  g_MCMS_Power ;  //主控系统能量管理
extern  MCpara       g_MCMS_runpara ;     //主控运行数据

extern  st_SOC       g_RateSOC;           //SOC消耗率计算

extern void ramInit(void) ;



#define  CAN_PC_ADDRESS            0x01
#define  CAN_BMS_ADDRESS           0x05 
#define  CAN_MAINBORD_ADDRESS      0x50  
#define  CAN_FCBORD_ADDRESS     	 0x8C    


#define  DCDC_LOAD_STEP    50      //加载 单步
#define  DCDC_SHED_STEP    -300     //降载 单步
#define  DCDC_SETV_LOAD    0.2     //DCDC增高电压


#define  BMS_SOC_CHARGE_JUDGMENT    70   //充电模式判断
#define  DDDL_CURRENT_LIMITING      44   //电堆电压限流点, 49v
#define  POWER_RATING               3000 //额   2800
#define  POWER_MIN                  250  //DCDC输出最低是 300w

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
				//64字节
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
						uint8_t kp ; uint8_t ki ; uint8_t kd ; uint8_t kT ;  //PID值    
						uint16_t  PaiH_times ;   uint16_t  A_Integral ;
						uint16_t  FC_KaiLuV  ;   uint16_t  FC_OffV ;  //开路电压   关机电压    
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

//将512字节 数据映射成结构体形式
#define FLASHMEMORYMAP   ((FLash_data *)(&GucIapData)) 





#endif

