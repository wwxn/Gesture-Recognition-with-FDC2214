#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "key.h"
#include "usart.h"
#include "stdio.h"
#include "sys.h"
#include "FDC2214.h"
#include "MyIIC.h"
#include "led.h"
#include "string.h"	
#include "kalman.h"
#include "DataScope_DP.h"



double CH1,CH2,CH3,CH4;
float res0,res1,res2,res3;
float temp0,temp1,temp2,temp3;
u8 str[20];
double Flag[10],Flag1[10];
u8 mode=0 ;
u8 Number1=0;
u8 Code[5]={0,0,0,0,0};

/*卡尔曼滤波*/


void Low_pass_filter()
{
	float new_value;
	new_value=Cap_Calculate(0)-temp0;
  new_value=new_value*new_value*100;
  if(new_value-CH1>CH1*0.1||new_value-CH1<-CH1*0.1)
	CH1=new_value;
 
	
}
void Low_pass_filter1()
{
	float new_value1;
	new_value1=Cap_Calculate(2)-temp2;
  new_value1=new_value1*new_value1*1000;
  if(new_value1-CH3>CH3*0.05||new_value1-CH3<-CH3*0.05)
	CH3=new_value1;
}


/*初始化*/
void Init_All()
{
	delay_init();
	SystemInit();	
	LED_Init();
	LCD_Init();
	Key_Init();
	uart_init(115200);//串口1
	BEEP_Init();
	BEEP=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	while(FDC2214_Init())
	temp0 = Cap_Calculate(0);//读取初始值
	temp1 = Cap_Calculate(1);
	temp2 = Cap_Calculate(2);
	temp3 = Cap_Calculate(3);
}
void Number_Calculation()
{
	u8 i;
		
   double sign[4];
	 for(i=0;i<4;i++)
	 {
		 sign[i]=(Flag1[i+1]-Flag1[i])/(Flag1[i+1]+Flag1[i])*Flag1[i]+Flag1[i];
	 }
	 if(CH3<20)
	 {
		 Number1=0;
	 }
	 if(CH3>20&&CH3<sign[0])
	 {
		 Number1=1;
	 }
	 if(CH3>sign[0]&&CH3<sign[1])
	 {
		 Number1=2;
	 }
	 if(CH3>sign[1]&&CH3<sign[2])
	 {
		 Number1=3;
	 }	
	 if(CH3>sign[2]&&CH3<sign[3])
	 {
		 Number1=4;
	 }
	 if(CH3>sign[3])
	 {
		 Number1=5;
	 }
}
 
void Number_Display()
{
	if(mode==1)
	{
		sprintf((char*)str,"Result:%d              ", Number1);
	  LCD_ShowString(30,70,210,24,24,str);
	}
	if(mode==2)
	{
		if(Number1==0&&CH1<10)
		{
			LCD_ShowString(30,70,210,24,24,"No Result!         ");
		}
		else if(Number1==2)
		{
			LCD_ShowString(30,70,210,24,24,"Result:scissors!        ");
		}
		else if(Number1==5)
		{
			LCD_ShowString(30,70,210,24,24,"Result:paper!        ");
		}
		else if(Number1==0&&CH1>=10)
		{
			LCD_ShowString(30,70,210,24,24,"Result:rock!        ");
		}
		else
		{
			LCD_ShowString(30,70,210,24,24,"No Result!         ");
		}
	}
		if(mode==3)
		{
			LCD_ShowString(30,70,210,24,24,"Training....         ");
		}
		if(mode==4)
	 {
		 LCD_ShowString(30,70,210,24,24,"Passward....               ");
	 }
}




	




int main(void)
{
	Init_All();
	while(1) 
	{		
		Low_pass_filter();
		Low_pass_filter1();
		sprintf((char*)str,"CH1:%.2f CH2:%.2f       ",CH1,CH3);
		LCD_ShowString(30,10,210,24,16,str);
		Number_Calculation();
		Number_Display();
		LED1=~LED1;
		delay_ms(50);
	}
}


