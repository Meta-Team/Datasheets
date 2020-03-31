#include "spi.h"
/*
input:BaudRatePrescaler
#define SPI_BaudRatePrescaler_2         ((uint16_t)0x0000)
#define SPI_BaudRatePrescaler_4         ((uint16_t)0x0008)
#define SPI_BaudRatePrescaler_8         ((uint16_t)0x0010)
#define SPI_BaudRatePrescaler_16        ((uint16_t)0x0018)
#define SPI_BaudRatePrescaler_32        ((uint16_t)0x0020)
#define SPI_BaudRatePrescaler_64        ((uint16_t)0x0028)
#define SPI_BaudRatePrescaler_128       ((uint16_t)0x0030)
#define SPI_BaudRatePrescaler_256       ((uint16_t)0x0038)
*/
void Spi1_Int(uint16_t BaudRatePrescaler)
{
	GPIO_InitTypeDef GPIO_spix;
	SPI_InitTypeDef  SPI_X;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
	/**GPIO_Pin_5-SCK******GPIO_Pin_6-MISO*****GPIO_Pin_7-MOSI******/
	GPIO_spix.GPIO_Pin=GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_spix.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_spix.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_spix);
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	/*A2-NSS*/
	GPIO_spix.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIO_spix.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_spix.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_spix);
	GPIO_SetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3);
	
	SPI_X.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//双线双向全双工
	SPI_X.SPI_Mode=SPI_Mode_Master;//主模式
	SPI_X.SPI_DataSize=SPI_DataSize_8b;//数据位大小8位
	SPI_X.SPI_CPOL=SPI_CPOL_High;//时钟极性，空闲时时钟为高电平
	SPI_X.SPI_CPHA=SPI_CPHA_2Edge;//时钟相位，在时钟的第2个跳变沿采集数据
	SPI_X.SPI_NSS=SPI_NSS_Soft;//NSS信号由软件产生 
	SPI_X.SPI_BaudRatePrescaler=BaudRatePrescaler;//256分频波特率，RCC_APB2=72M，72M/256=281.25KHz
	SPI_X.SPI_FirstBit=SPI_FirstBit_MSB;//数据高位在前
	SPI_X.SPI_CRCPolynomial=7;//CRC复位值
	SPI_Init(SPI1, &SPI_X);
	SPI_Cmd(SPI1, ENABLE);
}
//spi读写底层驱动
//spi写过程，发送缓存寄存器数据--移位寄存器
//spi读过程，移位寄存器数据--接收缓存寄存器
//TxData：存入的数据
 u8 Spi_RW(u8 TxData)
{		
	
  /*当发送缓存寄存器为空时置1，无须等待*/	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
				  
	SPI_I2S_SendData(SPI1, TxData); //存入一个数据到发送缓存寄存器

 /*当接收缓存寄存器非空时置1，无须等待*/	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	 						    
	return SPI_I2S_ReceiveData(SPI1); //从接收缓存寄存器读取一个数据				    
}
//写数据
/*
add:寄存器地址
*pBuf：写入的数据缓存
num：写入的数据个数
*/
void Spi_write_buf(uint8_t add, uint8_t *pBuf, uint8_t num,uint16_t GPIO_Pin_X)
{
	uint8_t i=0;
	GPIO_ResetBits(GPIOA,GPIO_Pin_X);//NSS拉低选择器件
	Spi_RW(add);
	while(i<num)
	Spi_RW(pBuf[i++]);
	GPIO_SetBits(GPIOA,GPIO_Pin_X);//NSS拉高释放器件
}

//读数据
/*
add:寄存器地址
*pBuf：读取缓存
num：读取的数据个数
*/
void Spi_read_buf(uint8_t add, uint8_t *pBuf, uint8_t num,uint16_t GPIO_Pin_X)
{
	uint8_t i=0;
	GPIO_ResetBits(GPIOA,GPIO_Pin_X);//NSS拉低选择器件
	Spi_RW(add);
	while(i<num)
	pBuf[i++]=Spi_RW(0);
	GPIO_SetBits(GPIOA,GPIO_Pin_X);//NSS拉高释放器件
}
