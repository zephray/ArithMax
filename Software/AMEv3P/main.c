/*******************************************************************************
  ArithMax Open Source Calculator Project
  www.zephray.com
*******************************************************************************/
#include "main.h"
#include "lcd.h"
#include "timing.h"
#include "fvm.h"
#include "ui.h"
#include "font.h"

void SwitchToRad()
{
  //
}

UI_MENU MainMenu = {
  3,
  {
    {"RAD",&SwitchToRad,"Switch To Rad"},
    {"DEG",&SwitchToRad,"Switch To Deg"},
    {"GRA",&SwitchToRad,"Switch To Gra"},
  }
};

int main(void)
{
  TIME_Init();
  FVM_Init();
  LCD_Init();
  LCD_SelectBuffer(LCD_Screen);
  LCD_SetDrawMode(0,1);
  LCD_SetFont((unsigned char *)Font_Ascii_Medium);
  //LCD_String(0,1,"the quick brown fox jumps over the lazy dog");
  LCD_String(0,1,"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG");
  LCD_LowLevelRefresh(0);
  
  UI_DrawBottomMenu(&MainMenu);
  
  while(1)
  {
  }
  
}
