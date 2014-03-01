#ifndef __POWERMAN_H__
#define __POWERMAN_H__

#include "main.h"

void PM_Config();
void PM_EnterStopMode();
void PM_EnterStandbyMode();
#define PM_LDO_On()  GPIOC->BSRRL = GPIO_Pin_2
#define PM_LDO_Off() GPIOC->BSRRH = GPIO_Pin_2

#endif
