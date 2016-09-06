#include "stm32f10x.h"
#include "bsp.h"
#include "GPIO_STM32F10x.h"

#define LED1_GPIO_PORT		GPIOB
#define LED1_GPIO_NUM   	8
#define LED1_GPIO_PIN   	GPIO_Pin_8

#define LED2_GPIO_PORT		GPIOB
#define LED2_GPIO_NUM   	9
#define LED2_GPIO_PIN   	GPIO_Pin_9

/**
  * @brief  Configures the Board Led.
  * @param  None
  * @retval None
  */
void LED_Config(void)
{
	
		GPIO_PortClock(LED1_GPIO_PORT,ENABLE); 
	
		GPIO_PinConfigure(LED1_GPIO_PORT,LED1_GPIO_NUM,GPIO_OUT_PUSH_PULL,GPIO_MODE_OUT2MHZ);
		GPIO_PinConfigure(LED2_GPIO_PORT,LED2_GPIO_NUM,GPIO_OUT_PUSH_PULL,GPIO_MODE_OUT2MHZ);
	
}

