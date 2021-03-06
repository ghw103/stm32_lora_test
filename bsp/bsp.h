#ifndef __BSP_H
#define __BSP_H

#include "stm32f10x.h"

#define LED1_GPIO_PORT		GPIOB
#define LED1_GPIO_NUM   	8
#define LED1_GPIO_PIN   	GPIO_Pin_8

#define LED2_GPIO_PORT		GPIOB
#define LED2_GPIO_NUM   	9
#define LED2_GPIO_PIN   	GPIO_Pin_9

#define LED1_ON		GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF  GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_TOGGLE    		LED1_GPIO_PORT->ODR ^= LED1_GPIO_PIN

#define LED2_ON		GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF  GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_TOGGLE    		LED2_GPIO_PORT->ODR ^= LED2_GPIO_PIN




void LED_Config(void);
void TIM4_CH1_PWM_Init(u16 arr,u16 psc);






#endif