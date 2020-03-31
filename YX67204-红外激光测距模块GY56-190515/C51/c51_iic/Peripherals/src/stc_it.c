#include "stc_it.h"
#include "usart.h"

//外部中断1
uint8_t stata=0;
void EXTI1_IRQHandler(void)interrupt 2
{
}
//串口中断
void USART_IRQHandler(void)interrupt 4
{
	static uint8_t i=0,rebuf[3]={0};
	uint8_t sum=0;
    if(TI)//发送完成标志
	{
	  TI=0;//清发送完成标志
	  send_ok=0;//缓存标志置0 
	}
	if(RI)//接收完成标志
	{
		rebuf[i++]=SBUF;
		RI=0;//清中断接收标志
		if(!(rebuf[0]==0xa5))//判断帧头
		i=0;
		if(i==3)
		{
			sum=rebuf[0]+rebuf[1];
			if(sum==rebuf[2])
			{
				if(rebuf[1]==0xbb)//校准指令
				{
					Calibration=1;
				 
				}
				if(rebuf[1]==0x51)//读数据
				{
					Calibration=0;
					
				}
			}
			i=0;
		}
	
	}

}
