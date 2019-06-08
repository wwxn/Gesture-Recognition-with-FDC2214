#include "pwm.h"
#include "sys.h"
#include "led.h"
#include "delay.h"
#include "usart.h"

float speed1,speed2;
u32 Pwm,Pwm1;
void TIM1_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; 
TIM_OCInitTypeDef TIM_OCInitStructure; 
	
  NVIC_InitTypeDef NVIC_InitStructure;

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);   

GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //�����������
GPIO_Init(GPIOA,&GPIO_InitStructure);

TIM_TimeBaseInitStructure.TIM_Period = arr; //�����Զ���װ�ؼĴ������ڵ�ֵ

TIM_TimeBaseInitStructure.TIM_Prescaler = psc;//����������ΪTIMxʱ��Ƶ��Ԥ��Ƶֵ
TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//����ʱ�ӷָ�:TDTS = Tck_tim
TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
TIM_TimeBaseInit(TIM1, & TIM_TimeBaseInitStructure);
	

TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
TIM_OCInitStructure.TIM_Pulse=10;
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
TIM_OC1Init(TIM1,&TIM_OCInitStructure);

TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

TIM_Cmd(TIM1,ENABLE);

//��������Ǹ߼���ʱ�����У����PWM�����

TIM_CtrlPWMOutputs(TIM1, ENABLE);                             //pwm���

}
void TIM8_Configuration(u16 arr,u16 psc)  // ���1    �������ӿ����ã�TIM8��/PC6-A��  PC7-B��  
{
GPIO_InitTypeDef GPIO_InitStructure; 
TIM_ICInitTypeDef  TIM_ICInitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);  //�ر�JTAG�ӿ�
//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);  //�ر�JTAG�ӿ� ������SWD
//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 

//GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);       //TIM8û�������ض��� ��ע�͵�
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;      //PC6,PC7
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
GPIO_Init(GPIOC, &GPIO_InitStructure);      
GPIO_WriteBit(GPIOC, GPIO_Pin_6|GPIO_Pin_7,Bit_SET);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
TIM_TimeBaseStructure.TIM_Period = 260; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ�������㹻�󼴿�
            // ֻҪ�ڲ��������ڲ��������������
TIM_TimeBaseStructure.TIM_Prescaler = 0; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ           
TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim 

TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3�������½�������

TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6;  //�����˲���9
    TIM_ICInit(TIM8, &TIM_ICInitStructure);
    TIM_ClearFlag(TIM8, TIM_FLAG_Update);  //������б�־λ
    TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE); //�����жϸ���

TIM8->CNT = 0;


TIM_Cmd(TIM8, ENABLE);
}
void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM2 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC2
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC2
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC2
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC2

	//TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM2, ENABLE);
	
}


void TIM3_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/*- ������������������ PA->6   PA->7 -*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                           

	/*- TIM4������ģʽ���� -*/
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = 260;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              
                 
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//���ñ�����ģʽ����Դ�ͼ���
	
	TIM_ICStructInit(&TIM_ICInitStructure);																																		//�����˲���
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
		
	TIM3->CNT = 0;

	TIM_Cmd(TIM3, ENABLE);   //����TIM3��ʱ��
}


void TIM5_Config(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 

	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 
 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); 

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

//	TIM_Cmd(TIM5, ENABLE);	
		
}

extern u16 motor;
extern u8 Flag;
void TIM5_IRQHandler()
{
	static float angle1,angle2;
	speed2=TIM8->CNT*1.39-angle2;
	speed1=TIM3->CNT*1.39-angle1;
	if(speed1>=-0.1)
	{
		;
	}
	else
	{
		speed1+=360;
	}
	angle1=TIM3->CNT*1.39;
		if(speed2>=-0.1)
	{
		;
	}
	else
	{
		speed2+=91093;
	}
	angle2=TIM8->CNT*1.39;
	speed1*=2.78;
	speed2*=2.78;
	Incremental_PI(speed1,25);
	Incremental_PI2(speed2,25);
  TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );
	
}

 float Bias,Last_bias,Bias1,Last_bias1;
int Incremental_PI (float Encoder,int Target)
{ 	
	
	
	  float Kp=30,Ki=0.5;	
	
	 Bias=Target-Encoder;                //����ƫ��
		if(Bias<-80||Bias>200)return 0;
	 Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //PI������

	if(Pwm>2000)Pwm = 2000;
	
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	
	// return Pwm;                         //�������
}
int Incremental_PI2 (float Encoder,int Target)
{ 	
	
	
	  float Kp=30,Ki=0.5;	
	
	 Bias1=Target-Encoder;                //����ƫ��
		if(Bias1<-80||Bias1>200)return 0;
	 Pwm1+=Kp*(Bias1-Last_bias1)+Ki*Bias1;   //PI������

	if(Pwm1>2000)Pwm1 = 2000;
	
	 Last_bias1=Bias1;	                   //������һ��ƫ�� 
	
	// return Pwm;                         //�������
}

void zero()
{
	Bias=0;
	Last_bias=0;
	speed1=0;
	printf("Stop!!!:%d\r\n",Pwm);
	Pwm=0;
	
}



