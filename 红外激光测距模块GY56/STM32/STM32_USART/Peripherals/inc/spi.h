#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"
#define NCS GPIO_Pin_2
#define CSB GPIO_Pin_3
void Spi1_Int(uint16_t BaudRatePrescaler);
 u8 Spi_RW(u8 TxData);
void Spi_write_buf(uint8_t add, uint8_t *pBuf, uint8_t num,uint16_t GPIO_Pin_X);
void Spi_read_buf(uint8_t add, uint8_t *pBuf, uint8_t num,uint16_t GPIO_Pin_X);
#endif
