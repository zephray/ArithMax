/*******************************************************************************
  ArithMax Open Source Calculator Project
  www.zephray.com
*******************************************************************************/
#include "lcd.h"
#include "ui.h"
#include "timing.h"
#include "fvm.h"
#include "font.h"

uint8_t *UI_Screen;
void * *UI_TempBlk;

void UI_DrawBottomMenu(UI_MENU *menu)
{
  uint8_t x;
  int8_t i;
  
  UI_TempBlk = FVM_Create();
  FVM_SelectBlk(UI_TempBlk);
  UI_Screen = LCD_CreateBuffer(LCD_WIDTH,LCD_HEIGHT+8);
  LCD_SelectBuffer(LCD_Screen);
  LCD_BlitBuffer(UI_Screen,0,0);
  LCD_SelectBuffer(UI_Screen);
  
  LCD_SetDrawMode(0,1);
  LCD_SetFont((unsigned char *)Font_Ascii_Small);
  LCD_Fill(0,LCD_HEIGHT,15,LCD_HEIGHT+7);
  LCD_Fill(16,LCD_HEIGHT,31,LCD_HEIGHT+7);
  LCD_Fill(32,LCD_HEIGHT,47,LCD_HEIGHT+7);
  LCD_Fill(48,LCD_HEIGHT,63,LCD_HEIGHT+7);
  LCD_Fill(64,LCD_HEIGHT,79,LCD_HEIGHT+7);
  LCD_Fill(80,LCD_HEIGHT,95,LCD_HEIGHT+7);
  
  x = 1;
  LCD_SetDrawMode(0,0);
  for (i=0;i<(menu->Count);i++)
  {
    LCD_String(x,LCD_HEIGHT+1,menu->Items[i].ShortName);
    x += 16;
  }
  
  for (i=1;i<8;i++)
  {
    LCD_BlitBuffer(LCD_Screen,0,-i);
    LCD_LowLevelRefresh(0);
    Delay(1);
  }
  
  FVM_SelectBlk(FVM_GlobalBlk);
}

void UI_ProcessMessage()
{
  
}