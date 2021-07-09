#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
 #include "LED.h"
 /*
 Keil: MDK5.10.0.2
MCU:stm32f103c8
硬件接法：
GY-53---STM32
1、GY-56_RX---STM32_TX,STM32复位将发送A5 65 01 0B 给模块
2、STM32_TX---FT232,STM32将数据上传给上位机
3、GY-56_TX---STM32_RX，接收模块数据
软件说明:
该程序采用串口方式获取模块数据，波特率9600

注：中断函数位于stm32f10x_it.c
联系方式：
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_X;
  
  /* 4个抢占优先级，4个响应优先级 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /*抢占优先级可打断中断级别低的中断*/
	/*响应优先级按等级执行*/
	NVIC_X.NVIC_IRQChannel = USART1_IRQn;//中断向量
  NVIC_X.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
  NVIC_X.NVIC_IRQChannelSubPriority = 0;//响应优先级
  NVIC_X.NVIC_IRQChannelCmd = ENABLE;//使能中断响应
  NVIC_Init(&NVIC_X);
}
void send_com(u8 function,u8 value )
{
	u8 bytes[4]={0};
	bytes[0]=0xa5;
	bytes[1]=function;//功能指令
	bytes[2]=value;//指令值
	USART_Send(bytes,4);//发送帧头、功能字节、校验和
}
typedef  struct
{
	uint16_t distance;
	uint8_t   mode;
	uint8_t   temp;
}sensor;
int main(void)
{
  u8 sum=0,i=0;
	int16_t data=0;
  sensor GY56;
	
	delay_init(72);
	NVIC_Configuration();
	Usart_Int(9600);
	delay_ms(500);//等待模块初始化完成
	send_com(0x65,1);//发送连续输出指令
	while(1)
	{
		if(Receive_ok)//串口接收完毕
		{
			for(sum=0,i=0;i<(re_Buf_Data[3]+4);i++)
			sum+=re_Buf_Data[i];
			if(sum==re_Buf_Data[i])//校验和判断
			{
				GY56.distance=re_Buf_Data[4]<<8|re_Buf_Data[5];
				GY56.mode=re_Buf_Data[6];
				GY56.temp=re_Buf_Data[7];		
				send_3out(&re_Buf_Data[4],4,0x15);//上传给上位机
			}
			Receive_ok=0;//处理数据完毕标志
		}
	}
}
