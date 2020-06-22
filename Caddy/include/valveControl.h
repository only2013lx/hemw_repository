#ifndef  _H_VALVECONTROL_H
#define  _H_VALVECONTROL_H

#include "lpc17xx.h"


//主接触器
extern void MasterContactor(uint8_t flag) ;
//加热器控制
extern void MCalorifierSpeedSet(uint8_t flag);
//锂电池开关
extern void PC_Control_Relay(uint8_t state) ;



//预留
//循环泵
extern void CirclePumpControl(uint8_t flag);
//回风风扇
extern void MReturnFanSpeedSet(uint8_t flag);
//加热带控制
extern void HeaterBandControl(uint8_t flag);
//复位FC板
extern void PC_Control_Resrt_FC(void) ;

#endif

