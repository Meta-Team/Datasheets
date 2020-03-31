#ifndef __SPI_H__
#define __SPI_H__
#include <reg52.h>
#define uint32_t unsigned long
#define uint16_t unsigned int
#define uint8_t unsigned char
sbit MOSI=P3^7;
sbit MISO=P3^6;
sbit SCK=P3^5;
sbit CS=P3^4;
void Spi_Int(void);
void Spi_write_buf(uint8_t add, uint8_t *pBuf, uint8_t num);
void Spi_read_buf(uint8_t add, uint8_t *pBuf, uint8_t num);
#endif
