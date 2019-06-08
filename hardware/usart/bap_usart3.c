
  
#include "bsp_usart3.h"
#include <stdio.h>

volatile unsigned char *rx3_address;
volatile unsigned int rx3_count;
volatile unsigned int rx3_length;

 /**
  * @brief  USART1 GPIO 配置,工作模式配置。9600 8-N-1
  * @param  无
  * @retval 无
  */



void USART3_Config(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PB.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	
	/* 使能串口1接收中断 */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART3, ENABLE);
	 
}

/// 配置USART3接收中断
void USART3_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn ;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



void USART3_Send(unsigned char *tx_buf, int len)
{
		USART_ClearFlag(USART3, USART_FLAG_TC);
		USART_ClearITPendingBit(USART3, USART_FLAG_TXE);
	while(len--)
	{
		USART_SendData(USART3, *tx_buf);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) != 1);
		USART_ClearFlag(USART3, USART_FLAG_TC);
		USART_ClearITPendingBit(USART3, USART_FLAG_TXE);
		tx_buf++;
	}
	
}

void USART3_Receive(unsigned char *rx_buf, int len)
{
	rx3_count = 0;
	rx3_length = len;
	rx3_address = rx_buf;
}

void USART3_IRQHandler(void)
{
	unsigned char Res;
	//u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		
		Res = USART_ReceiveData(USART3);
	//	 printf("[USART1_IRQHandler],Rec_data = %x\r\n", Res);

		if(rx3_length > rx3_count)
		{
			*rx3_address = Res;
			rx3_address++;
			rx3_count++;	
		}
				 
    } 
	
		if ( USART_GetITStatus( USART3, USART_IT_IDLE ) == SET )                                         //数据帧接收完毕
		{
    		rx3_length = 0;			
			Res = USART_ReceiveData( USART3 ); 
			 
   		 } 		
}


/*********************************************END OF FILE**********************/
