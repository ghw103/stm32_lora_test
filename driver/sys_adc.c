 
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
	
    DMA_DeInit(DMA1_Channel1);//��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMA����ADC����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;//DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize  = AD_BUFSIZE;//DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//������ 
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ��?xӵ�и����ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel1,&DMA_InitStructure);//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ�� 
		DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);   //open interrupt  use for filter
    DMA_Cmd(DMA1_Channel1,ENABLE); //����DMAͨ��?
		
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
    GPIO_Init(GPIOA,&GPIO_InitStructure); //Ĭ���ٶ�Ϊ����
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode                  =   ADC_Mode_Independent;  //����ģʽ
    ADC_InitStructure.ADC_ScanConvMode          =  	ENABLE;  // DISABLE;    //������ͨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode    =   ENABLE;      //����ת��
    ADC_InitStructure.ADC_ExternalTrigConv      =   ADC_ExternalTrigConv_None; //ת������������
    ADC_InitStructure.ADC_DataAlign             =   ADC_DataAlign_Right;   //�Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel          =   2;       //ɨ��ͨ����
    ADC_Init(ADC1,&ADC_InitStructure);
    ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 1,ADC_SampleTime_55Cycles5); //
		ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 2,ADC_SampleTime_55Cycles5); //
 //	ADC_RegularChannelConfig(ADC1,ADC_Channel_16, 2,ADC_SampleTime_55Cycles5);
		ADC_DMACmd(ADC1,ENABLE);//
    ADC_Cmd  (ADC1,ENABLE);             //ʹ�ܻ���ʧ��ָ����ADC
//		ADC_TempSensorVrefintCmd(ENABLE);
    //ADC_SoftwareStartConvCmd(ADC1,ENABLE);//ʹ�ܻ���ʧ��ָ����ADC�����ת����������
    ADC_ResetCalibration(ADC1);//��λָ����ADC1��У׼�Ĵ���
    while(ADC_GetResetCalibrationStatus(ADC1));//��ȡADC1��λУ׼�Ĵ�����״̬,����״̬��ȴ
    ADC_StartCalibration(ADC1);//��ʼָ��ADC1��У׼״̬
    while(ADC_GetCalibrationStatus(ADC1)); //��ȡָ��ADC1
		
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
 
