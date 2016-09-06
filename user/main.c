/******************** (C) COPYRIGHT 2016 LORY ********************
* File Name          : xxx.c
* Author             : zzr
* Date First Issued  : 09/06/2016
* Description        :  
********************************************************************************
* History:
* 
*  
********************************************************************************
* .
*******************************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "bsp.h"
#include "sys_tick_delay.h" 

static void prvSetupHardware( void );

int  main(void)
{
	prvSetupHardware();
	LED_Config();
	Sys_delay_init();
	while(1)
	{
		Sys_delay_ms(200);
		LED1_TOGGLE;
		
	}
	return 0;
}


 /**
   * @brief  Configures the different system clocks.
   * @param  None
   * @retval None
   */
static void prvSetupHardware( void )
{
	//�ο���ַ��http://blog.csdn.net/iamlvshijie/article/details/9249545
	ErrorStatus HSEStartUpStatus; 
	RCC_DeInit();     //����RCC�Ĵ�����������ΪĬ��ֵ
	RCC_HSEConfig (RCC_HSE_ON);//���ⲿ����ʱ�Ӿ���HSE��
	HSEStartUpStatus = RCC_WaitForHSEStartUp();//�ȴ��ⲿʱ�Ӿ����ȶ�����
	if(HSEStartUpStatus == SUCCESS)//SUCCESS��HSE�����ȶ��Ҿ���
	{
		//FLASH_PrefetchBufferCmd(ENABLE);          //����FLASH��Ԥȡ����
		//FLASH_SetLatency(FLASH_Latency_2);      //FLASH�ӳ���������
		RCC_HCLKConfig(RCC_SYSCLK_Div1);//����AHBʱ�Ӳ���Ƶ
		RCC_PCLK2Config(RCC_HCLK_Div1); //����APB2ʱ�Ӳ���Ƶ
		RCC_PCLK1Config(RCC_HCLK_Div2); //����APB1ʱ�Ӷ���Ƶ ��Ӧ�Ķ�ʱ����ʱ��Ϊ2��Ƶ
//	FLASH_SetLatency(FLASH_Latency_2);//����FLASH�洢����ʱ��������FLASH_Latency_2 2��ʱ����
//	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//ѡ��FLASHԤȡָ�����ģʽ��Ԥȡָ����ʹ��
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);//����PLL  11.0592�屶Ƶ=55.296��
		RCC_PLLCmd(ENABLE); //PLLʱ��ʹ��
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource()!=0x08);//�ȴ�������
	}
	
	 RCC_LSICmd(ENABLE);
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
 
 

	/* Enable GPIOA, GPIOB,   and AFIO clocks */
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE );

	/* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );


	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	/* Configure HCLK clock as SysTick clock source. */
	//SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK ); 
}