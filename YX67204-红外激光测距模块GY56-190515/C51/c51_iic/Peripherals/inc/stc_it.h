#ifndef __STC_IT_H__
#define __STC_IT_H__
#include <reg52.h>
#define uint32_t unsigned long
#define uint16_t unsigned int
#define uint8_t unsigned char
void EXTI1_IRQHandler(void);
void USART_IRQHandler(void);
#endif