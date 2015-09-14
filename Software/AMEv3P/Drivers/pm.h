/*******************************************************************************
  ArithMax Open Source Calculator Project
  www.zephray.com
*******************************************************************************/
#ifndef __PM_H__
#define __PM_H__

#include "main.h"

void PM_Init(void);
void PM_EnterStopMode(void);
void PM_EnterStandbyMode(void);
void PM_SetCPUFreq(uint8_t freq);
uint32_t PM_GetVolt();

#endif
