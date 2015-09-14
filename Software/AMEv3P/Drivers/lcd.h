/*******************************************************************************
  ArithMax Open Source Calculator Project
  www.zephray.com
*******************************************************************************/
#ifndef __LCD_H__
#define __LCD_H__

#include "main.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define LCD_D   *(vu16*)((u32)0x60020000)
#define LCD_C   *(vu16*)((u32)0x60000000)

#define LCD_STB_Shift   0
#define LCD_STB_Alpha   4
#define LCD_STB_M       7
#define LCD_STB_STO     13
#define LCD_STB_RCL     19
#define LCD_STB_STAT    26
#define LCD_STB_CMPLX   35
#define LCD_STB_MAT     43
#define LCD_STB_VCT     50
#define LCD_STB_D       56
#define LCD_STB_R       59
#define LCD_STB_G       62
#define LCD_STB_FIX     67
#define LCD_STB_SCI     72
#define LCD_STB_Math    80
#define LCD_STB_Down    86
#define LCD_STB_Up      87
#define LCD_STB_Disp    93

#define LCD_WIDTH       96
#define LCD_HEIGHT      32

extern uint8_t *LCD_Screen;

void LCD_Init(void);
void LCD_SetBrightness(uint8_t value);
void LCD_PowerDown(void);
void LCD_ClearStatus(void);
void LCD_SetStatus(unsigned char id,unsigned char status);
void LCD_SetCursorPos(uint8_t x,uint8_t y);
void LCD_SetCursorEn(uint8_t en);
uint8_t LCD_GetCursorEn(void);
void LCD_Refresh(void);
void LCD_LowLevelRefresh(uint8_t CursorState);
void LCD_SelectBuffer(uint8_t *buf);
uint8_t *LCD_CreateBuffer(uint16_t w, uint16_t h);
void LCD_BlitBuffer(uint8_t *dst, int16_t x, int16_t y);
void LCD_ClearBuffer(void);
void LCD_SetPixel(uint16_t x, uint16_t y, uint8_t c);
uint8_t LCD_GetPixel(uint16_t x, uint16_t y);
void LCD_SetDrawMode(uint8_t overlap, uint8_t color);
void LCD_XLine(uint16_t x0, uint16_t y0, uint16_t x1);
void LCD_YLine(uint16_t x0, uint16_t y0, uint16_t y1);
void LCD_Line(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1);
void LCD_Rect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void LCD_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void LCD_Circle(uint16_t x, uint16_t y, uint16_t r);
void LCD_SetFont(uint8_t* font);
void LCD_String(uint8_t x, uint8_t y, uint8_t* str);


#endif