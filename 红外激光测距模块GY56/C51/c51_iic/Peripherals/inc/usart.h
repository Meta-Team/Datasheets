#ifndef __USART_H__
#define __USART_H__
#include <reg52.h>
#define uint32_t unsigned long
#define uint16_t unsigned int
#define int16_t  int
#define uint8_t unsigned char
#define RegisterA 0X08//¼Ä´æÆ÷1¹Ì¶¨Öµ
#define RegisterB 0X11//¼Ä´æÆ÷2¹Ì¶¨Öµ
#define ACC 0X01
#define GYR 0X02
#define MAG 0X04
#define RPY 0X08
#define Q4  0X10

sbit LED_0=P0^0;
void Usart_Int(uint32_t BaudRatePrescaler);
void send_3out(uint8_t *Data,uint8_t length,uint8_t send);
void USART_send_byte(uint8_t Tx_data);
void send_out(int16_t *Data,uint8_t length,uint8_t send);

extern uint8_t send_ok,Calibration;

#endif