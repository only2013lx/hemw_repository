
#ifndef  _H_ADC_TASK_H
#define  _H_ADC_TASK_H


typedef enum xx
{
    reserve1Channel = 0, 
		HYLChannel, 
		HNDChannel, 
		FAChannel, 
		FVChannel, 
		reserve2Channel, 
		FCKWDChannel,
		FJKWDChannel
}channel;



extern void ADC_Task(void) ;
extern void ADC_Init(void) ;


//static void HYL_Measuring_ADC(const uint8_t channel);
//static void HND_Measuring_ADC(const uint8_t channel);
//static void FV_Measuring_ADC(const uint8_t channel);
//static void FA_Measuring_ADC(const uint8_t channel);
//static void T_Measuring_ADC(const uint8_t channel , float *T_Current);

#endif
