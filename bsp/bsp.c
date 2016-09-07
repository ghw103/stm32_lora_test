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
  * @brief  Configures //PWM�����ʼ��
  * @param  //arr���Զ���װֵ
			//psc��ʱ��Ԥ��Ƶ��
  * @retval None
  */


void TIM4_CH1_PWM_Init(u16 arr,u16 psc)
{  
	 GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	 TIM_DeInit(TIM4);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	                                                                     	

   //���ø�����Ϊ�����������,���TIM4 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period =  arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = arr>>1; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	TIM_CtrlPWMOutputs(TIM4,ENABLE);	//MOE �����ʹ��	

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM1
 
   
}

 
/**********************************************************
* �������� ---> ��ȡCPU 96λΨһID
* ��ڲ��� ---> none
* ������ֵ ---> 
* ����˵�� ---> none
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

