#include "exti.h"
uint8_t ACM_BUF[36]={0},BIT=0;
/*
input:n;外部中断源设置，0-外部中断0，1-外部中断1
input:Mode；外部中断触发方式，0-低电平触发，1-下降沿触发
*/
void Exti_Int(bit n,uint8_t Mode)
{
	if (n)
	{
	  TCON|=(uint8_t)(Mode<<2);//外部中断1中断方式设置
	  IE  |=(uint8_t)(0x80|(1<<2));//允许外部中断1中断，开启总中断
	}
	else
	{
	  TCON|=(uint8_t)Mode;//外部中断0中断方式设置
	  IE  |=(uint8_t)(0x80|1);//允许外部中断0中断，开启总中断
	}
}
