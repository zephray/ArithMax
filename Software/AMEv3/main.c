#include "main.h"
#include "usart.h"
#include "stn.h"
#include "sdcard.h"
#include "spiflash.h"
#include "key.h"
#include "powerman.h"
#include "rtc.h"
//#include "dac.h"

uint8_t brightness=30;
uint32_t sf_size,voltage;

const char* menu_string[]=
{
  "计算器","电子书","游戏","设置"
};

__IO uint8_t ShiftState;//最低位：Shift状态 第二位：Alpha状态
__IO uint32_t TimingDelay;

void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

void About_main()
{
 LCD_Clear(0);
 LCD_String_6X8(0,0 ,(unsigned char*)"ArithMax E301 r1",1);
 LCD_String_6X8(0,8 ,(unsigned char*)"Software:1.0.DEV",1);
 LCD_String_6X8(0,16,(unsigned char*)"",1);
 LCD_String_6X8(0,24,(unsigned char*)"Designed by Zweb",1);
 LCD_Update();
 GetKey();
}

void Info_main()
{
  char str[17]="Designed by Zweb";
  sprintf(str,"Flash:%dKB",(sf_size>>10));
  LCD_Clear(0);
  LCD_String_6X8(0,0 ,(u8 *)"CPU:STM32F407VG",1);
  LCD_String_6X8(0,8 ,(u8 *)str,1);
  sprintf(str,"SD:%dMB",SDCardInfo.CardCapacity>>20);
  LCD_String_6X8(0,16,(u8 *)str,1); 
  voltage=PM_GetVolt();
  sprintf(str,"Volt:%dmV",voltage);
  LCD_String_6X8(0,24,(u8 *)str,1);
  LCD_Update();
  GetKey();
}
  
void Contrast_main()
{
      u8 cont=1;
      u8 key;
      
      LCD_Clear(0x00);
  LCD_String_6X8(0,0 ,"CONTRAST        ",1);
  LCD_String_6X8(0,16,"LIGHT       DARK",1);
  LCD_String_6X8(0,24," [<]        [>] ",1);
  LCD_Update();
  while (cont)
  {
    key=GetKey(); 
    switch (key)
    {
    case KEY_CTRL_LEFT:if (brightness>0) brightness-=1; LCD_SetBrightness(brightness);break;
    case KEY_CTRL_RIGHT:if (brightness<63) brightness+=1; LCD_SetBrightness(brightness);break;
    case KEY_CTRL_AC:cont=0;break;
    } 
    PWR_BackupAccessCmd(ENABLE);//使能备份寄存器操作
    RTC_WriteProtectionCmd(DISABLE);
    RTC_WriteBackupRegister(RTC_BKP_DR2,brightness);
    RTC_WriteProtectionCmd(ENABLE);
    PWR_BackupAccessCmd(DISABLE);
  }
}

void Diag_main()
{
  u8 key;
  u16 i;
  LCD_Clear(0);
  LCD_String_6X8(0,1 ,"DIAGNOSTIC",1);
  LCD_Update();
  key=GetKey();
  if (key!=KEY_CHAR_9) return;
  LCD_Clear(0xff);
  LCD_Update();
  LCD_WriteCmd(0xee);
  LCD_WriteCmd(0xb8);
  LCD_WriteCmd(0x12);
  LCD_WriteCmd(0x04);
  LCD_WriteCmd(0xe0); 
  for (i=0;i<96;i++)
    LCD_WriteDat(0x01);
  WaitForCertainKey(KEY_CTRL_EXE);
  LCD_Clear(0x00);
  LCD_Update();
  LCD_WriteCmd(0xee);
  LCD_WriteCmd(0xb8);
  LCD_WriteCmd(0x12);
  LCD_WriteCmd(0x04);
  LCD_WriteCmd(0xe0); 
  for (i=0;i<96;i++)
    LCD_WriteDat(0x00);
  WaitForCertainKey(KEY_CTRL_EXE);
  LCD_Line(0,0,95,0,1);
  LCD_Line(0,0,0,30,1);
  LCD_Line(0,30,95,30,1);
  LCD_Line(95,0,95,30,1);
  LCD_Update();
  WaitForCertainKey(KEY_CTRL_EXE);
  for (i=0;i<384;i+=2)
  {
      LCD_FB[i]=0xAA;
      LCD_FB[i+1]=0x55;
  }
  LCD_StatusSet(LCD_STB_Shift,1);
  LCD_StatusSet(LCD_STB_Alpha,1);
  LCD_StatusSet(LCD_STB_M,1);
  LCD_StatusSet(LCD_STB_RCL,1);
  LCD_StatusSet(LCD_STB_STAT,1);
  LCD_StatusSet(LCD_STB_MAT,1);
  LCD_StatusSet(LCD_STB_G,1);
  LCD_StatusSet(LCD_STB_FIX,1);
  LCD_StatusSet(LCD_STB_Math,1);
  LCD_StatusSet(LCD_STB_Disp,1);
  WaitForCertainKey(KEY_CTRL_EXE);
  for (i=0;i<384;i+=2)
  {
      LCD_FB[i]=0x55;
      LCD_FB[i+1]=0xAA;
  }
  LCD_Update();
  LCD_WriteCmd(0xee);
  LCD_WriteCmd(0xb8);
  LCD_WriteCmd(0x12);
  LCD_WriteCmd(0x04);
  LCD_WriteCmd(0xe0); 
  for (i=0;i<96;i++)
    LCD_WriteDat(0x00);
  LCD_StatusSet(LCD_STB_STO,1);
  LCD_StatusSet(LCD_STB_CMPLX,1);
  LCD_StatusSet(LCD_STB_VCT,1);
  LCD_StatusSet(LCD_STB_D,1);
  LCD_StatusSet(LCD_STB_R,1);
  LCD_StatusSet(LCD_STB_SCI,1);
  LCD_StatusSet(LCD_STB_Down,1);
  LCD_StatusSet(LCD_STB_Up,1);
  WaitForCertainKey(KEY_CTRL_EXE);
  LCD_Clear(0);
  LCD_WriteCmd(0xee);
  LCD_WriteCmd(0xb8);
  LCD_WriteCmd(0x12);
  LCD_WriteCmd(0x04);
  LCD_WriteCmd(0xe0); 
  for (i=0;i<96;i++)
    LCD_WriteDat(0x00);
  LCD_String_6X8(0,1 ,"ArithMax E300",1);
  LCD_String_6X8(0,8 ,"EVT For DEV Only",1);
  LCD_String_6X8(0,24,"Press AC",1);
  LCD_Update();
  WaitForCertainKey(KEY_CTRL_AC);
  Contrast_main();
  LCD_Clear(0);
  LCD_Update();
  while (key!=KEY_CTRL_EXE)
  {
    key=GetKey();
    LCD_DispNum_6X8(0,0,key,3,1);
    LCD_Update();
  }
}

void SetTime_main();


void Setup_main()
{
  u8 key;
  u8 cont =1;
  
  LCD_Clear(0x00);
  LCD_String_6X8(0,0 ,"1:Deg   2:Rad   ",1);
  LCD_String_6X8(0,8 ,"3:Gra   4:Date  ",1);
  LCD_String_6X8(0,16,"5:Cont. 6:Diag. ",1);
  LCD_String_6X8(0,24,"7:Info  8:About ",1);
  LCD_Update();
  while (cont==1)
  {
    key=GetKey();
    cont=0;
    switch (key)
    {
    case KEY_CHAR_4:SetTime_main();break;
    case KEY_CHAR_6:Diag_main();break;
    case KEY_CHAR_5:Contrast_main();break;
    case KEY_CHAR_7:Info_main();break;
    case KEY_CHAR_8:About_main();break;
    case KEY_CTRL_AC:cont=0;break;
    default:LCD_StatusSet(LCD_STB_Disp,1);Delay(10);LCD_StatusSet(LCD_STB_Disp,0);cont=1;break;
    }
  }
}

void UI_ShowDateTime(u8 y,RTC_TimeTypeDef *time,RTC_DateTypeDef *date)
{
  LCD_String_6X8(0,y,"20  -  -     :  ",1);
  //LCD_DispNum_6X8(11*6,8,RTC_TimeStructure.RTC_Seconds,2,1);
  LCD_DispNum_6X8(11*6,y,time->RTC_Hours,2,1);
  LCD_DispNum_6X8(14*6,y,time->RTC_Minutes,2,1);
  LCD_DispNum_6X8(2*6,y,date->RTC_Year,2,1);
  LCD_DispNum_6X8(5*6,y,date->RTC_Month,2,1);
  LCD_DispNum_6X8(8*6,y,date->RTC_Date,2,1);
  LCD_Update();
}

void SetTime_main()
{
  RTC_TimeTypeDef RTC_TimeStructure;
  RTC_DateTypeDef RTC_DateStructure;
  u8 state,key;
  u16 tmp;
 
  //获取新的时间
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  state=0;
  LCD_Clear(0);
  UI_ShowDateTime(8,&RTC_TimeStructure,&RTC_DateStructure);
  while (state!=5)
  {
    //Set Year
    LCD_Line(0*6,16,4*6,16,1);
    while(state==0)
    {
      key=GetKey();
      if (key==KEY_CTRL_AC)
        return;
      else
      if (key==KEY_CTRL_UP)
      {
        tmp=RTC_DateStructure.RTC_Year;
        if (tmp<99) tmp++; else tmp=00;
        RTC_DateStructure.RTC_Year=tmp;
        UI_ShowDateTime(8,&RTC_TimeStructure,&RTC_DateStructure);
      }
      else  
      if (key==KEY_CTRL_DOWN)
      {
        tmp=RTC_DateStructure.RTC_Year;
        if (tmp>0) tmp--; else tmp=99;
        RTC_DateStructure.RTC_Year=tmp;
        UI_ShowDateTime(8,&RTC_TimeStructure,&RTC_DateStructure);
      }
      else
      if (key==KEY_CTRL_EXE)
        state=1;
    }
    LCD_Line(0*6,16,4*6,16,0);
  
    //Set Month
    LCD_Line(5*6,16,7*6,16,1);
    LCD_Update();
    while(state==1)
    {
      key=GetKey();
      if (key==KEY_CTRL_AC)
        state=0;
      else
      if (key==KEY_CTRL_UP)
      {
        tmp=RTC_DateStructure.RTC_Month;
        if (tmp<12) tmp++; else tmp=1;
        RTC_DateStructure.RTC_Month=tmp;
        UI_ShowDateTime(8,&RTC_TimeStructure,&RTC_DateStructure);
      }
      else  
      if (key==KEY_CTRL_DOWN)
      {
        tmp=RTC_DateStructure.RTC_Month;
        if (tmp>1) tmp--; else tmp=12;
        RTC_DateStructure.RTC_Month=tmp;
        UI_ShowDateTime(8,&RTC_TimeStructure,&RTC_DateStructure);
      }
      else
      if (key==KEY_CTRL_EXE)
        state=2;
    }
    LCD_Line(5*6,16,7*6,16,0);
  
    //Set Day
    LCD_Line(8*6,16,10*6,16,1);
    LCD_Update();
    while(state==2)
    {
      key=GetKey();
      if (key==KEY_CTRL_AC)
        state=1;
      else
      if (key==KEY_CTRL_UP)
      {
        tmp=RTC_DateStructure.RTC_Date;
        if (tmp<31) tmp++; else tmp=1;
        RTC_DateStructure.RTC_Date=tmp;
        UI_ShowDateTime(8,&RTC_TimeStructure,&RTC_DateStructure);
      }
      else  
      if (key==KEY_CTRL_DOWN)
      {
        tmp=RTC_DateStructure.RTC_Date;
        if (tmp>1) tmp--; else tmp=31;
        RTC_DateStructure.RTC_Date=tmp;
        UI_ShowDateTime(8,&RTC_TimeStructure,&RTC_DateStructure);
      }
      else
      if (key==KEY_CTRL_EXE)
        state=3;
    }
    LCD_Line(8*6,16,10*6,16,0);
  
    //Set Hour
    LCD_Line(11*6,16,13*6,16,1);
    LCD_Update();
    while(state==3)
    {
      key=GetKey();
      if (key==KEY_CTRL_AC)
        state=2;
      else
      if (key==KEY_CTRL_UP)
      {
        tmp=RTC_TimeStructure.RTC_Hours;
        if (tmp<23) tmp++; else tmp=0;
        RTC_TimeStructure.RTC_Hours=tmp;
        UI_ShowDateTime(8,&RTC_TimeStructure,&RTC_DateStructure);
      }
      else  
      if (key==KEY_CTRL_DOWN)
      {
        tmp=RTC_TimeStructure.RTC_Hours;
        if (tmp>0) tmp--; else tmp=23;
        RTC_TimeStructure.RTC_Hours=tmp;
        UI_ShowDateTime(8,&RTC_TimeStructure,&RTC_DateStructure);
      }
      else
      if (key==KEY_CTRL_EXE)
        state=4;
    }
    LCD_Line(11*6,16,13*6,16,0);
  
    //Set Minute
    LCD_Line(14*6,16,16*6,16,1);
    LCD_Update();
    while(state==4)
    {
      key=GetKey();
      if (key==KEY_CTRL_AC)
        state=3;
      else
      if (key==KEY_CTRL_UP)
      {
        tmp=RTC_TimeStructure.RTC_Minutes;
        if (tmp<59) tmp++; else tmp=0;
        RTC_TimeStructure.RTC_Minutes=tmp;
        UI_ShowDateTime(8,&RTC_TimeStructure,&RTC_DateStructure);
      }
      else  
      if (key==KEY_CTRL_DOWN)
      {
        tmp=RTC_TimeStructure.RTC_Minutes;
        if (tmp>0) tmp--; else tmp=59;
        RTC_TimeStructure.RTC_Minutes=tmp;
        UI_ShowDateTime(8,&RTC_TimeStructure,&RTC_DateStructure);
      }
      else
      if (key==KEY_CTRL_EXE)
        state=5;
    }
    //LCD_Line(14*6,16,16*6,16,0);
    //LCD_Update();
  }
  RTC_SetDateTime(&RTC_TimeStructure,&RTC_DateStructure);
}

void Mode_main()
{
  RTC_TimeTypeDef RTC_TimeStructure;
  RTC_DateTypeDef RTC_DateStructure;
 
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  
  LCD_Clear(0);
  UI_ShowDateTime(0,&RTC_TimeStructure,&RTC_DateStructure);
  GetKey();
}

int main(void)
{
  SD_Error state;
  uint16_t i;
  uint8_t key;
  
  //USART1_Config();
        
  printf("\r\n\r\nSTM32F2 Development Platform\r\nBuild by Zweb.\r\n");

  PM_SetCPUFreq(168);
  NVIC_Config();
  
  SPI_FLASH_Init();
  SPI_Flash_WAKEUP();
  Delay(5);
  sf_size = SPI_FLASH_GetSize();
  
  LCD_Init();
  LCD_StatusClear();
  LCD_SelectFont((u8 *)Font_Ascii_6X8E);
  printf("LCD Inited.\r\n");
  
  RTC_Config();//初始化实时时钟
  PWR_BackupAccessCmd(ENABLE);//使能备份寄存器操作
  if (RTC_ReadBackupRegister(RTC_BKP_DR1)==0x0001)
  {
    RTC_WriteProtectionCmd(DISABLE);
    RTC_WriteBackupRegister(RTC_BKP_DR1,0x0000);
    PM_EnterStandbyMode();
  }
  brightness = RTC_ReadBackupRegister(RTC_BKP_DR2);
  LCD_SetBrightness(brightness);
  PWR_BackupAccessCmd(DISABLE);
  
  //DAC1_Config();
  //printf("DAC Inited.\r\n");
  //LCD_Clear(0x00);
  
  KBD_Config();
  
  PM_Config();
  
  LCD_Clear(0x00);
  LCD_Update();
  //for (i=0;i<=100;i++)
  //{
  //  LCD_SetBrightness(i);
  //  Delay(1);
  //}
  KBD_TIMConfig();
  KBD_EXTIConfig();
  PM_LDO_On();
  state=SD_Init();
  
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//使能写入PR和RLR
  IWDG_SetPrescaler(IWDG_Prescaler_128);  //写入PR预分频值
  IWDG_SetReload(100);  //写入RLR
  
  PM_TIMConfig();
  IWDG_Enable();//KR写入0xCCCC
  
  PM_SetCPUFreq(16);
  
  while(1)
  {
    //menu_main();
    eigenmath_main();
  }
  
}
