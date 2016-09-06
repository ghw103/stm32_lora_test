#include "stm32f10x.h"
#include "bsp.h"
#include "GPIO_STM32F10x.h"



#define STM32_UNIQUEID_ADDR   0x1FFFF7E8


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



/**********************************************************
* 函数功能 ---> 读取CPU 96位唯一ID
* 入口参数 ---> none
* 返回数值 ---> 
* 功能说明 ---> none
**********************************************************/
/**
  * @brief  get the  96 bit id from mcu's inter flash
  * @param   point to Device_Serial
  * @retval None
  */
void GetSTM32ID(uint32_t *Device_Serial)
{
	//uint32_t Device_Serial0,Device_Serial1,Device_Serial2;
	*Device_Serial++ = *(uint32_t*)(STM32_UNIQUEID_ADDR);      //12 Bytes Serial Number
  *Device_Serial++ = *(uint32_t*)(STM32_UNIQUEID_ADDR + 4);
  *Device_Serial++ = *(uint32_t*)(STM32_UNIQUEID_ADDR + 8);	
}

