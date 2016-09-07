 /******************** (C) COPYRIGHT 2016 LORY ********************
 * File Name          : touch_key.c
 * Author             : zzr
 * Date First Issued  : 09/06/2016
 * Description        :  触摸按键 测试程序 
		三种方式  key 1 使用 rh6030芯片  key2 使用 pwm 输入查看 key脚电压变化  也可参考ti官网 
 ********************************************************************************
 * History:
 * 
 * 02/05/2007: V0.1
 ********************************************************************************
 * .
 *******************************************************************************/
#include "stm32f10x.h"
#include "GPIO_STM32F10x.h"
#include "bsp.h"

 

#define TOUCH_KEY1_GPIO_PORT		GPIOB
#define TOUCH_KEY1_GPIO_NUM   	1
#define TOUCH_KEY1_GPIO_PIN   	GPIO_Pin_1
	void Key2Config(void);
 
/**
  * @brief  Configures the touch key io.
  * @param  None
  * @retval None
  */
	void Key1Config(void)
	{
		GPIO_PinConfigure(TOUCH_KEY1_GPIO_PORT,TOUCH_KEY1_GPIO_NUM,GPIO_IN_FLOATING,GPIO_MODE_INPUT);
		Key2Config();
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
	
	
	/**
    * @brief  Configures the Key  Config.
    * @param  None
    * @retval None
    */
	void Key2Config(void)
	{
		//250k pwm out
		TIM4_CH1_PWM_Init(3,7); 
		
	}