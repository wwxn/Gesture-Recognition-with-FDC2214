#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>


void USART1_Config(u32 bound);
void USART1_NVIC_Configuration(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
void USART1_Send(unsigned char *tx_buf, int len);
void USART1_Receive(unsigned char *rx_buf, int len);


#endif /* __USART1_H */
