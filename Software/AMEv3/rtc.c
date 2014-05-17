#include "rtc.h"

void RTC_Config(void)
{
  RTC_InitTypeDef RTC_InitStructure;
  RTC_TimeTypeDef RTC_TimeStructure;
  RTC_DateTypeDef RTC_DateStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
  PWR_BackupAccessCmd(ENABLE);//使能备份寄存器操作
  
  RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();
  
  if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x9741)   //一个变量，看看RTC初始化没
  {
    
    RTC_WriteProtectionCmd(DISABLE);
  
    RTC_EnterInitMode();
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_InitStructure.RTC_AsynchPrediv = 0x7D-1;
    RTC_InitStructure.RTC_SynchPrediv = 0xFF-1;
    RTC_Init(&RTC_InitStructure);
  
    RTC_TimeStructure.RTC_Seconds = 0x00;
    RTC_TimeStructure.RTC_Minutes = 0x01;
    RTC_TimeStructure.RTC_Hours = 0x01;
    RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
    RTC_SetTime(RTC_Format_BCD,&RTC_TimeStructure);
  
    RTC_DateStructure.RTC_Date = 18;
    RTC_DateStructure.RTC_Month = 5;
    RTC_DateStructure.RTC_WeekDay= RTC_Weekday_Saturday;
    RTC_DateStructure.RTC_Year = 13;
    RTC_SetDate(RTC_Format_BCD,&RTC_DateStructure);
  
    RTC_ExitInitMode();
    RTC_WriteBackupRegister(RTC_BKP_DR0,0X9741);
    RTC_WriteBackupRegister(RTC_BKP_DR1,0);
    RTC_WriteBackupRegister(RTC_BKP_DR2,31);
    RTC_WriteProtectionCmd(ENABLE);
    RTC_WriteBackupRegister(RTC_BKP_DR0,0x9741);  //初始化完成，设置标志
  }
  PWR_BackupAccessCmd(DISABLE);
}

void RTC_SetDateTime(RTC_TimeTypeDef *time,RTC_DateTypeDef *date)
{
  RTC_InitTypeDef RTC_InitStructure;
  RTC_TimeTypeDef RTC_TimeStructure;
  RTC_DateTypeDef RTC_DateStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
  PWR_BackupAccessCmd(ENABLE);//使能备份寄存器操作
  
  RCC_LSICmd(ENABLE);
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();
    
    RTC_WriteProtectionCmd(DISABLE);
  
    RTC_EnterInitMode();
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_InitStructure.RTC_AsynchPrediv = 0x7D-1;
    RTC_InitStructure.RTC_SynchPrediv = 0xFF-1;
    RTC_Init(&RTC_InitStructure);
    
    RTC_SetTime(RTC_Format_BIN,time);
    RTC_SetDate(RTC_Format_BIN,date);
  
    RTC_ExitInitMode();
    RTC_WriteBackupRegister(RTC_BKP_DR0,0X9741);
    RTC_WriteBackupRegister(RTC_BKP_DR1,0);
    RTC_WriteBackupRegister(RTC_BKP_DR2,31);
    RTC_WriteProtectionCmd(ENABLE);
    RTC_WriteBackupRegister(RTC_BKP_DR0,0x9741);  //初始化完成，设置标志

  PWR_BackupAccessCmd(DISABLE);
}