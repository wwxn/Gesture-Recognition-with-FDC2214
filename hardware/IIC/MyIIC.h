#ifndef __MYIIC_H__
#define __MYIIC_H__

#include "sys.h"

#define MyIIC_WR 0 /* 写控制 */ 
#define MyIIC_RD 1 /* 读控制 */
#define MyIIC_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE() 
#define MyIIC_GPIO_PORT GPIOB 
#define MyIIC_SCL_PIN GPIO_PIN_10
#define MyIIC_SDA_PIN GPIO_PIN_11

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8; 
					 
//IO方向设置
#define MyIIC_SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
#define MyIIC_SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}

//IO操作函数	 
#define MyIIC_SCL    PCout(10) 		//SCL
#define MyIIC_SDA    PCout(11) 		//SDA	 
#define MyIIC_READ_SDA   PCin(11) 		//输入SDA 

void MyIIC_Start(void);
void MyIIC_Stop(void);
void MyIIC_SendByte(uint8_t Byte);
uint8_t MyIIC_ReadByte(uint8_t ack);
uint8_t MyIIC_WaitAck(void);
void MyIIC_Ack(void);
void MyIIC_NAck(void);


#endif
