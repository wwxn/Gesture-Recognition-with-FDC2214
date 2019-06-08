
#include "motor_control.h"

/*------------------------------------------
 				全局变量				
------------------------------------------*/



/*------------------------------------------
 函数功能:控制器软件复位
 函数说明:强制复位			
------------------------------------------*/
void MCU_Reset(void) 
{  
	__set_FAULTMASK(1);   // 关闭所有中断
 	NVIC_SystemReset();   // 复位
}


/*------------------------------------------
 函数功能:电机底层驱动函数
 函数说明:
------------------------------------------*/
void MotorMove(u32 pwm1,u32 pwm2,u32 pwm3,u32 pwm4)
{	
	TIM_SetCompare4(TIM2,pwm1);//左轮前进
	TIM_SetCompare1(TIM2,pwm2);//右轮前进
	
	TIM_SetCompare3(TIM2,pwm3);//左轮后退
	TIM_SetCompare2(TIM2,pwm3);//右轮后退
	
	TIM_SetCompare1(TIM1,pwm4);//舵机
}


