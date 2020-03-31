#include "usart.h"
#include "string.h"

void Usart_Int(uint32_t BaudRatePrescaler)
{
	 SCON  = 0x50;//1位起始位，8位数据位，1位停止位，异步，使能接收
	 TMOD |= 0x20;//定时器1，工作方式2，8位自动重装计数值
	//例9600，28800/9600=3,TH1=253=FD；
     TH1   =256-(28800/BaudRatePrescaler); 
	 TL1   =256-(28800/BaudRatePrescaler);
     TCON |=0x40;//启动定时器1
	 IE   |=0x90;//打开串口中断，MCU总中断
}
//发送一个字节
uint8_t send_ok=0,Calibration=0;
void USART_send_byte(uint8_t Tx_data)
{
	while(send_ok);//等待发送缓存为空
 	SBUF = Tx_data;
	send_ok=1;//缓存标志置1
}
//发送Length-1个数据+1个数据累加和
void USART_Send(uint8_t *Buffer, uint8_t Length)
{
	uint8_t i=0;
	while(i<Length)
	{
		if(i<(Length-1))
		Buffer[Length-1]+=Buffer[i];
		USART_send_byte(Buffer[i++]);
	}
}
void send_3out(uint8_t *Data,uint8_t length,uint8_t send)
{
	uint8_t TX_DATA[13]={0},i=0,k=0;
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=send;//功能字节
	TX_DATA[i++]=length;//数据个数
	for(k=0;k<length;k++)//存入数据到缓存TX_DATA
	{
		TX_DATA[i++]=(uint8_t)Data[k];
	}
	USART_Send(TX_DATA,length+5);	
}
//发送一帧数据
void send_out(int16_t *Data,uint8_t length,uint8_t send)
{
	uint8_t TX_DATA[13],i=0,k=0;
	memset(TX_DATA,0,(2*length+5));//清空发送数据
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=send;//功能字节
	TX_DATA[i++]=2*length;//数据个数
	for(k=0;k<length;k++)//存入数据到缓存TX_DATA数组
	{
		TX_DATA[i++]=(uint16_t)Data[k]>>8;
		TX_DATA[i++]=(uint16_t)Data[k];
	}
	USART_Send(TX_DATA,(2*length+5));//发送一帧数据	
}



