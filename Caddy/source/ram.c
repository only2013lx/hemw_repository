
#include  "ram.h"
#include  "am_types.h"



SystemState  g_FCMS_system[FC_MODE] ;    
Fcpara  		 g_FCMS_runpara[FC_MODE];
DCpara       g_DCMS_runpara[FC_MODE] ;

BMSRunpara   g_FCMS_BMSpara;      //BMS数据
DCSetpara    g_DCMS_setpara ;    	//DCDC设置数据

EnergyManagement  g_MCMS_Power ;  //主控系统能量管理
SystemState  g_MCMS_system ;      //主控系统
MCpara       g_MCMS_runpara ;     //主控运行数据

st_SOC       g_RateSOC; 


void ramInit(void)
{
	g_MCMS_Power.mBMSState  = 0 ;
	g_MCMS_Power.mDCDCState = 0 ;
	g_MCMS_Power.mPowerMax  = POWER_RATING ;
	g_MCMS_Power.mPowerMin  = 0 ;  //最小功率 1kw
	g_MCMS_Power.mPowerLimitMode = 0 ;
	g_MCMS_Power.mTempMode = 0 ;
	
	g_MCMS_system.msysWarn.word = 0 ;
	g_MCMS_system.msysControlMode = 0 ;
	g_MCMS_system.msysOnOffOrder  = 0 ;
	g_MCMS_system.msysState = 0 ;
}
