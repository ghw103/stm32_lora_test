#include "stm32f10x.h"
#include "GPIO_STM32F10x.h"





#define TOUCH_KEY1_GPIO_PORT		GPIOB
#define TOUCH_KEY1_GPIO_NUM   	1
#define TOUCH_KEY1_GPIO_PIN   	GPIO_Pin_1

 
/**
  * @brief  Configures the touch key io.
  * @param  None
  * @retval None
  */
	void KeyConfig(void)
	{
		GPIO_PinConfigure(TOUCH_KEY1_GPIO_PORT,TOUCH_KEY1_GPIO_NUM,GPIO_IN_PULL_DOWN,GPIO_MODE_OUT2MHZ);
	}
	
	
	/**
    * @brief  get the key1 status.
    * @param  None
    * @retval None
    */
	uint8_t Key1San(void)
	{
		return GPIO_ReadInputDataBit(TOUCH_KEY1_GPIO_PORT,TOUCH_KEY1_GPIO_PIN);
		
	}