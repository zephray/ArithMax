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
 LCD_String_5X7(0,0 ,(unsigned char*)"This calc. is   ",1);
 LCD_String_5X7(0,8 ,(unsigned char*)"Designed by Zweb",1);
 LCD_String_5X7(0,16,(unsigned char*)"of Ningbo High  ",1);
 LCD_String_5X7(0,24,(unsigned char*)"School.         ",1);
 LCD_Update();
 GetKey();
}

void Info_main()
{
  char str[17]="Designed by Zweb";
  sprintf(str,"Flash:%dKB",(sf_size>>10));
  LCD_Clear(0);
  LCD_String_5X7(0,0 ,(u8 *)"CPU:STM32F207VC",1);
  LCD_String_5X7(0,8 ,(u8 *)str,1);
  sprintf(str,"SD:%dMB",SDCardInfo.CardCapacity>>20);
  LCD_String_5X7(0,16,(u8 *)str,1); 
  voltage=PM_GetVolt();
  sprintf(str,"Volt:%dmV",voltage);
  LCD_String_5X7(0,24,(u8 *)str,1);
  LCD_Update();
  GetKey();
}
  
void Contrast_main()
{
      u8 cont=1;
      u8 key;
      
      LCD_Clear(0x00);
  LCD_String_5X7(0,0 ,"CONTRAST        ",1);
  LCD_String_5X7(0,16,"LIGHT       DARK",1);
  LCD_String_5X7(0,24," [<]        [>] ",1);
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
  }
}

void Diag_main()
{
  u8 key;
  u16 i;
  LCD_Clear(0);
  LCD_String_5X7(0,1 ,"DIAGNOSTIC",1);
  LCD_String_5X7(0,24,"Press 9 To Start",1);
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
  LCD_String_5X7(0,1 ,"ArithMax E300",1);
  LCD_String_5X7(0,8 ,"EVT For DEV Only",1);
  LCD_String_5X7(0,24,"Press AC",1);
  LCD_Update();
  WaitForCertainKey(KEY_CTRL_AC);
  Contrast_main();
  LCD_Clear(0);
  LCD_Update();
  while (key!=KEY_CTRL_EXE)
  {
    key=GetKey();
    LCD_DispNum_5X7(0,0,key,3,1);
    LCD_Update();
  }
}


void Setup_main()
{
  u8 key;
  u8 cont =1;
  
  LCD_Clear(0x00);
  LCD_String_5X7(0,0 ,"1:Deg   2:Rad   ",1);
  LCD_String_5X7(0,8 ,"3:Gra   4:Cmplx ",1);
  LCD_String_5X7(0,16,"5:Cont. 6:Diag. ",1);
  LCD_String_5X7(0,24,"7:Info  8:About ",1);
  LCD_Update();
  while (cont==1)
  {
    key=GetKey();
    cont=0;
    switch (key)
    {
    case KEY_CHAR_6:Diag_main();break;
    case KEY_CHAR_5:Contrast_main();break;
    case KEY_CHAR_7:Info_main();break;
    case KEY_CHAR_8:About_main();break;
    case KEY_CTRL_AC:cont=0;break;
    default:LCD_StatusSet(LCD_STB_Disp,1);Delay(10);LCD_StatusSet(LCD_STB_Disp,0);cont=1;break;
    }
  }
}

void Mode_main()
{
  LCD_Clear(0);
  LCD_String_5X7(0,0 ,"1:CAS   2:STAT  ",1);
  LCD_String_5X7(0,8,"3:TABLE 4:GRAPH ",1);
  LCD_String_5X7(0,16,"5:PRGM  6:USRAPP",1);
  LCD_Update();
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
  LCD_SelectFont((u8 *)Font_Ascii_5X7E);
  printf("LCD Inited.\r\n");
  
  RTC_Config();//初始化实时时钟
  PWR_BackupAccessCmd(ENABLE);//使能备份寄存器操作
  if (RTC_ReadBackupRegister(RTC_BKP_DR1)==0x0001)
  {
    RTC_WriteProtectionCmd(DISABLE);
    RTC_WriteBackupRegister(RTC_BKP_DR1,0x0000);
    PM_EnterStandbyMode();
  }
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
