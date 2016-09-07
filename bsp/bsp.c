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
/**
  * @brief  Configures //PWM输出初始化
  * @param  //arr：自动重装值
			//psc：时钟预分频数
  * @retval None
  */


void TIM4_CH1_PWM_Init(u16 arr,u16 psc)
{  
	 GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	 TIM_DeInit(TIM4);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外设时钟使能
	                                                                     	

   //设置该引脚为复用输出功能,输出TIM4 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period =  arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = arr>>1; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	TIM_CtrlPWMOutputs(TIM4,ENABLE);	//MOE 主输出使能	

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH1预装载使能	 
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIM1
 
   
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

