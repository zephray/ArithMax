/*******************************************************************************
  ArithMax Open Source Calculator Project
  www.zephray.com
*******************************************************************************/
#include "timing.h"

__IO uint32_t TimingDelay;

void TIME_Init() 
{
  RCC_ClocksTypeDef RCC_Clocks;
  
  //÷ÿ–¬≈‰÷√SysTick
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
}

void Delay(__IO uint32_t nTime) 
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void TimingDelay_Decrement(void) 
{
  if (TimingDelay != 0x00) 
  { 
    TimingDelay--;
  }
}

void DelayUs(uint32_t us) 
{
    uint32_t time=100*us/7;   
    while(--time);  
}

void SysTick_Handler(void) 
{
  TimingDelay_Decrement();
}