#ifndef __LED_H
#define __LED_H
#include "sys.h"


#define LED  PBout(5)
#define LED1 PEout(5)
#define BEEP PAout(0)

void LED_Init(void);
void BEEP_Init(void);

#endif

