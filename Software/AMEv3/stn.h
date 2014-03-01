#ifndef __STN_H__
#define __STN_H__

#include "main.h"

#define Bank1_LCD_D      ((u32) 0x60020000) /* RS = 1 */
#define Bank1_LCD_C      ((u32) 0x60000000) /* RS = 0 */

#define LCD_D   *(vu16*)((u32)0x60020000)  //disp Data ADDR
#define LCD_C   *(vu16*)((u32)0x60000000)	 //disp Reg  ADDR

extern uint8_t LCD_FB[96*32/8]; //LCD Framebuffer 

#define LCD_STB_Shift   1
#define LCD_STB_Alpha   5
#define LCD_STB_M       9
#define LCD_STB_STO     14
#define LCD_STB_RCL     19
#define LCD_STB_STAT    27
#define LCD_STB_CMPLX   35
#define LCD_STB_MAT     44
#define LCD_STB_VCT     49
#define LCD_STB_D       55
#define LCD_STB_R       59
#define LCD_STB_G       62
#define LCD_STB_FIX     67
#define LCD_STB_SCI     72
#define LCD_STB_Math    78
#define LCD_STB_Down    84
#define LCD_STB_Up      86
#define LCD_STB_Disp    92
   
u32 mypow(u8 m,u8 n);

void LCD_Init(void);
void LCD_PowerSave();
void LCD_Update();
void LCD_StatusSet(unsigned char id,unsigned char status);
void LCD_DispBmp(u8 x0,u8 y0,u8 x1,u8 y1,u8 *pic);
void LCD_Fill(u8 x0,u8 y0,u8 x1,u8 y1,u8 c);
void LCD_Display_ASCII_8X16_Chr(u16 left,u16 top,u8 chr,u16 color);
void LCD_Display_ASCII_5X7_Chr(u16 left,u16 top,u8 chr,u16 color);
void LCD_Display_ASCII_8X16(u16 left,u16 top,u8 *s,u16 color);
//void LCD_Display_ASCII_24X48_Chr(u8 left,u8 top,u8 chr);
void LCD_Display_Chn_Chr(u8 left,u8 top,u8 *chr,u16 color);
void LCD_String(u16 left,u16 top,u8 *s,u16 color);
void LCD_String_5X7(u16 left,u16 top,u8 *s,u16 color);
void LCD_DispNum(u16 x, u16 y, s32 num, u8 len,u16 color);
//打点
//参数：left 水平位置，top 垂直位置，color 颜色（0或1）
void LCD_Point(u8 x,u8 y,u8 color);
//画一条水平线
//参数：x0 起始横坐标，y0 起始纵坐标，x1 结束横坐标，color 颜色（0或1）
void LCD_XLine(u16 x0,u16 y0,u16 x1,u16 color);
//画一条竖直线
//参数：x0 起始横坐标，y0 起始纵坐标，y1 结束纵坐标，color 颜色（0或1）
void LCD_YLine(u16 x0,u16 y0,u16 y1,u16 color);
//采用布兰森汉姆(Bresenham)算法画线
//参数：x0 起始横坐标，y0 起始纵坐标，x1 结束横坐标，y1 结束纵坐标，color 颜色（0或1）
void LCD_Line(u16 x0,u16 y0,u16 x1,u16 y1,u16 color);
//画圆
//参数：x0 水平位置，y0 垂直位置，r半径，color 颜色（0或1）
void LCD_Circle(u16 x0,u16 y0,u16 r,u16 color);
void LCD_Clear(u8 c);

#endif