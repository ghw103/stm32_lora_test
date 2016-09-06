#ifndef __BSP_H
#define __BSP_H



#define LED1_GPIO_PORT		GPIOB
#define LED1_GPIO_NUM   	8
#define LED1_GPIO_PIN   	GPIO_Pin_8

#define LED2_GPIO_PORT		GPIOB
#define LED2_GPIO_NUM   	9
#define LED2_GPIO_PIN   	GPIO_Pin_9


#define LED1_TOGGLE    		LED1_GPIO_PORT->ODR ^= LED1_GPIO_PIN
#define LED2_TOGGLE    		LED2_GPIO_PORT->ODR ^= LED2_GPIO_PIN

void LED_Config(void);







#endif