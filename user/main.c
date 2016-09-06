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
	//参考网址：http://blog.csdn.net/iamlvshijie/article/details/9249545
	ErrorStatus HSEStartUpStatus; 
	RCC_DeInit();     //设置RCC寄存器重新设置为默认值
	RCC_HSEConfig (RCC_HSE_ON);//打开外部高速时钟晶振（HSE）
	HSEStartUpStatus = RCC_WaitForHSEStartUp();//等待外部时钟晶振稳定工作
	if(HSEStartUpStatus == SUCCESS)//SUCCESS：HSE晶振稳定且就绪
	{
		//FLASH_PrefetchBufferCmd(ENABLE);          //开启FLASH的预取功能
		//FLASH_SetLatency(FLASH_Latency_2);      //FLASH延长两个周期
		RCC_HCLKConfig(RCC_SYSCLK_Div1);//设置AHB时钟不分频
		RCC_PCLK2Config(RCC_HCLK_Div1); //设置APB2时钟不分频
		RCC_PCLK1Config(RCC_HCLK_Div2); //设置APB1时钟二分频 对应的定时器的时钟为2倍频
//	FLASH_SetLatency(FLASH_Latency_2);//设置FLASH存储器延时周期数，FLASH_Latency_2 2延时周期
//	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//选择FLASH预取指缓存的模式，预取指缓存使能
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);//设置PLL  11.0592五倍频=55.296。
		RCC_PLLCmd(ENABLE); //PLL时钟使能
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource()!=0x08);//等待工作。
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