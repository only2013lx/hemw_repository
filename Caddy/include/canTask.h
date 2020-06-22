
#ifndef  CANTASK_H
#define  CANTASK_H

#include <stdint.h>


#define  FC_LOCAL_ADRESS       1


#define  SET_WARN         1
#define  SET_FAN          2
#define  SET_PID          3
#define  SET_BLOWER       4
#define  SET_FAN_LOWEST   5
#define  SET_VOLTAGE      6
#define  SET_CURRENT      7
#define  SET_FC_PAR       8
#define  SET_SYS_WARN     10

#define  READ_WARN        11
#define  READ_FAN         12
#define  READ_PID         13
#define  READ_BLOWER      14
#define  READ_FAN_LOWEST  15
#define  READ_VOLTAGE     16
#define  READ_CURRENT     17
#define  READ_FC_PAR      18
#define  READ_SYS_WARN    20

//控制指令
#define CONTROL_SYSRESET   40
#define CONTROL_ONOFF      41
#define CONTROL_MODULE     42

#define CONTROL_FC_VALVE1     43
#define CONTROL_FC_VALVE2     44
#define CONTROL_FC_VALVE3     45
#define CONTROL_FC_VALVE4     46
#define CONTROL_FC_VALVE5     47
#define CONTROL_FC_PWM1       48
#define CONTROL_FC_PWM2       49

#define CONTROL_MAIN_VALVE1    63
#define CONTROL_MAIN_VALVE2    64
#define CONTROL_MAIN_VALVE3    65
#define CONTROL_MAIN_VALVE4    66
#define CONTROL_MAIN_VALVE5    67
#define CONTROL_MAIN_PWM1      68
#define CONTROL_MAIN_PWM2      69
#define CONTROL_MAIN_BMS       70

/********************************
*1、服务数据
*2、过程数据
*3、网络数据
*4、命令/请求
*********************************/
#define  RXBOX_SERVER_SIZE     (20u+1u)
#define  RXBOX_PROCESS_SIZE    (15u+1u)
#define  RXBOX_INTERNET_SIZE   (5u+1u)
#define  RXBOX_ORDER_SIZE   	 (5u+1u) 


extern void canTaskInit(void) ;
extern void canSeverHandle(void) ;
extern void canProcessHandle(void) ;
extern void canInternetHandle(void) ;
extern void canOrderHandle(void) ;
extern void canDataSend(void) ;
//extern void PC_Control_Relay(uint8_t state) ;

#endif

