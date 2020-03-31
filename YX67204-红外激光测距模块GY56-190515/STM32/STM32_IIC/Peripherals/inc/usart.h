#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"

void Usart_Int(uint32_t BaudRatePrescaler);
void USART1_send_byte(uint8_t byte);
void USART2_send_byte(uint8_t byte);
void send_3out(uint8_t *data,uint8_t length,uint8_t send);
void send_out(uint16_t *data,uint8_t length,uint8_t send);
void display(int16_t *num,u8 send,u8 count);
void CHeck(uint8_t *re_data);
void USART_Send(uint8_t *Buffer, uint8_t Length);
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length);

extern u8 Calibration;
#endif
