/*******************************************************************************
  ArithMax Open Source Calculator Project
  www.zephray.com
*******************************************************************************/
#ifndef __TIMING_H__
#define __TIMING_H__

#include "main.h"

void TIME_Init();
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void DelayUs(uint32_t us);

#endif