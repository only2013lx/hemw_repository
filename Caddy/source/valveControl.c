
#include  "valveControl.h"
#include  "ioControl.h"
#include  "canDriver.h"
#include  "canHard.h"
#include  "canSendQueue.h"
#include 	"ucos_ii.h"

//主接触器
void MasterContactor(uint8_t flag)
{
		if(flag)
		{
			  FAMEN_2_Control(1);				
		}
		else
		{
				FAMEN_2_Control(0);				
		}			
}

//循环泵
void CirclePumpControl(uint8_t flag)
{
		
}

//回风风扇
void MReturnFanSpeedSet(uint8_t flag)
{
		
}

//加热器控制
void MCalorifierSpeedSet(uint8_t flag)
{
		if(flag)
		{
				FAMEN_1_Control(1);
		}
		else
		{
				FAMEN_1_Control(0);				
		}				
}

//加热带控制
void HeaterBandControl(uint8_t flag)
{
		
}

void PC_Control_Relay(uint8_t state)
{
		j1939_message sendMessage ;
		sendMessage.channel 	= CAN2 ;
		sendMessage.FF      	= 1 ;
		sendMessage.LEN     	= 8 ;		
		sendMessage.std_id.ID = 0x1816A8B0 ;
		//sendMessage.std_id.pdu.source_address = sourAddress ;
		sendMessage.mCANDF.DATA[0] = state;
		sendMessage.mCANDF.DATA[1] = 0xFF;
		sendMessage.mCANDF.DATA[2] = 0xFF;
		sendMessage.mCANDF.DATA[3] = 0xFF;	
		sendMessage.mCANDF.DATA[4] = 0xFF;
		sendMessage.mCANDF.DATA[5] = 0xFF;
		sendMessage.mCANDF.DATA[6] = 0xFF;
		sendMessage.mCANDF.DATA[7] = 0xFF;
		WriteCANSedCyBuf(CAN2 , &sendMessage);		
}

void PC_Control_Resrt_FC(void)
{
		j1939_message sendMessage ;
		sendMessage.channel 	= CAN2 ;
		sendMessage.FF      	= 1 ;
		sendMessage.LEN     	= 2 ;		
		sendMessage.std_id.ID = 0x8148C01 ;
		sendMessage.mCANDF.DATA[0] = 40;
		sendMessage.mCANDF.DATA[1] = 0x77;
		WriteCANSedCyBuf(CAN2 , &sendMessage);		
}


//呼吸灯
void breathingLED(void)
{
		breathingLEDOn(); 	
		OSTimeDlyHMSM(0, 0, 0, 300);
		breathingLEDOff();
		OSTimeDlyHMSM(0, 0, 0, 300);
}
