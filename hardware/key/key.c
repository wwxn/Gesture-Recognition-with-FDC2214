#include "key.h"
#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "stdio.h"
#include "led.h"

u8 Key_Value;
extern double Flag[10],Flag1[10];
extern double CH1,CH3;
u8 str1[20];
extern u8 mode;
extern void Low_pass_filter1(void);
extern u8 Code[5];
extern u8 Number1;
extern void Number_Calculation(void);
void Key_Init(void)
{
/******************PE的0 1 2 3作为输出**********************************/
		GPIO_InitTypeDef GPIO_InitStructure;
	 	EXTI_InitTypeDef EXTI_InitStructure;
 	 NVIC_InitTypeDef NVIC_InitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);                //使能PORTC
	  GPIO_InitStructure.GPIO_Pin= GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; 
    GPIO_InitStructure.GPIO_Speed= GPIO_Speed_10MHz;
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	  GPIO_Init(GPIOF, &GPIO_InitStructure);

/******************PE的 4 5 6 7作为下拉输入*****************************/
  
  GPIO_InitStructure.GPIO_Pin= GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;  //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_SetBits(GPIOF,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2 |GPIO_Pin_3); 
  GPIO_ResetBits(GPIOF,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	
	
	
	
	
/****************配置中断线***********************************************/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

    
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource4);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	

   
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_Init(&EXTI_InitStructure);	  	

   
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource6);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
  	EXTI_Init(&EXTI_InitStructure);	  	
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource7);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_Init(&EXTI_InitStructure);	
		
		
		
/******************************配置中断优先级**************************/
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  	NVIC_Init(&NVIC_InitStructure); 

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  	NVIC_Init(&NVIC_InitStructure);


}


/*****************键盘扫描*********************************/
u8 Read_KeyValue(void)
{
	u8 KeyValue=0; 
	if((GPIO_ReadInputData(GPIOF)&0xff)!=0x0f) 	
	{
		//delay_ms(10);
		if((GPIO_ReadInputData(GPIOF)&0xff)!=0x0f) 
		{
			GPIO_SetBits(GPIOF,GPIO_Pin_0); 
			GPIO_ResetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
			switch(GPIO_ReadInputData(GPIOF)&0xff)
		   {	    
		case 0x11:KeyValue=1;break; 
		case 0x21:KeyValue=5;break;
			case 0x41:KeyValue=9;break;
			case 0x81:KeyValue=13;break;
		   }	
		GPIO_SetBits(GPIOF,GPIO_Pin_1); 
		GPIO_ResetBits(GPIOF,GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3); 
			switch(GPIO_ReadInputData(GPIOF)&0xff) 
			{
				case 0x12:KeyValue=2;break;
				 case 0x22:KeyValue=6;break;
				case 0x42:KeyValue=10;break;
				case 0x82:KeyValue=14;break;
			}
		 GPIO_SetBits(GPIOF,GPIO_Pin_2); 
		 GPIO_ResetBits(GPIOF,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3); 
			switch(GPIO_ReadInputData(GPIOF)&0xff) 
			{
				case 0x14:KeyValue=3;break;
				 case 0x24:KeyValue=7;break;
				case 0x44:KeyValue=11;break;
				case 0x84:KeyValue=15;break;
			}
		GPIO_SetBits(GPIOF,GPIO_Pin_3);
		GPIO_ResetBits(GPIOF,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
			switch(GPIO_ReadInputData(GPIOF)&0xff)
				{
			  case 0x18:KeyValue=4;break;
					case 0x28:KeyValue=8;break;
					case 0x48:KeyValue=12;break;
					case 0x88:KeyValue=16;break;
				}
			GPIO_SetBits(GPIOF,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2 |GPIO_Pin_3);
			GPIO_ResetBits(GPIOF, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7);
			while((GPIO_ReadInputData(GPIOF)&0xff)!=0x0f);
		  return KeyValue;
		}
}
	return 0;
}




void Key_Deal()
{
	static u8 i=0;
	Key_Value=Read_KeyValue();
	switch(Key_Value)
	{
		case 1:
		{
			delay_ms(500);
			delay_ms(500);
			delay_ms(500);
			delay_ms(500);
			Low_pass_filter1();
			Flag[i]=CH1;
			Flag1[i]=CH3;
			i++;
			if(mode==3)
			{
				sprintf((char*)str1,"Training Number:   %d",i);
				LCD_ShowString(30,100,210,24,16,str1);

			}
			break;
		}
		case 2:
		{
			mode=1;
			LCD_ShowString(30,100,210,24,16,"                    ");
			break;
		}
		case 3:
		{
			mode=2;
			LCD_ShowString(30,100,210,24,16,"                    ");
			break;
		}
		case 4:
		{
			mode=3;
			LCD_ShowString(30,100,210,24,16,"                    ");
			break;
		}
		case 5:
		{
			mode=4;
			Code[4]=1;
			LCD_ShowString(30,100,210,24,16,"                    ");
			break;
		}	
		case 6:
		{
			 if(mode==4)
			 {
				 delay_ms(1000);
				 Low_pass_filter1();
				 Number_Calculation();
				 if(Number1==1&&Code[2]==0&&Code[1]==0)
				 {
							 Code[0]=1;
							Code[4]=1;
				 }
				 if(Number1==2&&Code[0]==1&&Code[2]==0)
				 {
						
							 Code[1]=1;


				 }		 
				 if(Number1==5&&Code[0]==1&&Code[1]==1)
				 {
							 Code[2]=1;

				 } 
				 if(Number1==3)
				 {
							 Code[4]=0;
				 }
				 if(Number1==4)
				 {
							 Code[4]=0;
				 }
				 if(Code[0]&&Code[1]&&Code[2]&&Code[4])
				 {
					 BEEP=1;
					 LCD_ShowString(30,70,210,24,24,"Success!!                 ");
					 delay_ms(500);
					 delay_ms(500);
					 BEEP=0;
					 LCD_ShowString(30,70,210,24,24,"Passward ...               ");
					 Code[0]=0;
					 Code[1]=0;
					 Code[2]=0;
					 Code[4]=0;
				 }
			 }
		 }
	 }
 }









void EXTI4_IRQHandler(void)
{
	delay_ms(50);
	if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4)==1)
	{
		Key_Deal();
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}




void EXTI9_5_IRQHandler(void)
{
	delay_ms(50);
	if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)==1)
	{
		Key_Deal();
	}
	EXTI_ClearFlag(EXTI_Line5|EXTI_Line6|EXTI_Line7);
}

