#include "IIC.h"
#include "delay.h"
 void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
		/* 使能与 I2C有关的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );  

	 /* PC3-I2C_SCL、PC5-I2C_SDA*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}
void delay_1us(u8 x)//粗略延时
{
	u8 i=10;
	x=i*x;
	while(x--);
}
////////IIC起始函数//////////
/*
IIC起始:当SCL处于高电平期间，SDA由高电平变成低电平出现一个下降沿，然后SCL拉低
*/
u8 I2C_Start(void)
{
		SDA_H; 
		delay_1us(5);	//延时保证时钟频率低于100K，以便从机识别
		SCL_H;
		delay_1us(5);//延时保证时钟频率低于100K，以便从机识别
		if(!SDA_read) return 0;//SDA线为低电平则总线忙,退出
		SDA_L;   //SCL处于高电平的时候，SDA拉低
		delay_1us(5);
	  if(SDA_read) return 0;//SDA线为高电平则总线出错,退出
		SCL_L;
	  delay_1us(5);
	  return 1;
}
//**************************************
//IIC停止信号
/*
IIC停止:当SCL处于高电平期间，SDA由低电平变成高电平出现一个上升沿
*/
//**************************************
void I2C_Stop(void)
{
    SDA_L;
		SCL_L;
		delay_1us(5);
		SCL_H;
		delay_1us(5);
		SDA_H;//当SCL处于高电平期间，SDA由低电平变成高电平             //延时
}
//**************************************
//IIC发送应答信号
//入口参数:ack (0:ACK 1:NAK)
/*
应答：当从机接收到数据后，向主机发送一个低电平信号
先准备好SDA电平状态，在SCL高电平时，主机采样SDA
*/
//**************************************
void I2C_SendACK(u8 i)
{
    if(1==i)
			SDA_H;	             //准备好SDA电平状态，不应答
    else 
			SDA_L;  						//准备好SDA电平状态，应答 	
	  SCL_H;                    //拉高时钟线
    delay_1us(5);                 //延时
    SCL_L ;                  //拉低时钟线
    delay_1us(5);    
} 
///////等待从机应答////////
/*
当本机(主机)发送了一个数据后，等待从机应答
先释放SDA，让从机使用，然后采集SDA状态
*/
/////////////////
u8 I2C_WaitAck(void) 	 //返回为:=1有ACK,=0无ACK
{
	uint16_t i=0;
	SDA_H;	        //释放SDA
	SCL_H;         //SCL拉高进行采样
	while(SDA_read)//等待SDA拉低
	{
		i++;      //等待计数
		if(i==500)//超时跳出循环
		break;
	}
	if(SDA_read)//再次判断SDA是否拉低
	{
		SCL_L; 
		return RESET;//从机应答失败，返回0
	}
  delay_1us(5);//延时保证时钟频率低于100K，
	SCL_L;
	delay_1us(5); //延时保证时钟频率低于100K，
	return SET;//从机应答成功，返回1
}
//**************************************
//向IIC总线发送一个字节数据
/*
一个字节8bit,当SCL低电平时，准备好SDA，SCL高电平时，从机采样SDA
*/
//**************************************
void I2C_SendByte(u8 dat)
{
  u8 i;
	SCL_L;//SCL拉低，给SDA准备
  for (i=0; i<8; i++)         //8位计数器
  {
		if(dat&0x80)//SDA准备
		SDA_H;  
		else 
		SDA_L;
    SCL_H;                //拉高时钟，给从机采样
    delay_1us(5);        //延时保持IIC时钟频率，也是给从机采样有充足时间
    SCL_L;                //拉低时钟，给SDA准备
    delay_1us(5); 		  //延时保持IIC时钟频率
		dat <<= 1;          //移出数据的最高位  
  }					 
}
//**************************************
//从IIC总线接收一个字节数据
//**************************************
u8 I2C_RecvByte()
{
    u8 i;
    u8 dat = 0;
    SDA_H;//释放SDA，给从机使用
    delay_1us(1);         //延时给从机准备SDA时间            
    for (i=0; i<8; i++)         //8位计数器
    { 
		  dat <<= 1;
			
      SCL_H;                //拉高时钟线，采样从机SDA
     
		  if(SDA_read) //读数据    
		   dat |=0x01;      
       delay_1us(5);     //延时保持IIC时钟频率		
       SCL_L;           //拉低时钟线，处理接收到的数据
       delay_1us(5);   //延时给从机准备SDA时间
    } 
    return dat;
}
//**************************************
//向IIC设备写入一个字节数据
//**************************************
u8 Single_WriteI2C_byte(u8 Slave_Address,u8 REG_Address,u8 data)
{
	  if(I2C_Start()==0)  //起始信号
		{I2C_Stop(); return RESET;}           

    I2C_SendByte(Slave_Address);   //发送设备地址+写信号
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(REG_Address);    //内部寄存器地址，
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(data);       //内部寄存器数据，
	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		
		I2C_Stop();   //发送停止信号
		
		return SET;
}
//**************************************
//从IIC设备读取一个字节数据
//**************************************
u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length)
{
 if(I2C_Start()==0)  //起始信号
		{I2C_Stop(); return RESET;}          
	 
	I2C_SendByte(Slave_Address);    //发送设备地址+写信号
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;} 
	
	I2C_SendByte(REG_Address);     //发送存储单元地址
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;} 
	
	if(I2C_Start()==0)  //起始信号
			{I2C_Stop(); return RESET;}            

	I2C_SendByte(Slave_Address+1);  //发送设备地址+读信号
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
	
	while(length-1)
	{
		*REG_data++=I2C_RecvByte();       //读出寄存器数据
		I2C_SendACK(0);               //应答
		length--;
	}
	*REG_data=I2C_RecvByte();  
	I2C_SendACK(1);     //发送停止传输信号
	I2C_Stop();                    //停止信号
	return SET;
}
