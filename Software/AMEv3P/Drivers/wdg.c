/*******************************************************************************
  ArithMax Open Source Calculator Project
  www.zephray.com
*******************************************************************************/
#include "main.h"
#include "wdg.h"

void WDG_TIMConfig(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  uint16_t PrescalerValue = 0;
  
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  
  /* Enable the TIM3 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
  
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) 0x400;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 0x800;
  TIM_TimeBaseStructure.TIM_Prescaler = 0x400;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  
  /* TIM IT enable */
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
  
  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
}

void WDG_Init(void)
{
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//使能写入PR和RLR
  IWDG_SetPrescaler(IWDG_Prescaler_128);  //写入PR预分频值
  IWDG_SetReload(100);  //写入RLR
  WDG_TIMConfig();
  IWDG_Enable();//KR写入0xCCCC
}