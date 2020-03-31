#include "spi.h"
void Spi_Int(void)
{
	CS=1;
	SCK=1;
	MOSI=1;
	MISO=1;	
}
/*
1：
要求spi速度小于400Khz，该程序采用的晶振为11.0592M，则机器周期为
12/11.0592us，Spi_RW发送一bit数据和接收一bit数据共有5个语句
就算一个语句一个机器周期，5个语句需要约5.4us，即1000000/5.4约为185K
小于400Khz，符合要求；
2：
spi的CPOL=1，即时钟空闲状态为高电平
3：
spi的CPHA=1，即时钟相位为1，第二个时钟跳变沿采样
4:
因为CPIO=1，CPHA=1，所以第一个跳变沿是下降沿，第二个跳变沿为上升沿，
则在第一个下降沿时，主机输出引脚MOSI开始输出，从机其实也是在第一个下
降沿准备好MOSI，所以主机在下降沿后一段时间MISO可以采样（即上升沿前采样）
*/

uint8_t Spi_RW(uint8_t Data)
{
	unsigned char i;
	SCK = 1;
   	for(i=0;i<8;i++) 
   	{ 
	 MOSI=(Data & 0x80);/*写 1 bit，从高往低写，时钟前沿输出*/ 
	 Data = (Data << 1); //数据向左移一位，更新数据
	 SCK = 0;  //时钟拉低，第一个跳变沿，此时数据还未被从机采样  
     Data |= MISO; //从MISO读取1 bit,存放在Data最低位 ，时钟后沿采样 
	 SCK = 1;/*CPIO=1，第二个跳变沿*/      		  	
	}
    return(Data); 
}
void Spi_write_buf(uint8_t add, uint8_t *pBuf, uint8_t num)
{
	uint8_t i=0;
	CS=0;
	Spi_RW(add);
	while(i<num)
	Spi_RW(pBuf[i++]);
	CS=1;
}

void Spi_read_buf(uint8_t add, uint8_t *pBuf, uint8_t num)
{
	uint8_t i=0;
	CS=0;
	Spi_RW(add);
	while(i<num)
    {
	pBuf[i++]=Spi_RW(0);
    }
	CS=1;
}
