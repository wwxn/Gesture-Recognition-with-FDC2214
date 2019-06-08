#ifndef __PWM_H
#define __PWM_H
#include "sys.h"
void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM3_Mode_Config(void);
void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM5_Config(u16 arr,u16 psc);
void TIM8_Configuration(u16 arr,u16 psc);
int Incremental_PI (float Encoder,int Target);
int Incremental_PI2 (float Encoder,int Target);
void zero(void);
#endif

