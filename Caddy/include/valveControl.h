#ifndef  _H_VALVECONTROL_H
#define  _H_VALVECONTROL_H

#include "lpc17xx.h"


//���Ӵ���
extern void MasterContactor(uint8_t flag) ;
//����������
extern void MCalorifierSpeedSet(uint8_t flag);
//﮵�ؿ���
extern void PC_Control_Relay(uint8_t state) ;



//Ԥ��
//ѭ����
extern void CirclePumpControl(uint8_t flag);
//�ط����
extern void MReturnFanSpeedSet(uint8_t flag);
//���ȴ�����
extern void HeaterBandControl(uint8_t flag);
//��λFC��
extern void PC_Control_Resrt_FC(void) ;

#endif

