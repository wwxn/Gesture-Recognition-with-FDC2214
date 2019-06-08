#ifndef __BSP_USART3_H
#define	__BSP_USART3_H

#include "stm32f10x.h"
#include <stdio.h>


void USART3_Config(u32 bound);
void USART3_NVIC_Configuration(void);

void USART3_Send(unsigned char *tx_buf, int len);
void USART3_Receive(unsigned char *rx_buf, int len);


#endif /* __USART1_H */
