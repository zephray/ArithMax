#include "main.h"
#include "usart.h"
#include "stn.h"
#include "sdcard.h"
#include "spiflash.h"
#include "key.h"
#include "powerman.h"
//#include "dac.h"

uint8_t brightness=30;
uint32_t sf_size,voltage;

const char* menu_string[]=
{
  "计算器","电子书","游戏","设置"
};

__IO uint8_t ShiftState;//最低位：Shift状态 第二位：Alpha状态

__IO uint32_t FlashID = 0;
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

unsigned int SPI_FLASH_GetSize()
{
  unsigned int size;
  FlashID = SPI_FLASH_ReadID();
  size = mypow(2,(FlashID&0xff));
  return size;
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
  LCD_String_5X7(0,0 ,(unsigned char*)"CPU:STM32F207VC",1);
  LCD_String_5X7(0,8 ,str,1);
  sprintf(str,"SD:%dMB",SDCardInfo.CardCapacity>>20);
  LCD_String_5X7(0,16,str,1); 
  voltage=PM_GetVolt();
  sprintf(str,"Volt:%dmV",voltage);
  LCD_String_5X7(0,24,str,1);
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
    case KEY_CTRL_RIGHT:if (brightness<64) brightness+=1; LCD_SetBrightness(brightness);break;
    case KEY_CTRL_AC:cont=0;break;
    } 
  }
}

void Setup_main()
{
  u8 key;
  
  LCD_Clear(0x00);
  LCD_String_5X7(0,0 ,"1:Deg   2:Rad   ",1);
  LCD_String_5X7(0,8 ,"3:Gra   4:Cmplx ",1);
  LCD_String_5X7(0,16,"5:Cont. 6:Diag. ",1);
  LCD_String_5X7(0,24,"7:Info  8:About ",1);
  LCD_Update();
    key=GetKey();
    switch (key)
    {
    case KEY_CHAR_5:Contrast_main();break;
    case KEY_CHAR_7:Info_main();break;
    case KEY_CHAR_8:About_main();break;
    }
}

void Mode_main()
{
  LCD_Clear(0);
  LCD_String_5X7(0,0 ,"1:COMP  2:CAS   ",1);
  LCD_String_5X7(0,8 ,"3:STAT  4:VECTOR",1);
  LCD_String_5X7(0,16,"5:TABLE 6:GRAPH ",1);
  LCD_String_5X7(0,24,"7:PRGM  8:USRAPP",1);
 LCD_Update();
 GetKey();
}

int main(void)
{
  SD_Error state;
  RCC_ClocksTypeDef RCC_Clocks;
  uint16_t i;
  uint8_t key;
  
  USART1_Config();
        
  printf("\r\n\r\nSTM32F2 Development Platform\r\nBuild by Zweb.\r\n");
  printf("Ready to turn on the Systick.\r\n");
        
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
  
  printf("Systick opened successfully.\r\n");
  printf("Main Clock Frequency: %d MHz\r\n",(RCC_Clocks.HCLK_Frequency/1000000));

  NVIC_Config();
  
  //DAC1_Config();
  //printf("DAC Inited.\r\n");
  LCD_Init();
  LCD_StatusClear();
  LCD_SelectFont(Font_Ascii_5X7E_Menu);
  printf("LCD Inited.\r\n");
  //LCD_Clear(0x00);
  SPI_FLASH_Init();
  Delay(5);
  sf_size = SPI_FLASH_GetSize();
  
 /*打印芯片的ID及设备ID*/
  printf("SPIFLASH芯片ID为：0x%X , 容量为%d KB \r\n", FlashID, (sf_size>>10));
  
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
  
  while(1)
  {
    //menu_main();
    eigenmath_main();
  }
  
}
