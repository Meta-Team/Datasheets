#ifndef __EXTI_H__
#define __EXTI_H__
#include <reg52.h>
#define uint32_t unsigned long
#define uint16_t unsigned int
#define uint8_t unsigned char
void Exti_Int(bit n,uint8_t Mode);
extern uint8_t ACM_BUF[36],BIT;
#endif