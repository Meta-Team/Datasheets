#include <reg52.h>
#include "usart.h"
#include "iic.h"  
/*
硬件接法：
GY-56----C51
SCL---P3^6
SDA---P3^7
C51---FT232
TX ---RX
RX ---TX
软件说明：

注：
	中断函数位于stc_it.c
联系方式：
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/

void delay(unsigned int x)
{
	while(x--);
}
int main(void)
{

	Usart_Int(9600);
 	while(1)
	{
	   	uint16_t diatance=0;
	    requestRange((0xe1),&diatance);		 //读取距离数据
		takeRangeReading(0xe0);				 //发送测量命令

		send_out(&diatance,1,0x45);		 //发向上位机
		delay(60000);	
	}
}
