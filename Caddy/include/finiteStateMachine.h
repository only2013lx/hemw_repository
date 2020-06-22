#ifndef 	_FINITESTATEMACHINE_H__ 
#define 	_FINITESTATEMACHINE_H__

#include "LPC17xx.h" 

#define  WARN_STATESYSSELFINSPECTION   0x2782783f 	//开机自检
#define  WARN_STATESYSWAITSTART   		 0x2782783f 	//等待开机
#define  WARN_STATESYSSTARTING    		 0x3fa3f83f 	//正在开机
#define  WARN_STATESYSRUNING  				 0x3ff3ff3f 	//正在运行
#define  WARN_STATESYSSTOPING   			 0x2782783f 	//正在关机
#define  WARN_STATESYSSTOPED   				 0x2782783f 	//关机完成

/******************************************
* 1、告警还会被重启
* 2、正在启动中 , 是否对外输出
*    充电：不可
*    放电: 可    （正在启动态）
*    正常：可        
* 3、正在启动中 ,无法关机
* 4、正在启动中 ,FC无法加载
* 5、温度传感器   -20°之后需要测
* 6、电堆电压传感器  断线闪烁，直接关机， 告警也复位了。
*******************************************/

/*******************************************
*开机前 SOC判断          ***/
//大于 DISCHARGE_SOC , 放电
//小于 CHARGE_SOC , 充电
#define  DISCHARGE_SOC     70  
#define  CHARGE_SOC        30
//运行数据定义   
//关闭FC 
#define  CLOSE_FC_SOC      70  //90
//打开FC
#define  OPEN_FC_SOC       40

/*******低温模式********/
#define  LOW_T_MODE_SOC    60


#define HJWD_MODE_LOW         28    //0度以下
#define HJWD_MODE_HIGH   		  35
#define HJWD_MODE_HIGH_OVER   55

/****************************************/
//环境温度
typedef enum _T_mode
{
    T_Normal_mode = 0, 
		T_High_mode,
		T_Low_mode
}T_mode;

/**********************************************
*				          状态机任务
***********************************************/
typedef struct _finiteStateMachine_
{
	uint8_t modeID;															//系统状态	    
	void (*current_operation)(void);						//执行程序
} finiteStateMachine , *P_finiteStateMachine;


extern const finiteStateMachine  stateMachineTable[] ;

extern void finiteStateMachineTask(void);

extern void breathingLED(void) ;

extern void FC_FJControl(uint8_t speed) ;
extern void PaiQiSelfComplex(uint16_t time) ;

#endif

