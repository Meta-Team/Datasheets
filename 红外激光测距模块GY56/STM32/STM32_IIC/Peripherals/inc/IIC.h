#ifndef _IIC_H
#define _IIC_H
#include "stm32f10x.h"
#define SCL_H         GPIOB->BSRR = GPIO_Pin_6
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7

#define SCL_read      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
#define SDA_read      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
void I2C_GPIO_Config(void);
void I2C_Stop(void);
u8 Single_WriteI2C_byte(u8 Slave_Address,u8 REG_Address,u8 data);
u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length);
u8 takeRangeReading(u8 Slave_Address);
u8 requestRange(u8 Slave_Address,uint16_t *distance);
u8 changeAddress(u8 oldAddress, u8 newAddress);
u8 find_addr(u8 add);
#endif
