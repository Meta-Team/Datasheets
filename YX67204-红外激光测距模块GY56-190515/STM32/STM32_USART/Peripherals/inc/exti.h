#ifndef __EXTI_H
#define __EXTI_H
#include "stm32f10x.h"
void Exti_Int(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,uint8_t GPIO_PortSource);
#endif
