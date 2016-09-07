 
#include "stm32f10x_dma.h"
 
#include "stm32f10x_adc.h"  
#include "sys_adc.h"
  
  #define AD_BUFSIZE 32
u16 AD_Value[AD_BUFSIZE] ;
u16 gAdcFilter1, gAdcFilter2 ;
void sys_adc_dma_config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);          // Enable the DMA Interrupt 
	
    DMA_DeInit(DMA1_Channel1);//½«DMAµÄÍ¨µÀ1¼Ä´æÆ÷ÖØÉèÎªÈ±Ê¡Öµ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMAÍâÉèADC»ùµØÖ·
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;//DMAÄÚ´æ»ùµØÖ·
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //ÄÚ´æ×÷ÎªÊý¾Ý´«ÊäµÄÄ¿µÄµØ
    DMA_InitStructure.DMA_BufferSize  = AD_BUFSIZE;//DMAÍ¨µÀµÄDMA»º´æµÄ´óÐ¡
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//ÍâÉèµØÖ·¼Ä´æÆ÷²»±ä
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //ÄÚ´æµØÖ·¼Ä´æÆ÷µÝÔö
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//Êý¾Ý¿í¶ÈÎª16Î»
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//Êý¾Ý¿í¶ÈÎª16Î»
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//¹¤×÷ÔÚ 
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAÍ¨µÀ?xÓµÓÐ¸ßÓÅÏÈ¼¶
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMAÍ¨µÀxÃ»ÓÐÉèÖÃÎªÄÚ´æµ½ÄÚ´æ´«Êä
    DMA_Init(DMA1_Channel1,&DMA_InitStructure);//¸ù¾ÝDMA_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯DMAµÄÍ¨µÀ 
		DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);   //open interrupt  use for filter
    DMA_Cmd(DMA1_Channel1,ENABLE); //Æô¶¯DMAÍ¨µÀ?
		
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

 void sys_adc_init(void)
{
    ADC_InitTypeDef  ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1| GPIO_Pin_0;
  //  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA,&GPIO_InitStructure); //Ä¬ÈÏËÙ¶ÈÎªÁ½Õ×
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode                  =   ADC_Mode_Independent;  //¶ÀÁ¢Ä£Ê½
    ADC_InitStructure.ADC_ScanConvMode          =  	ENABLE;  // DISABLE;    //Á¬Ðø¶àÍ¨µÀÄ£Ê½
    ADC_InitStructure.ADC_ContinuousConvMode    =   ENABLE;      //Á¬Ðø×ª»»
    ADC_InitStructure.ADC_ExternalTrigConv      =   ADC_ExternalTrigConv_None; //×ª»»²»ÊÜÍâ½ç¾ö¶¨
    ADC_InitStructure.ADC_DataAlign             =   ADC_DataAlign_Right;   //ÓÒ¶ÔÆë
    ADC_InitStructure.ADC_NbrOfChannel          =   2;       //É¨ÃèÍ¨µÀÊý
    ADC_Init(ADC1,&ADC_InitStructure);
    ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 1,ADC_SampleTime_55Cycles5); //
		ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 2,ADC_SampleTime_55Cycles5); //
 //	ADC_RegularChannelConfig(ADC1,ADC_Channel_16, 2,ADC_SampleTime_55Cycles5);
		ADC_DMACmd(ADC1,ENABLE);//
    ADC_Cmd  (ADC1,ENABLE);             //Ê¹ÄÜ»òÕßÊ§ÄÜÖ¸¶¨µÄADC
//		ADC_TempSensorVrefintCmd(ENABLE);
    //ADC_SoftwareStartConvCmd(ADC1,ENABLE);//Ê¹ÄÜ»òÕßÊ§ÄÜÖ¸¶¨µÄADCµÄÈí¼þ×ª»»Æô¶¯¹¦ÄÜ
    ADC_ResetCalibration(ADC1);//¸´Î»Ö¸¶¨µÄADC1µÄÐ£×¼¼Ä´æÆ÷
    while(ADC_GetResetCalibrationStatus(ADC1));//»ñÈ¡ADC1¸´Î»Ð£×¼¼Ä´æÆ÷µÄ×´Ì¬,ÉèÖÃ×´Ì¬ÔòµÈ´
    ADC_StartCalibration(ADC1);//¿ªÊ¼Ö¸¶¨ADC1µÄÐ£×¼×´Ì¬
    while(ADC_GetCalibrationStatus(ADC1)); //»ñÈ¡Ö¸¶¨ADC1
		
		sys_adc_dma_config();
}

 
 //u16 adc_filter[3];

 
//Temperature= (1.42 - ADC_Value*3.3/4096)*1000/4.35 + 25
void filter(void)
{
	u32  sum1 = 0;
	u32 sum2 = 0;
 
   u8 i;
   for(i=0;i<(AD_BUFSIZE>>1);i++)
   {
	   sum1 +=	 AD_Value[2*i] ;
		 sum2 += 	 AD_Value[2*i+1];   
   }
   gAdcFilter1   = (u16) (sum1/(AD_BUFSIZE>>1)) ;
	 gAdcFilter2	 = (u16) (sum2/(AD_BUFSIZE>>1)) ;
}
u16 CurrDataCounterEnd = 0;
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
	{
		DMA_Cmd(DMA1_Channel1,DISABLE);
		 CurrDataCounterEnd=DMA_GetCurrDataCounter(DMA1_Channel1);
		DMA_ClearITPendingBit(DMA1_IT_TC1);
		filter();		
		DMA_SetCurrDataCounter(DMA1_Channel1,AD_BUFSIZE);
		DMA_Cmd(DMA1_Channel1,ENABLE);
	}
}
 
