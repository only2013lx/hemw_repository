

#include "adcHard.h"
#include "adcDriver.h"
#include "adcTask.h"

#include <stdio.h>
#include "ram.h"

static void HYL_Measuring_ADC(const uint8_t channel)
{
		uint32_t tmpValue = 0 ;
		tmpValue = tmpValue ;
		tmpValue = MiddleAverageValueFilter(channel); 
		//printf("%d\n" , tmpValue);
		g_MCMS_runpara.mH_YL.flaotword = tmpValue * 0.0003 - 0.2158 ;
}



//void ADC_Init(void)
//{
//		SPIInit() ;
//}

void ADC_Task(void)
{
		#if 0
		HYL_Measuring_ADC(HYLChannel) ;
		HND_Measuring_ADC(HNDChannel) ;

//		FV_Measuring_ADC(FVChannel) ;
//		FA_Measuring_ADC(FAChannel) ;
//		FCKWD_Measuring_ADC(FCKWDChannel) ;
		T_Measuring_ADC(FCKWDChannel , &g_MCMS_runpara.mF_HJWD.flaotword) ;
		#else
		g_MCMS_runpara.mH_YL.flaotword = 134.5 ;
		g_MCMS_runpara.mH_ND.flaotword = 0.04 ;
		g_MCMS_runpara.mF_HJWD.flaotword  = 22.5 ;
	
	
		#endif
	
}



