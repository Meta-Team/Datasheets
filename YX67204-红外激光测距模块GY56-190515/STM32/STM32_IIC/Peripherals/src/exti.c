#include "exti.h"
#include <math.h>
//B端口外部上升沿中断设置
//input：GPIO_Pin_x 要设置的中断引脚
void Exti_Int(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,uint8_t GPIO_PortSource)
{
	GPIO_InitTypeDef GPIO_Exti;
	EXTI_InitTypeDef EXTI_X;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	/*中断引脚I/O设置*/
	GPIO_Exti.GPIO_Pin=GPIO_Pin_x;
	GPIO_Exti.GPIO_Mode=GPIO_Mode_IPD;//上拉输入
	GPIO_Init(GPIOx, &GPIO_Exti);
	
	/*将端口B和中断线映射起来*/
	GPIO_EXTILineConfig(GPIO_PortSource, sqrt(GPIO_Pin_x));
	/*设置中断线*/
	EXTI_X.EXTI_Line=(uint32_t)GPIO_Pin_x;
	EXTI_X.EXTI_Mode=EXTI_Mode_Interrupt;//中断模式
	EXTI_X.EXTI_Trigger=EXTI_Trigger_Rising;//上升沿中断
	EXTI_X.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_X);
}
