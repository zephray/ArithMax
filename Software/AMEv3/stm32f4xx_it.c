/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides all exceptions handler and peripherals interrupt
  *          service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"
#include "stn.h"
#include "pccon.h"
#include "key.h"
#include "powerman.h"
    
uint16_t Timing=0;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
  if (LCD_FB_Touched)
  {
    LCD_RealUpdate(Timing%50);
    LCD_FB_Touched =0;
  }
  Timing++;
  if (Timing==100)
  {
    voltage=PM_GetVolt();
    if (voltage<3680)
    {
      LCD_Fill(0,0,95,7,0);
      LCD_String_5X7(0,0,"Battery Low",1);
    }
    if (voltage<3350)
    {
      PM_EnterStandbyMode();
    }
    LCD_RealUpdate(1);    
    Timing=0;
  }
  else if (Timing==50)
  {
    LCD_RealUpdate(0);
  }
}

/******************************************************************************/
/*                 STM32Fxxx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32fxxx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @brief  This function handles EXTI0_IRQ Handler.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0)==SET)
  { 
    //LCD_String(0,0,"ON Key Pressed!",1);
    //LCD_Update();
    EXTI_ClearITPendingBit(EXTI_Line0);
    NVIC_SystemReset();
    
  }
}

void TIM3_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM3,TIM_IT_Update)!= RESET)
  {
    IWDG_ReloadCounter();
    TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update); 
  }
}

void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET)
  {
    NowKey=KBD_Scan();
    if (NowKey==6)
    {
      if (ShiftState&0x80)
      {
        LCD_StatusSet(LCD_STB_Shift,0);
        ShiftState&=0x7f;
      }else
      {
        LCD_StatusSet(LCD_STB_Shift,1);
        ShiftState|=0x80;
      }
      while(KBD_Scan()==6);
    }else
    if (NowKey==5)
    {
      if (ShiftState&0x40)
      {
        LCD_StatusSet(LCD_STB_Alpha,0);
        ShiftState&=0xbf;
      }else
      {
        LCD_StatusSet(LCD_STB_Alpha,1);
        ShiftState|=0x40;
      }
      while(KBD_Scan()==5);
    }
    else
    if ((NowKey==35)&&(ShiftState&0x80))
    {
      if (ShiftState&0x80)
      {
        //IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
        LCD_DispBmp1bpp(0,0,96-1,32-1,(unsigned char *)gImage_mainlogo);
        LCD_StatusClear();
        LCD_RealUpdate(0);
        DelayUs(100000);
        DelayUs(100000);
        DelayUs(50000);
        TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update); 
        PWR_BackupAccessCmd(ENABLE);//使能备份寄存器操作
        RTC_WriteProtectionCmd(DISABLE);
        RTC_WriteBackupRegister(RTC_BKP_DR1,0x0001);
        PWR_BackupAccessCmd(DISABLE);
        NVIC_SystemReset();
      }
    }else
    if (NowKey!=255)
    {
      LastKey=NowKey;
    }
    TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update); 
  }
}

void USART1_IRQHandler(void)
{
  unsigned char c;
  uint16_t d;
  if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
  {
     c = USART_ReceiveData(USART1);
     if (c==0x5a)
     {
       PCCon_Start();
     }
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
  }
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
