
#include  "canTask.h"
#include  "canDriver.h"
#include  "canHard.h"
#include  "ram.h"
#include  "ioControl.h"
#include  "pwmDriver.h"
#include  "j1939Stack.h"
#include  "IAPFlashDriver.h"
#include  "canSendQueue.h"
#include  <string.h>
#include  "warn.h"
#include "os_cpu.h"
#include "valveControl.h"
#include "pwmHard.h"

#define  CHECK_EQUALITY(checkId,id)   ((checkId == id)?1:0 ) //��Ƚ��Ϊ0


CAN_Stack_Type myStack ;      			 //����ɸѡ��
static CAN_Box_Type   rxboxServer;   //��������
static CAN_Box_Type   rxboxProcess ; //��������
static CAN_Box_Type   rxboxInternet; //��������
static CAN_Box_Type   rxboxOrder ;   //ָ��
static MtPack recPack ;  						 //��֡����֡

uint8_t canFilter_0(uint32_t id)
{
	return CHECK_EQUALITY(0x801ff01 , id) ;  //IO�ڿ���
//		if(0x8140001 == (id & 0xFFF00FF))
//		{
//				return 1 ;
//		}
//		return 0 ;
}

//��������
uint8_t canFilter_1(uint32_t id)           
{
//	return CHECK_EQUALITY(0xc1e0301 , id) ;  //��ѹ������
	//����˫��� ����  ˫�� �ϴ� �� ����
	if(0x100A0000 == (id & 0xFFFF0000))
	{
			return 1 ;
	}
	return 0 ;
}

uint8_t canFilter_2(uint32_t id)
{
//	return CHECK_EQUALITY(0xc1f0301 , id) ;
	if(0x100B0000 == (id & 0xFFFF0000))    //����������
	{
			return 1 ;
	}
	return 0 ;
}

uint8_t canFilter_3(uint32_t id)
{
//	return CHECK_EQUALITY(0xc200301 , id) ;
	if(0x100C0000 == (id & 0xFFFF0000))   //�������ת��
	{
			return 1 ;
	}
	return 0 ;
}

uint8_t canFilter_4(uint32_t id)
{
//	return CHECK_EQUALITY(0xc210301 , id) ;
	if(0x100D0000 == (id & 0xFFFF0000)) //������в���
	{
			return 1 ;
	}
	return 0 ;
}

uint8_t canFilter_5(uint32_t id)
{
//	return CHECK_EQUALITY(0xc220301 , id) ;
	if(0x100E0000 == (id & 0xFFFF0000))  //PID����
	{
			return 1 ;
	}
	return 0 ;
}

uint8_t canFilter_6(uint32_t id)
{
	return CHECK_EQUALITY(0xc02ff01 , id) ;	   //END   
//	if(0xc02ff01 == (id & 0xFFFF0000))       //DCDC����
//	{
//			return 1 ;
//	}
//	return 0 ;
	
	
}

uint8_t canFilter_7(uint32_t id)
{
//	return CHECK_EQUALITY(0xcecff01 , id) ;	 //�㲥֡ - ֡ͷ
	if(0xCECFF00 == (id & 0xFFFFFF00))
	{
			return 1 ;
	}
	return 0 ;
}

uint8_t canFilter_8(uint32_t id)
{
//	return CHECK_EQUALITY(0xcebff01, id) ;	//�㲥֡ - ��֡����  0xcebff01
	if(0xCEBFF00 == (id & 0xFFFFFF00))
	{
			return 1 ;
	}
	return 0 ;
}

uint8_t canFilter_9(uint32_t id)
{
	return CHECK_EQUALITY(0x1012ff01 , id) ;	//����PGN����
//		if(0x1012ff01 == (id & 0xFFFF00FF))
//		{
//				return 1 ;
//		}
//		return 0 ;
}

uint8_t canFilter_10(uint32_t id)
{
		if(0xc008c50 == (id & 0xF00FFFF))
		{
				return (TRUE) ;
		}
		return (FALSE) ;
//	return CHECK_EQUALITY(0xc3c5001 , id) ;   //dcdc����         -PC ����
}

uint8_t canFilter_11(uint32_t id)
{
	return CHECK_EQUALITY(0xc3d5001 , id) ;   //dcdc����Ĭ��ֵ   - PC����
}

uint8_t canFilter_12(uint32_t id)
{
	return CHECK_EQUALITY(0 , id) ;  
}

uint8_t canFilter_13(uint32_t id) 
{
	return CHECK_EQUALITY(0 , id) ;  
}

uint8_t canFilter_14(uint32_t id)
{
	return CHECK_EQUALITY(0 , id) ;   
}

uint8_t canFilter_15(uint32_t id)
{
	return CHECK_EQUALITY(0 , id) ;   
}

uint8_t canFilter_16(uint32_t id)
{
	return CHECK_EQUALITY(0 , id) ;  
}

uint8_t canFilter_17(uint32_t id)
{
	return CHECK_EQUALITY(0 , id) ;   
}

uint8_t canFilter_18(uint32_t id)
{
	return CHECK_EQUALITY(0 , id) ;
}

uint8_t canFilter_19(uint32_t id)
{
	return CHECK_EQUALITY(0 , id) ;
}

//��������
uint8_t canFilter_20(uint32_t id)
{
	if(18655000 == (id & 0xFFFFFF00))  //FC״̬
	{
			return 1 ;
	}
	return 0 ;
	//return CHECK_EQUALITY(0x1814508C , id) ; 
}

uint8_t canFilter_21(uint32_t id)
{
	if(0x18665000 == (id & 0xFFFFFF00))        //��Ѳ���
	{
			return 1 ;
	}
	return 0 ;
//	return CHECK_EQUALITY(0x1815508C , id) ; 
}

uint8_t canFilter_22(uint32_t id)
{
	if(0x18675000 == (id & 0xFFFFFF00))       //FC���п��Ʋ���
	{
			return 1 ;
	}
	return 0 ;
//	return CHECK_EQUALITY(0x1816508C , id) ; //���ȡ��ķ����Ŀ���¶�
}

uint8_t canFilter_23(uint32_t id)
{
	if(0x18A15000 == (id & 0xFFFFFF00))       //DCDC����
	{
			return 1 ;
	}
	return 0 ;
//	return CHECK_EQUALITY(0x1817508C , id) ;
}

uint8_t canFilter_24(uint32_t id)      
{
	if(0x1800508c == (id & 0xFF00FFFF))
	{
			return 1 ;
	}
	return 0 ;
//	return CHECK_EQUALITY(0x1820508C , id) ; 
}

uint8_t canFilter_25(uint32_t id)
{
//	if(0x18215000 == (id & 0xFFFFFF00))
//	{
//			return 1 ;
//	}
	return 0 ;
//	return CHECK_EQUALITY(0x1821508C, id) ; //DCDCģ��4����
}

uint8_t canFilter_26(uint32_t id)
{
	return CHECK_EQUALITY(0x161068A0 , id) ; //li�������
}

uint8_t canFilter_27(uint32_t id)
{
	return CHECK_EQUALITY(0x1610A700 , id) ; 
}

uint8_t canFilter_28(uint32_t id)
{
	return CHECK_EQUALITY(0x1818A8B1 , id) ; //����
}

uint8_t canFilter_29(uint32_t id)
{
	return CHECK_EQUALITY(0x161099B9 , id) ;
}

uint8_t canFilter_30(uint32_t id)
{
	return CHECK_EQUALITY(0 , id) ; 
}

uint8_t canFilter_31(uint32_t id)
{
	return CHECK_EQUALITY(0 , id) ;//DC/DCģ��3 �ϴ�����
}

uint8_t canFilter_32(uint32_t id)
{
	return CHECK_EQUALITY(0 , id) ;//DC/DCģ��4 �ϴ�����
}

uint8_t canFilter_35(uint32_t id)         //�ڵ� ������
{
//	return CHECK_EQUALITY(0x1C0CFF8C , id) ;
		if(0x1CEDFF00 == (id & 0xFFFFFF00))
		{
				return 1 ;
		}
		return 0 ;
}

void canLoadSever(uint8_t index)
{
	j1939_receive_one_msg(&rxboxServer , &myStack.can_rx_box[index].RxMessage);
}

void canLoadProcess(uint8_t index)
{
 	j1939_receive_one_msg(&rxboxProcess , &myStack.can_rx_box[index].RxMessage);
}

void canLoadInternet(uint8_t index)
{
	j1939_receive_one_msg(&rxboxInternet , &myStack.can_rx_box[index].RxMessage); 
}

void canLoadOrder(uint8_t index)
{
	j1939_receive_one_msg(&rxboxOrder , &myStack.can_rx_box[index].RxMessage); 
}

void CAN_FilterTaskInit(void)
{
	//��������
		CAN_FilterInitTypeDef CAN_FilterInitStructure ;
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 0 ;
		CAN_FilterInitStructure.pfilter = canFilter_0 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 1 ;
		CAN_FilterInitStructure.pfilter = canFilter_1 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 2 ;
		CAN_FilterInitStructure.pfilter = canFilter_2 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);		

		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 3 ;
		CAN_FilterInitStructure.pfilter = canFilter_3 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);	
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 4 ;
		CAN_FilterInitStructure.pfilter = canFilter_4 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);	

		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 5 ;
		CAN_FilterInitStructure.pfilter = canFilter_5 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);	

		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 6 ;
		CAN_FilterInitStructure.pfilter = canFilter_6 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 7 ;
		CAN_FilterInitStructure.pfilter = canFilter_7 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);	
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 8 ;
		CAN_FilterInitStructure.pfilter = canFilter_8 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);	

		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 9 ;
		CAN_FilterInitStructure.pfilter = canFilter_9 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);	

		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 10 ;
		CAN_FilterInitStructure.pfilter = canFilter_10 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 11 ;
		CAN_FilterInitStructure.pfilter = canFilter_11 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);
	
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 12 ;
		CAN_FilterInitStructure.pfilter = canFilter_12 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);		
		

		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 13 ;
		CAN_FilterInitStructure.pfilter = canFilter_13 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);	
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 14 ;
		CAN_FilterInitStructure.pfilter = canFilter_14 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);	

		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 15 ;
		CAN_FilterInitStructure.pfilter = canFilter_15 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);	
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 16 ;
		CAN_FilterInitStructure.pfilter = canFilter_16 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);	
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 17 ;
		CAN_FilterInitStructure.pfilter = canFilter_17 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);	

		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 18 ;
		CAN_FilterInitStructure.pfilter = canFilter_18 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);	
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 19 ;
		CAN_FilterInitStructure.pfilter = canFilter_19 ;
		CAN_FilterInitStructure.pload   = canLoadSever ;
		CAN_FilterInit(&CAN_FilterInitStructure);	
		
		//��������
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 20 ;
		CAN_FilterInitStructure.pfilter = canFilter_20 ;
		CAN_FilterInitStructure.pload   = canLoadProcess ;
		CAN_FilterInit(&CAN_FilterInitStructure);
		

		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 21 ;
		CAN_FilterInitStructure.pfilter = canFilter_21 ;
		CAN_FilterInitStructure.pload   = canLoadProcess ;
		CAN_FilterInit(&CAN_FilterInitStructure);
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 22 ;
		CAN_FilterInitStructure.pfilter = canFilter_22 ;
		CAN_FilterInitStructure.pload   = canLoadProcess ;
		CAN_FilterInit(&CAN_FilterInitStructure);		

		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 23 ;
		CAN_FilterInitStructure.pfilter = canFilter_23 ;
		CAN_FilterInitStructure.pload   = canLoadProcess ;
		CAN_FilterInit(&CAN_FilterInitStructure);	
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 24 ;
		CAN_FilterInitStructure.pfilter = canFilter_24 ;
		CAN_FilterInitStructure.pload   = canLoadProcess ;
		CAN_FilterInit(&CAN_FilterInitStructure);	

		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 25 ;
		CAN_FilterInitStructure.pfilter = canFilter_25 ;
		CAN_FilterInitStructure.pload   = canLoadProcess ;
		CAN_FilterInit(&CAN_FilterInitStructure);
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 26 ;
		CAN_FilterInitStructure.pfilter = canFilter_26 ;
		CAN_FilterInitStructure.pload   = canLoadProcess ;
		CAN_FilterInit(&CAN_FilterInitStructure);		
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 27 ;
		CAN_FilterInitStructure.pfilter = canFilter_27 ;
		CAN_FilterInitStructure.pload   = canLoadProcess ;
		CAN_FilterInit(&CAN_FilterInitStructure);		
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 28 ;
		CAN_FilterInitStructure.pfilter = canFilter_28 ;
		CAN_FilterInitStructure.pload   = canLoadProcess ;
		CAN_FilterInit(&CAN_FilterInitStructure);		

		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 29 ;
		CAN_FilterInitStructure.pfilter = canFilter_29 ;
		CAN_FilterInitStructure.pload   = canLoadProcess ;
		CAN_FilterInit(&CAN_FilterInitStructure);		
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 30 ;
		CAN_FilterInitStructure.pfilter = canFilter_30 ;
		CAN_FilterInitStructure.pload   = canLoadProcess ;
		CAN_FilterInit(&CAN_FilterInitStructure);		
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 31 ;
		CAN_FilterInitStructure.pfilter = canFilter_31 ;
		CAN_FilterInitStructure.pload   = canLoadProcess ;
		CAN_FilterInit(&CAN_FilterInitStructure);		
		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 32 ;
		CAN_FilterInitStructure.pfilter = canFilter_32 ;
		CAN_FilterInitStructure.pload   = canLoadProcess ;
		CAN_FilterInit(&CAN_FilterInitStructure);		
		
		//��������		
		CAN_FilterInitStructure.pstack = &myStack ;
		CAN_FilterInitStructure.CAN_FilterNumber = 35 ;
		CAN_FilterInitStructure.pfilter = canFilter_35 ;
		CAN_FilterInitStructure.pload   = canLoadInternet ;
		CAN_FilterInit(&CAN_FilterInitStructure);		
}

void canTaskInit(void)
{
		CanInit(&myStack , BPS_250K , BPS_250K) ;  //Baud��IO init.
		
		//������������
		can_create_rxbox(&rxboxServer,RXBOX_SERVER_SIZE) ;	
	  can_create_rxbox(&rxboxProcess,RXBOX_PROCESS_SIZE) ;
		can_create_rxbox(&rxboxInternet,RXBOX_INTERNET_SIZE) ;	
//		can_create_rxbox(&rxboxOrder,RXBOX_ORDER_SIZE) ;	
	
		rxboxServer.rxcount = 0 ;
	
		CAN_FilterTaskInit();
}

//void PC_Control_Relay(uint8_t state)
//{
//		j1939_message sendMessage ;
//		sendMessage.channel 	= CAN2 ;
//		sendMessage.FF      	= 1 ;
//		sendMessage.LEN     	= 8 ;		
//		sendMessage.std_id.ID = 0x1816A8B0 ;
//		//sendMessage.std_id.pdu.source_address = sourAddress ;
//		sendMessage.mCANDF.DATA[0] = state;
//		sendMessage.mCANDF.DATA[1] = 0xFF;
//		sendMessage.mCANDF.DATA[2] = 0xFF;
//		sendMessage.mCANDF.DATA[3] = 0xFF;	
//		sendMessage.mCANDF.DATA[4] = 0xFF;
//		sendMessage.mCANDF.DATA[5] = 0xFF;
//		sendMessage.mCANDF.DATA[6] = 0xFF;
//		sendMessage.mCANDF.DATA[7] = 0xFF;
//		WriteCANSedCyBuf(CAN2 , &sendMessage);		
//}
	

/***************************************************************
**											PCָ�����
***************************************************************/
static void PC_Order_Control(const j1939_message revCANDATA)
{
		switch(revCANDATA.mCANDF.DATA[1])
		{
			case CONTROL_ONOFF : //��/�ػ�
				g_MCMS_system.msysOnOffOrder = revCANDATA.mCANDF.DATA[2] ;
				printf("Mode:0x%x,CONTROL_ONOFF:%d\n",CAN_MAINBORD_ADDRESS,revCANDATA.mCANDF.DATA[2]);
				break;
			case CONTROL_MODULE : //����ģʽ
				g_MCMS_system.msysControlMode = revCANDATA.mCANDF.DATA[2] ;
				printf("Mode:0x%x,CONTROL_MODULE:%d\n",CAN_MAINBORD_ADDRESS,revCANDATA.mCANDF.DATA[2]);
				break;
			case CONTROL_MAIN_VALVE1:
				FAMEN_1_Control(revCANDATA.mCANDF.DATA[2]);
				printf("Mode:0x%x,CONTROL_MAIN_VALVE1:%d\n",CAN_MAINBORD_ADDRESS,revCANDATA.mCANDF.DATA[2]);
				break;
			case CONTROL_MAIN_VALVE2:
				FAMEN_2_Control(revCANDATA.mCANDF.DATA[2]);
				printf("Mode:0x%x,CONTROL_MAIN_VALVE2:%d\n",CAN_MAINBORD_ADDRESS,revCANDATA.mCANDF.DATA[2]);
				break;
			case CONTROL_MAIN_VALVE3:
				FAMEN_3_Control(revCANDATA.mCANDF.DATA[2]);
				printf("Mode:0x%x,CONTROL_MAIN_VALVE3:%d\n",CAN_MAINBORD_ADDRESS,revCANDATA.mCANDF.DATA[2]);
				break;
			case CONTROL_MAIN_VALVE4:
				FAMEN_4_Control(revCANDATA.mCANDF.DATA[2]);
				printf("Mode:0x%x,CONTROL_MAIN_VALVE4:%d\n",CAN_MAINBORD_ADDRESS,revCANDATA.mCANDF.DATA[2]);
				break;			
			case CONTROL_MAIN_VALVE5:
				FAMEN_5_Control(revCANDATA.mCANDF.DATA[2]);
				printf("Mode:0x%x,CONTROL_MAIN_VALVE5:%d\n",CAN_MAINBORD_ADDRESS,revCANDATA.mCANDF.DATA[2]);
				break;						
			case CONTROL_MAIN_PWM1:  //���Ʒ���ת��
				PWM_Set_5(1 , revCANDATA.mCANDF.DATA[2]) ;
				printf("Mode:0x%x,CONTROL_MAIN_PWM1:%d\n",CAN_MAINBORD_ADDRESS,revCANDATA.mCANDF.DATA[2]);
				break;
			case CONTROL_MAIN_PWM2:  //���ƹķ��ת��
				PWM_Set_6(1 , revCANDATA.mCANDF.DATA[2]) ;
				printf("Mode:0x%x,CONTROL_MAIN_PWM2:%d\n",CAN_MAINBORD_ADDRESS,revCANDATA.mCANDF.DATA[2]);
				break;		
			case CONTROL_MAIN_BMS:
				PC_Control_Relay(revCANDATA.mCANDF.DATA[2]); //����BMS����
				printf("Mode:0x%x,CONTROL_MAIN_BMS:%d\n",CAN_MAINBORD_ADDRESS,revCANDATA.mCANDF.DATA[2]);
				break ;
			default:
				break;
		}
}

static void Ask_TP_PGN(const uint8_t index)
{
		j1939_message sendMessage ;
//		uint16_t i = 0 ;
	
		switch(index)
		{
			case READ_SYS_WARN : //�����ϴ��澯��������
//				printf("warn\n");
				if(0 == mMtPackIs_is_idling())
				{
						return ;
				}
				getMtPackp()->mMtPack.mPriority = 3 ;
				getMtPackp()->mMtPack.mPF 		  = 236 ;  //�����ʽ����ʼ��Ϊ0
				getMtPackp()->mMtPack.mDA       = J1939_GLOBAL_ADDRESS ;  
				getMtPackp()->mMtPack.mSA       = CAN_MAINBORD_ADDRESS ;
				getMtPackp()->mMtPack.mByteCount= 32;
				getMtPackp()->mMtPack.mNextPackNumber = 0 ; 

//				for(i = 0 ; i < getMtPackp()->mMtPack.mByteCount ;i++)
//				{
//						getMtPackp()->mMtPack.mData[i] = FLASHMEMORYMAP->mflashData[i] ;
//				}
				memcpy(getMtPackp()->mMtPack.mData,&FLASHMEMORYMAP->mflashData[FLASH_SYS_WARN_BASE],getMtPackp()->mMtPack.mByteCount);
				j1939_transmit(READ_SYS_WARN | 0xFE00);
				break;
			default:
				break;
		}
}

void canSeverHandlePart(j1939_message revCANDATA)
{
		j1939_message sendMessage ;
		uint8_t i = 0 ;
		switch(revCANDATA.std_id.ID)
				{
					case 0x801ff01:
						if(revCANDATA.mCANDF.DATA[0] == CAN_MAINBORD_ADDRESS)
						{
								PC_Order_Control(revCANDATA);
						}
						break;
					case 0x1012ff01:
						if(revCANDATA.mCANDF.DATA[0] == CAN_MAINBORD_ADDRESS)
						{
								Ask_TP_PGN(revCANDATA.mCANDF.DATA[1]);
						}
						break ;
					case 0xc02ff01:  //PC������DCDC ����
						sendMessage.channel 	= CAN2 ;
						sendMessage.FF      	= revCANDATA.FF ;
						sendMessage.LEN     	= revCANDATA.LEN ;		
						sendMessage.std_id.ID = 0xc3cff50 ;
						for(i = 0 ; i < revCANDATA.LEN  ; i++)
						{
								sendMessage.mCANDF.DATA[i] = revCANDATA.mCANDF.DATA[i];
						}
						WriteCANSedCyBuf(CAN2 , &sendMessage);	
						break;
					case 0xc3d5001: //PC������DCDC ����
						sendMessage.channel 	= CAN2 ;
						sendMessage.FF      	= revCANDATA.FF ;
						sendMessage.LEN     	= revCANDATA.LEN ;		
						sendMessage.std_id.ID = 0xc3d8c50 ;
//						printf("3:%x\n",revCANDATA.std_id.ID);
						for(i = 0 ; i < revCANDATA.LEN  ; i++)
						{
								sendMessage.mCANDF.DATA[i] = revCANDATA.mCANDF.DATA[i];
						}
						WriteCANSedCyBuf(CAN2 , &sendMessage);		
						break ;
						/*
					case 0xCEBFF03:   //���� ȼ�ذ�����
					case 0xCECFF03:	
						sendMessage.channel 	= CAN1 ;
						sendMessage.FF      	= revCANDATA.FF ;
						sendMessage.LEN     	= revCANDATA.LEN ;		
						sendMessage.std_id.ID = revCANDATA.std_id.ID ;
//						printf("3:%x\n",revCANDATA.std_id.ID);
						for(i = 0 ; i < revCANDATA.LEN  ; i++)
						{
								sendMessage.mCANDF.DATA[i] = revCANDATA.mCANDF.DATA[i];
						}
						WriteCANSedCyBuf(CAN1 , &sendMessage);							
						break ;
						*/
					case 0xCECFF01:   //���� ���ذ�����
						sendMessage.channel 	= CAN2 ;
						sendMessage.FF      	= revCANDATA.FF ;
						sendMessage.LEN     	= revCANDATA.LEN ;		
						sendMessage.std_id.ID = revCANDATA.std_id.ID ;
						for(i = 0 ; i < revCANDATA.LEN  ; i++)
						{
								sendMessage.mCANDF.DATA[i] = revCANDATA.mCANDF.DATA[i];
						}
						WriteCANSedCyBuf(CAN2 , &sendMessage);	
						//Add 2017��3��9�� 10:17:12						
						recPack.mSA = revCANDATA.std_id.pdu.source_address ;						
						recPack.mByteCount 			= revCANDATA.mCANDF.DATA[1] | revCANDATA.mCANDF.DATA[2] << 8;
						recPack.mNextPackNumber = revCANDATA.mCANDF.DATA[3] ;
						recPack.mDA 			 		  = revCANDATA.mCANDF.DATA[5] ;   //ID
						recPack.mPF 		   			= revCANDATA.mCANDF.DATA[6] ;   //��֡
						//	
//						printf("DA:%x\n" , recPack.mDA);
						break ;
					case 0xCEBFF01:
						sendMessage.channel 	= CAN2 ;
						sendMessage.FF      	= revCANDATA.FF ;
						sendMessage.LEN     	= revCANDATA.LEN ;		
						sendMessage.std_id.ID = revCANDATA.std_id.ID ;
						for(i = 0 ; i < revCANDATA.LEN  ; i++)
						{
								sendMessage.mCANDF.DATA[i] = revCANDATA.mCANDF.DATA[i];
						}
						WriteCANSedCyBuf(CAN2 , &sendMessage);	

						if(SET_SYS_WARN != recPack.mDA)  //����ϵͳ�澯��������,���˳�
						{
								break ;
						}
						//���
						for(i = 0 ; i < 7 ; i++)
						{
								recPack.mData[(revCANDATA.mCANDF.DATA[0] - 1) * 7 + i]     = revCANDATA.mCANDF.DATA[i + 1] ;	
						}
						
						if(recPack.mNextPackNumber == revCANDATA.mCANDF.DATA[0])
						{
								if(SET_SYS_WARN == recPack.mDA)
								{
//										printf("%x\n", recPack.mData[0]);
										writeFlashCache(FLASH_SYS_WARN_BASE,recPack.mData,recPack.mByteCount);
										writeFlashDownload();
								}
						}
						break ;
					default:
						break ;
				}		
}

//��������
void canSeverHandle(void)
{
		j1939_message revCANDATA ;
		j1939_message sendMessage ;
		uint8_t i = 0 ;
//		PARA_VALUE_16 tmp ;
		
		if(rxboxServer.rxcount > 1)
		{
				revCANDATA = messages_push_mail(&rxboxServer);
//				printf("31:%x\n",revCANDATA.std_id.ID);
				//��ַ, 0x8C - 0x8C+10 ֮�䣬 ת�ﵽ��һ��
				if(revCANDATA.std_id.pdu.source_address == CAN_PC_ADDRESS)
				{
						sendMessage.channel 	= CAN2 ;
						sendMessage.FF      	= revCANDATA.FF ;
						sendMessage.LEN     	= revCANDATA.LEN ;		
						sendMessage.std_id.ID = revCANDATA.std_id.ID ;
//						printf("3:%x\n",revCANDATA.std_id.ID);
						for(i = 0 ; i < revCANDATA.LEN  ; i++)
						{
								sendMessage.mCANDF.DATA[i] = revCANDATA.mCANDF.DATA[i];
						}
						WriteCANSedCyBuf(CAN2 , &sendMessage);
				}
				//��ַ, 0x00 - 0x00+10 ֮�䣬 ת����һ��
				else if(revCANDATA.std_id.pdu.pduspecific == CAN_PC_ADDRESS)
				{
						sendMessage.channel 	= CAN1 ;
						sendMessage.FF      	= revCANDATA.FF ;
						sendMessage.LEN     	= revCANDATA.LEN ;		
						sendMessage.std_id.ID = revCANDATA.std_id.ID ;
//						printf("1:%x\n",revCANDATA.std_id.ID);
						for(i = 0 ; i < revCANDATA.LEN  ; i++)
						{
								sendMessage.mCANDF.DATA[i] = revCANDATA.mCANDF.DATA[i];
						}
						WriteCANSedCyBuf(CAN1 , &sendMessage);						
				}	
				else if(0xFF == revCANDATA.std_id.pdu.pduspecific)  //��֡�㲥
				{
						if(revCANDATA.std_id.pdu.source_address >= CAN_FCBORD_ADDRESS && revCANDATA.std_id.pdu.source_address <= CAN_FCBORD_ADDRESS + 10)
						{
								sendMessage.channel 	= CAN1 ;
								sendMessage.FF      	= revCANDATA.FF ;
								sendMessage.LEN     	= revCANDATA.LEN ;		
								sendMessage.std_id.ID = revCANDATA.std_id.ID ;
		//						printf("3:%x\n",revCANDATA.std_id.ID);
								for(i = 0 ; i < revCANDATA.LEN  ; i++)
								{
										sendMessage.mCANDF.DATA[i] = revCANDATA.mCANDF.DATA[i];
								}
								WriteCANSedCyBuf(CAN1 , &sendMessage);
						}
				}
				
				if(CAN_MAINBORD_ADDRESS == revCANDATA.std_id.pdu.pduspecific || 0xFF == revCANDATA.std_id.pdu.pduspecific)
				{
						canSeverHandlePart(revCANDATA) ;						
				}
		}
}

//��������
void canProcessHandle(void)
{
		j1939_message revCANDATA ;
		j1939_message sendMessage ;
		PARA_VALUE_16 tmp ;
//		uint8_t i = 0 ;
		if(rxboxProcess.rxcount > 1)
		{
				revCANDATA = messages_push_mail(&rxboxProcess) ;
			
				if(CAN_MAINBORD_ADDRESS == revCANDATA.std_id.pdu.pduspecific)
				{
					#if 0
						if(0x1815508C != revCANDATA.std_id.ID)
						{
								sendMessage.channel 	= CAN1 ;   //����ͨ����������Ҫ
								sendMessage.FF      	= revCANDATA.FF ;
								sendMessage.LEN     	= revCANDATA.LEN ;		 
								sendMessage.std_id.ID = revCANDATA.std_id.ID ; //revCANDATA.std_id.ID
								sendMessage.std_id.pdu.pduspecific = CAN_PC_ADDRESS ;    //����Ŀ���ַ
								//sendMessage.std_id.pdu.source_address = CAN_MAINBORD_ADDRESS ;
								for(i = 0 ; i < revCANDATA.LEN  ; i++)
								{
										sendMessage.mCANDF.DATA[i] = revCANDATA.mCANDF.DATA[i];
								}
								WriteCANSedCyBuf(CAN1 , &sendMessage);					
						}
						#endif
					switch(revCANDATA.std_id.ID)
					{
					case 0x1815508C: //����ѹ��,����Ũ��
						sendMessage.channel 	= CAN1 ;						//����ͨ����������Ҫ
						sendMessage.FF      	= revCANDATA.FF ;
						sendMessage.LEN     	= revCANDATA.LEN ;		
						sendMessage.std_id.ID = revCANDATA.std_id.ID;
						sendMessage.std_id.pdu.pduspecific = CAN_PC_ADDRESS ;    					//����Ŀ���ַ
						sendMessage.std_id.pdu.source_address = CAN_MAINBORD_ADDRESS ;    //����Ŀ���ַ
//						sendMessage.mCANDF.u16Word.word[0] = revCANDATA.mCANDF.u16Word.word[0];
//						sendMessage.mCANDF.u16Word.word[1] = g_FCMS_runpara[0].mH_ND.flaotword * 100 ;
						g_FCMS_runpara[0].mH_YL.flaotword = revCANDATA.mCANDF.u16Word.word[0] / 100.0 ;
						break ;
					case 0x1866508C:
					case 0x1866508D:
					case 0x1866508E:
					case 0x1866508F:
					case 0x18665090:
					case 0x18665091:
					case 0x18665092:
					case 0x18665093:
							g_FCMS_runpara[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].mF_V.flaotword 	 = revCANDATA.mCANDF.u16Word.word[0]  / 10.0;
							g_FCMS_runpara[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].mF_A.flaotword 	 = revCANDATA.mCANDF.u16Word.word[1]  / 10.0 - 1500 ;
							g_FCMS_runpara[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].mF_CKWD.flaotword = revCANDATA.mCANDF.u16Word.word[2] / 10.0 - 50;
							g_FCMS_runpara[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].mF_JKWD.flaotword = revCANDATA.mCANDF.u16Word.word[3] / 10.0 - 50;	
							break;
					case 0x1867508C:
					case 0x1867508D:
					case 0x1867508E:
					case 0x1867508F:
					case 0x18675090:
					case 0x18675091:
					case 0x18675092:
					case 0x18675093:    //���ȡ��ķ����Ŀ���¶� Ԥ����
							g_FCMS_runpara[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].mF_FJ   = revCANDATA.mCANDF.DATA[0];
							g_FCMS_runpara[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].mF_Purg = revCANDATA.mCANDF.DATA[1];
					
							tmp.Bits.DATA0 = revCANDATA.mCANDF.DATA[2];
							tmp.Bits.DATA1 = revCANDATA.mCANDF.DATA[3];
							g_FCMS_runpara[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].mF_MBWD.flaotword = tmp.word / 10.0 ;
							tmp.Bits.DATA0 = revCANDATA.mCANDF.DATA[4];
							tmp.Bits.DATA1 = revCANDATA.mCANDF.DATA[5];					
							g_FCMS_runpara[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].mH_YL.flaotword = tmp.word / 100.0 - 1;
							break;
					case 0x1865508C:
					case 0x1865508D:
					case 0x1865508E:
					case 0x1865508F:
					case 0x18655090:
					case 0x18655091:
					case 0x18655092:
					case 0x18655093:
							g_FCMS_system[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].msysState          = revCANDATA.mCANDF.DATA[0];
							g_FCMS_system[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].msysControlMode    = revCANDATA.mCANDF.DATA[1] ;
							g_FCMS_system[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].msysWarn.Bits.DATA0= revCANDATA.mCANDF.DATA[4] ;
							g_FCMS_system[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].msysWarn.Bits.DATA1= revCANDATA.mCANDF.DATA[5] ;
							g_FCMS_system[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].msysWarn.Bits.DATA2= revCANDATA.mCANDF.DATA[6] ;
							g_FCMS_system[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].msysWarn.Bits.DATA3= revCANDATA.mCANDF.DATA[7] ;						
							break ;					
					case 0x18A1508C:
					case 0x18A1508D:
					case 0x18A1508E:
					case 0x18A1508F:
					case 0x18A15090:
					case 0x18A15091:
					case 0x18A15092:
					case 0x18A15093: // DC/DC �����ѹ���������
							g_DCMS_runpara[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].mD_state = revCANDATA.mCANDF.DATA[0] ;
							g_DCMS_runpara[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].mD_Warn  = revCANDATA.mCANDF.DATA[1] ;
							g_DCMS_runpara[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].mD_V = revCANDATA.mCANDF.u16Word.word[1] / 10.0;
							g_DCMS_runpara[revCANDATA.std_id.pdu.source_address - CAN_FCBORD_ADDRESS].mD_A = revCANDATA.mCANDF.u16Word.word[2] / 10.0;		
							break;
					default:
							break ;
					}
				}
				
#if 1			
				//��ַ��can1����ת����can2
//				if(3 == revCANDATA.std_id.pdu.pduspecific)
//				{
//						sendMessage.channel 	= CAN2 ;   //����ͨ����������Ҫ
//						sendMessage.FF      	= revCANDATA.FF ;
//						sendMessage.LEN     	= revCANDATA.LEN ;		 
//						sendMessage.std_id.ID = revCANDATA.std_id.ID ; //revCANDATA.std_id.ID
//						for(i = 0 ; i < revCANDATA.LEN  ; i++)
//						{
//								sendMessage.mCANDF.DATA[i] = revCANDATA.mCANDF.DATA[i];
//						}
//						WriteCANSedCyBuf(CAN2 , &sendMessage);
//				}
				
				//�ⲿ����Э��Ĳ���
				switch(revCANDATA.std_id.ID)
				{
					case 0x161068A0:
						tmp.Bits.DATA1 = revCANDATA.mCANDF.DATA[4] ;
						tmp.Bits.DATA0 = revCANDATA.mCANDF.DATA[5] ;
						g_FCMS_BMSpara.mBMS_A = tmp.word * 0.1 - 1500;							
						break ;
					case 0x161099B9:
						tmp.Bits.DATA1 = revCANDATA.mCANDF.DATA[5] ;
						tmp.Bits.DATA0 = revCANDATA.mCANDF.DATA[6] ;
						g_FCMS_BMSpara.mBMS_SOC = tmp.word * 0.1;								
						break ;
					case 0x1818A8B1:
						tmp.Bits.DATA1 = revCANDATA.mCANDF.DATA[0] ;
						tmp.Bits.DATA0 = revCANDATA.mCANDF.DATA[1] ;
						g_FCMS_BMSpara.mBMS_V = tmp.word * 0.1;			
//						g_FCMS_BMSpara.mRelayState = revCANDATA.mCANDF.DATA[6] ;	
//						g_FCMS_BMSpara.mBMS_State  = revCANDATA.mCANDF.DATA[7] ;	
						break ;
					case 0x1818A8C1: //���ϴ���
						g_FCMS_BMSpara.mBMS_Warn = revCANDATA.mCANDF.DATA[7] ;						
						break;	
					case 0x1610A700: //﮵���¶�
						tmp.Bits.DATA1 = revCANDATA.mCANDF.DATA[4] ;
						tmp.Bits.DATA0 = revCANDATA.mCANDF.DATA[5] ;
						g_FCMS_BMSpara.mBMS_Temp = tmp.word * 0.1 - 50 ;
						break ;
					/*
					case 0x161068A0: //﮵�ص���
							tmp.Bits.DATA1 = revCANDATA.mCANDF.DATA[4] ;
							tmp.Bits.DATA0 = revCANDATA.mCANDF.DATA[5] ;
							g_FCMS_BMSpara.mBMS_A = tmp.word * 0.1 - 1500;
							break;
					case 0x1818A8B1: //﮵�ص�ѹ
							tmp.Bits.DATA1 = revCANDATA.mCANDF.DATA[0] ;
							tmp.Bits.DATA0 = revCANDATA.mCANDF.DATA[1] ;
							g_FCMS_BMSpara.mBMS_V = tmp.word * 0.1;			
							g_FCMS_BMSpara.mBMS_P = g_FCMS_BMSpara.mBMS_V * g_FCMS_BMSpara.mBMS_A ;
							break;
					case 0x161099B9: //﮵��soc
							tmp.Bits.DATA1 = revCANDATA.mCANDF.DATA[5] ;
							tmp.Bits.DATA0 = revCANDATA.mCANDF.DATA[6] ;
							g_FCMS_BMSpara.mBMS_SOC = tmp.word * 0.1;		
							break;
					case 0x1818A8C1: //���ϴ���
							g_FCMS_BMSpara.mBMS_Warn = revCANDATA.mCANDF.DATA[7] ;						
							break;
					case 0x1610A700: //﮵���¶�
							tmp.Bits.DATA1 = revCANDATA.mCANDF.DATA[4] ;
							tmp.Bits.DATA0 = revCANDATA.mCANDF.DATA[5] ;
							g_FCMS_BMSpara.mBMS_Temp = tmp.word * 0.1 - 50 ;
							break ;
					*/
					/*
					case 0x18140204:  //DCDC3 ����
							sendMessage.channel 	= CAN1 ;						//����ͨ����������Ҫ
							sendMessage.FF      	= revCANDATA.FF ;
							sendMessage.LEN     	= revCANDATA.LEN ;		
							sendMessage.std_id.ID = 0x18140204;
					
							sendMessage.mCANDF.u16Word.word[0] = revCANDATA.mCANDF.u16Word.word[0] ;
							sendMessage.mCANDF.u16Word.word[1] = revCANDATA.mCANDF.u16Word.word[1];
							sendMessage.mCANDF.u16Word.word[2] = revCANDATA.mCANDF.u16Word.word[2] ;
							sendMessage.mCANDF.u16Word.word[3] = revCANDATA.mCANDF.u16Word.word[3] ;
							WriteCANSedCyBuf(CAN1 , &sendMessage);				
							break;
					case 0x18150204:  //DCDC4 ����
							sendMessage.channel 	= CAN1 ;						//����ͨ����������Ҫ
							sendMessage.FF      	= revCANDATA.FF ;
							sendMessage.LEN     	= revCANDATA.LEN ;		
							sendMessage.std_id.ID = 0x18150204;
							sendMessage.mCANDF.u16Word.word[0] = revCANDATA.mCANDF.u16Word.word[0] ;
							sendMessage.mCANDF.u16Word.word[1] = revCANDATA.mCANDF.u16Word.word[1];
							sendMessage.mCANDF.u16Word.word[2] = revCANDATA.mCANDF.u16Word.word[2] ;
							sendMessage.mCANDF.u16Word.word[3] = revCANDATA.mCANDF.u16Word.word[3] ;
							g_DCMS_runpara[1].mD_V = revCANDATA.mCANDF.u16Word.word[1] / 10.0;
							g_DCMS_runpara[1].mD_A = revCANDATA.mCANDF.u16Word.word[2] / 10.0;							
							WriteCANSedCyBuf(CAN1 , &sendMessage);								
							break ;
							*/
					default:
							break;
				}
#endif							
		}
}

//��������
void canInternetHandle(void)
{
		j1939_message sendMessage ;
		j1939_message revCANDATA ;
		uint8_t i = 0 ;
		if(rxboxInternet.rxcount > 1)
		{
				revCANDATA = messages_push_mail(&rxboxInternet) ;
//				canSendHandle(msg); 
				sendMessage.channel 	= CAN1 ;   //����ͨ����������Ҫ
				sendMessage.FF      	= revCANDATA.FF ;
				sendMessage.LEN     	= revCANDATA.LEN ;		 
				sendMessage.std_id.ID = revCANDATA.std_id.ID ; //revCANDATA.std_id.ID
				//sendMessage.std_id.pdu.pduspecific = CAN_PC_ADDRESS ;    //����Ŀ���ַ
				//sendMessage.std_id.pdu.source_address = CAN_MAINBORD_ADDRESS ;
				for(i = 0 ; i < revCANDATA.LEN  ; i++)
				{
						sendMessage.mCANDF.DATA[i] = revCANDATA.mCANDF.DATA[i];
				}
				WriteCANSedCyBuf(CAN1 , &sendMessage);								
		}
}

void sys_Warn_send(void)
{
		j1939_message sendMessage ;
		sendMessage.channel 	= CAN1 ;
		sendMessage.FF      	= 1 ;
		sendMessage.LEN     	= 8 ;		
		sendMessage.std_id.ID = 0x18140150 ;
		sendMessage.std_id.pdu.source_address = CAN_MAINBORD_ADDRESS ;
		sendMessage.mCANDF.DATA[0] = g_MCMS_system.msysState;
		sendMessage.mCANDF.DATA[1] = g_MCMS_system.msysControlMode ;
		sendMessage.mCANDF.DATA[2] = g_MCMS_Power.mTempMode;
		sendMessage.mCANDF.DATA[3] = 0xFF;	
		sendMessage.mCANDF.DATA[4] = g_MCMS_system.msysWarn.Bits.DATA0;
		sendMessage.mCANDF.DATA[5] = g_MCMS_system.msysWarn.Bits.DATA1;
		sendMessage.mCANDF.DATA[6] = g_MCMS_system.msysWarn.Bits.DATA2;
		sendMessage.mCANDF.DATA[7] = g_MCMS_system.msysWarn.Bits.DATA3;
		WriteCANSedCyBuf(CAN1 , &sendMessage);
}

//		tmp = g_FCMS_BMSpara.mBMS_V * 10 ;
//		sendMessage.mCANDF.u16Word.word[0] 	  = tmp ;
//		tmp = (g_FCMS_BMSpara.mBMS_A + 1500) * 10 ;
//		sendMessage.mCANDF.u16Word.word[1] 		= tmp;
//		tmp = g_FCMS_BMSpara.mBMS_SOC * 100 ;
//		sendMessage.mCANDF.u16Word.word[2] 		=  tmp;
//		tmp = g_FCMS_BMSpara.mBMS_Temp + 100 ;
//		sendMessage.mCANDF.DATA[6]    =  tmp; //���������¶ȣ�����λ
//		sendMessage.mCANDF.DATA[7]		= g_FCMS_BMSpara.mBMS_Warn ;


void BMS_Data_Send(void)
{	
		j1939_message sendMessage ;
		uint16_t tmp = 0 ;

		sendMessage.channel 	= CAN1 ;						//����ͨ����������Ҫ
		sendMessage.FF      	= 1;
		sendMessage.LEN     	= 8 ;		
		sendMessage.std_id.ID = 0x18510105;
		sendMessage.std_id.pdu.source_address = CAN_BMS_ADDRESS ;
	
		//debug
		g_FCMS_BMSpara.mBMS_V    = 314.3;
		g_FCMS_BMSpara.mBMS_A    = 15.4;
		g_FCMS_BMSpara.mBMS_SOC  = 60;
		g_FCMS_BMSpara.mBMS_Temp = 25.3 ;
		//end
	
		tmp = g_FCMS_BMSpara.mBMS_V * 10 ;
		sendMessage.mCANDF.DATA[0] 	  = tmp ;
		sendMessage.mCANDF.DATA[1] 	  = tmp >> 8 ;
		tmp = (g_FCMS_BMSpara.mBMS_A + 1500) * 10 ;  //
		sendMessage.mCANDF.DATA[2] 	  = tmp ;
		sendMessage.mCANDF.DATA[3] 	  = tmp >> 8 ;
		tmp = g_FCMS_BMSpara.mBMS_SOC * 10 ;
		sendMessage.mCANDF.DATA[4] 	  = tmp ;
		sendMessage.mCANDF.DATA[5] 	  = tmp >> 8 ;
		tmp = (g_FCMS_BMSpara.mBMS_Temp  + 50)* 10 ;
		sendMessage.mCANDF.DATA[6]    = tmp; //���������¶ȣ�����λ
		sendMessage.mCANDF.DATA[7]		= tmp >> 8 ;
		WriteCANSedCyBuf(CAN1 , &sendMessage);	
		
		sendMessage.channel 	= CAN1 ;						//����ͨ����������Ҫ
		sendMessage.FF      	= 1;
		sendMessage.LEN     	= 8 ;		
		sendMessage.std_id.ID = 0x18520105;
		sendMessage.std_id.pdu.source_address = CAN_BMS_ADDRESS ;
		sendMessage.mCANDF.DATA[0] 	  = g_FCMS_BMSpara.mBMS_State ;
		sendMessage.mCANDF.DATA[1] 	  = g_FCMS_BMSpara.mRelayState ;
		sendMessage.mCANDF.DATA[2] 	  = 0 ;
		sendMessage.mCANDF.DATA[3] 	  = 0 ;
		sendMessage.mCANDF.DATA[4] 	  = 0 ;
		sendMessage.mCANDF.DATA[5] 	  = 0 ;
		sendMessage.mCANDF.DATA[6]    = 0 ; 
		sendMessage.mCANDF.DATA[7]		= g_FCMS_BMSpara.mBMS_Warn ; //�澯
		WriteCANSedCyBuf(CAN1 , &sendMessage);	
}

static void FC_State_Send(uint8_t sourAddress)
{
		j1939_message sendMessage ;
		sendMessage.channel 	= CAN1 ;
		sendMessage.FF      	= 1 ;
		sendMessage.LEN     	= 8 ;		
		sendMessage.std_id.ID = 0x1865508C ;
		sendMessage.std_id.pdu.pduspecific    =  CAN_PC_ADDRESS ;
		sendMessage.std_id.pdu.source_address =  sourAddress + CAN_FCBORD_ADDRESS ;
		sendMessage.mCANDF.DATA[0] = g_FCMS_system[sourAddress].msysState;
		sendMessage.mCANDF.DATA[1] = g_FCMS_system[sourAddress].msysControlMode ;
		sendMessage.mCANDF.DATA[2] = 0xFF;
		sendMessage.mCANDF.DATA[3] = 0xFF ;
		sendMessage.mCANDF.DATA[4] = g_FCMS_system[sourAddress].msysWarn.Bits.DATA0;
		sendMessage.mCANDF.DATA[5] = g_FCMS_system[sourAddress].msysWarn.Bits.DATA1;
		sendMessage.mCANDF.DATA[6] = g_FCMS_system[sourAddress].msysWarn.Bits.DATA2;
		sendMessage.mCANDF.DATA[7] = g_FCMS_system[sourAddress].msysWarn.Bits.DATA3;
		WriteCANSedCyBuf(CAN1 , &sendMessage);		
}

static void FC_Data_Send(uint8_t sourAddress)
{
		j1939_message sendMessage ;
		sendMessage.channel 	= CAN1 ;
		sendMessage.FF      	= 1 ;
		sendMessage.LEN     	= 8 ;		
		sendMessage.std_id.ID = 0x1866508C ;
		sendMessage.std_id.pdu.pduspecific    =  CAN_PC_ADDRESS ;
		sendMessage.std_id.pdu.source_address =  sourAddress + CAN_FCBORD_ADDRESS ;
		sendMessage.mCANDF.u16Word.word[0] = (g_FCMS_runpara[sourAddress].mF_V.flaotword) * 10 ;
		sendMessage.mCANDF.u16Word.word[1] = (g_FCMS_runpara[sourAddress].mF_A.flaotword + 1500)  * 10 ;
		sendMessage.mCANDF.u16Word.word[2] = (g_FCMS_runpara[sourAddress].mF_CKWD.flaotword + 50) * 10;
		sendMessage.mCANDF.u16Word.word[3] = (g_FCMS_runpara[sourAddress].mF_JKWD.flaotword + 50) * 10 ;
		WriteCANSedCyBuf(CAN1 , &sendMessage);
}

static void FC_FJ_Send(uint8_t sourAddress)
{
		PARA_VALUE_16 tmp ;
		j1939_message sendMessage ;
		sendMessage.channel 	= CAN1 ;
		sendMessage.FF      	= 1 ;
		sendMessage.LEN     	= 8 ;		 
		sendMessage.std_id.ID = 0x1867508C ;
		sendMessage.std_id.pdu.pduspecific    =  CAN_PC_ADDRESS ;
		sendMessage.std_id.pdu.source_address =  sourAddress + CAN_FCBORD_ADDRESS ;
		sendMessage.mCANDF.DATA[0] = g_FCMS_runpara[sourAddress].mF_FJ;
		sendMessage.mCANDF.DATA[1] = g_FCMS_runpara[sourAddress].mF_Purg ;
		tmp.word = g_FCMS_runpara[sourAddress].mF_MBWD.flaotword * 10 ;
		sendMessage.mCANDF.DATA[2] = tmp.Bits.DATA0;
		sendMessage.mCANDF.DATA[3] = tmp.Bits.DATA1;
		tmp.word = (g_FCMS_runpara[sourAddress].mH_YL.flaotword + 1) * 100 ;
		sendMessage.mCANDF.DATA[4] = tmp.Bits.DATA0;
		sendMessage.mCANDF.DATA[5] = tmp.Bits.DATA1;
		sendMessage.mCANDF.DATA[6] = 0xFF;
		sendMessage.mCANDF.DATA[7] = 0xFF;
		WriteCANSedCyBuf(CAN1 , &sendMessage);		
}

static void MC_HQ_Send(void)
{
		j1939_message sendMessage ;
		sendMessage.channel 	= CAN1 ;						//����ͨ����������Ҫ
		sendMessage.FF      	= 1 ;
		sendMessage.LEN     	= 8 ;		
		sendMessage.std_id.ID = 0x18150150;
		sendMessage.std_id.pdu.pduspecific = CAN_PC_ADDRESS ;    					//����Ŀ���ַ
		sendMessage.std_id.pdu.source_address = CAN_MAINBORD_ADDRESS ;    //����Ŀ���ַ

		sendMessage.mCANDF.u16Word.word[0] = g_MCMS_runpara.mH_YL.flaotword * 10 ; //������ѹ
		sendMessage.mCANDF.u16Word.word[1] = g_MCMS_runpara.mH_ND.flaotword * 100 ;
		sendMessage.mCANDF.u16Word.word[2] = (g_MCMS_runpara.mF_HJWD.flaotword + 50) * 10;
		WriteCANSedCyBuf(CAN1 , &sendMessage);	
}

void sys_SOC_Info_send(void)
{
		j1939_message sendMessage ;
	  PARA_VALUE_16 sendDate;
		sendMessage.channel 	= CAN1 ;
		sendMessage.FF      	= 1 ;
		sendMessage.LEN     	= 8 ;		
		sendMessage.std_id.ID = 0x18160150 ;
		sendMessage.std_id.pdu.pduspecific    =  CAN_PC_ADDRESS ;
		sendMessage.std_id.pdu.source_address =  CAN_MAINBORD_ADDRESS ;
	
		//debug
		g_RateSOC.SOC_rate   = 0.14;
		g_RateSOC.ChargeTime = 150;
		//end
	  sendDate.word = g_RateSOC.SOC_rate*100+1000;
		sendMessage.mCANDF.DATA[0] = sendDate.Bits.DATA0;
		sendMessage.mCANDF.DATA[1] = sendDate.Bits.DATA1 ;
		sendMessage.mCANDF.DATA[2] = g_RateSOC.LoadGrade;
	  sendDate.word = g_RateSOC.ChargeTime*10+15000;			//��ŵ�ʱ��(min)
		sendMessage.mCANDF.DATA[3] = sendDate.Bits.DATA0 ;
		sendMessage.mCANDF.DATA[4] = sendDate.Bits.DATA1;
		sendMessage.mCANDF.DATA[5] = 0xFF;
		sendMessage.mCANDF.DATA[6] = 0xFF;
		sendMessage.mCANDF.DATA[7] = 0xFF;
		WriteCANSedCyBuf(CAN1 , &sendMessage);
}

void DCDC_Data_Send(uint8_t sourAddress)
{
		j1939_message sendMessage ;
		PARA_VALUE_16 tmp ;
		sendMessage.channel 	= CAN1 ;						//����ͨ����������Ҫ
		sendMessage.FF      	= CAN_EXTENDED_FRAME ;
		sendMessage.LEN     	= 8 ;		
		sendMessage.std_id.ID = 0x18A1508C ;
		sendMessage.std_id.pdu.pduspecific    =  CAN_PC_ADDRESS ;
		sendMessage.std_id.pdu.source_address =  sourAddress + CAN_FCBORD_ADDRESS ;
		sendMessage.mCANDF.DATA[0] = g_DCMS_runpara[sourAddress].mD_state ;	//����
		sendMessage.mCANDF.DATA[1] = g_DCMS_runpara[sourAddress].mD_Warn;  	//״̬
		tmp.word = g_DCMS_runpara[sourAddress].mD_V * 10; 
		sendMessage.mCANDF.DATA[2] = tmp.Bits.DATA0 ; 		
		sendMessage.mCANDF.DATA[3] = tmp.Bits.DATA1 ;
		tmp.word = g_DCMS_runpara[sourAddress].mD_A * 10;
		sendMessage.mCANDF.DATA[4] = tmp.Bits.DATA0 ;    
		sendMessage.mCANDF.DATA[5] = tmp.Bits.DATA1 ;
		sendMessage.mCANDF.DATA[6] = 0xff ; 
		sendMessage.mCANDF.DATA[7] = 0xff ;
		WriteCANSedCyBuf(CAN1 , &sendMessage);	
}

static void HJ_Mode_Send(void)
{
		j1939_message sendMessage ;
		sendMessage.channel 	= CAN2 ;						//����ͨ����������Ҫ
		sendMessage.FF      	= 1 ;
		sendMessage.LEN     	= 1 ;		
		sendMessage.std_id.ID = 0x18158C50;
//		sendMessage.std_id.pdu.pduspecific    = CAN_FCBORD_ADDRESS ;      //����Ŀ���ַ
//		sendMessage.std_id.pdu.source_address = CAN_MAINBORD_ADDRESS ;    //����Ŀ���ַ
		sendMessage.mCANDF.DATA[0] = g_MCMS_Power.mTempMode;
		WriteCANSedCyBuf(CAN2 , &sendMessage);	
}

void canDataSend(void)
{
		uint8_t i = 0 ;
	
		sys_Warn_send();     //ϵͳ�澯
//	
		BMS_Data_Send();     //﮵������
		sys_SOC_Info_send(); //SOC�㷨
//	
		MC_HQ_Send();        //��������
		OSTimeDlyHMSM(0, 0, 0, 200);
	
		for(i = 0 ; i < FC_MODE; i++)
		{
				FC_State_Send(i); //ȼ������
				FC_Data_Send(i);	 //ȼ������
				FC_FJ_Send(i);    //ȼ������		
				DCDC_Data_Send(i);    //DC���ݷ���
				OSTimeDlyHMSM(0, 0, 0, 200);
		}
		
//		HJ_Mode_Send(); 

}

