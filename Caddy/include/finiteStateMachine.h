#ifndef 	_FINITESTATEMACHINE_H__ 
#define 	_FINITESTATEMACHINE_H__

#include "LPC17xx.h" 

#define  WARN_STATESYSSELFINSPECTION   0x2782783f 	//�����Լ�
#define  WARN_STATESYSWAITSTART   		 0x2782783f 	//�ȴ�����
#define  WARN_STATESYSSTARTING    		 0x3fa3f83f 	//���ڿ���
#define  WARN_STATESYSRUNING  				 0x3ff3ff3f 	//��������
#define  WARN_STATESYSSTOPING   			 0x2782783f 	//���ڹػ�
#define  WARN_STATESYSSTOPED   				 0x2782783f 	//�ػ����

/******************************************
* 1���澯���ᱻ����
* 2������������ , �Ƿ�������
*    ��磺����
*    �ŵ�: ��    ����������̬��
*    ��������        
* 3������������ ,�޷��ػ�
* 4������������ ,FC�޷�����
* 5���¶ȴ�����   -20��֮����Ҫ��
* 6����ѵ�ѹ������  ������˸��ֱ�ӹػ��� �澯Ҳ��λ�ˡ�
*******************************************/

/*******************************************
*����ǰ SOC�ж�          ***/
//���� DISCHARGE_SOC , �ŵ�
//С�� CHARGE_SOC , ���
#define  DISCHARGE_SOC     70  
#define  CHARGE_SOC        30
//�������ݶ���   
//�ر�FC 
#define  CLOSE_FC_SOC      70  //90
//��FC
#define  OPEN_FC_SOC       40

/*******����ģʽ********/
#define  LOW_T_MODE_SOC    60


#define HJWD_MODE_LOW         28    //0������
#define HJWD_MODE_HIGH   		  35
#define HJWD_MODE_HIGH_OVER   55

/****************************************/
//�����¶�
typedef enum _T_mode
{
    T_Normal_mode = 0, 
		T_High_mode,
		T_Low_mode
}T_mode;

/**********************************************
*				          ״̬������
***********************************************/
typedef struct _finiteStateMachine_
{
	uint8_t modeID;															//ϵͳ״̬	    
	void (*current_operation)(void);						//ִ�г���
} finiteStateMachine , *P_finiteStateMachine;


extern const finiteStateMachine  stateMachineTable[] ;

extern void finiteStateMachineTask(void);

extern void breathingLED(void) ;

extern void FC_FJControl(uint8_t speed) ;
extern void PaiQiSelfComplex(uint16_t time) ;

#endif

