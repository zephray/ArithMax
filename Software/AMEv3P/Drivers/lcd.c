/*******************************************************************************
  ArithMax Open Source Calculator Project
  www.zephray.com
*******************************************************************************/
#include "lcd.h"
#include "font.h"
#include "timing.h"
#include "fvm.h"

uint8_t* LCD_Screen;

uint8_t LCD_CursorEn = 0;
uint8_t LCD_CursorX = 0;
uint8_t LCD_CursorY = 0;
uint8_t* LCD_TargetBuffer = NULL;
uint8_t LCD_RefreshFlag = 0;
uint8_t LCD_DrawColor = 0;
uint8_t LCD_Overlap = 1;
uint8_t* LCD_CurrFont = NULL;

void LCD_Reset(void) 
{
  GPIO_ResetBits(GPIOD, GPIO_Pin_6);
  Delay(1);					   
  GPIO_SetBits(GPIOD, GPIO_Pin_6);		 	 
  Delay(1);	
}

void inline LCD_WriteCmd(uint16_t index) 
{
  LCD_C = index;
}

void LCD_WriteReg(uint16_t index, uint16_t val) 
{
  LCD_C = index;
  LCD_D = val;
}

void inline LCD_WriteDat(uint16_t val) 
{
  LCD_D = val;
}

void LCD_Init(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;

  //Enable the FSMC Clock
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE); 
  //Enable GPIOs clock
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE,ENABLE);
     
  //config tft data lines base on FSMC	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_14| GPIO_Pin_15;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
  //config tft control lines base on FSMC
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_11; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    
  //config tft rst gpio
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 	 
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
   
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);	                //配置GPIOD的Pin0复用连接到FSMC
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);			//配置GPIOD的Pin1复用连接到FSMC
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);			//配置GPIOD的Pin4复用连接到FSMC
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);			//配置GPIOD的Pin5复用连接到FSMC
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource7,GPIO_AF_FSMC);			//配置GPIOD的Pin7复用连接到FSMC
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource11,GPIO_AF_FSMC);		//配置GPIOD的Pin11复用连接到FSMC
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);		//配置GPIOD的Pin14复用连接到FSMC
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);		//配置GPIOD的Pin15复用连接到FSMC
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);			//配置GPIOE的Pin7复用连接到FSMC
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);			//配置GPIOE的Pin8复用连接到FSMC
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);			//配置GPIOE的Pin9复用连接到FSMC
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);		//配置GPIOE的Pin10复用连接到FSMC
   
  p.FSMC_AddressSetupTime = 0x04;	        //地址建立时间
  p.FSMC_AddressHoldTime = 0x04;	        //地址保持时间
  p.FSMC_DataSetupTime = 0x04;  		//数据建立时间
  p.FSMC_BusTurnAroundDuration = 0x00;
  p.FSMC_CLKDivision = 0x00;
  p.FSMC_DataLatency = 0x00;
  p.FSMC_AccessMode = FSMC_AccessMode_B;	 // 一般使用模式B来控制LCD   

  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p; 
    
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    
  //Enable FSMC Bank1_SRAM Bank
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
  
  LCD_Reset();
  
  LCD_WriteCmd(0xe2);//system reset
  Delay(5);   
  LCD_WriteCmd(0xae);        
  LCD_WriteCmd(0xf8);   
  LCD_WriteCmd(0x00);
  LCD_WriteCmd(0xa2);
  LCD_WriteCmd(0xa0);
  LCD_WriteCmd(0xc0);
  LCD_WriteCmd(0xa6);
  LCD_WriteCmd(0xa4);
  LCD_WriteCmd(0xe0);
  LCD_WriteCmd(0xac);
  LCD_WriteCmd(0x00);
  LCD_WriteCmd(0x21);
  LCD_WriteCmd(0x81);
  LCD_WriteCmd(0x1e);        
  LCD_WriteCmd(0x2f);   
  LCD_WriteCmd(0x40);
  Delay(5);
  LCD_WriteCmd(0xaf);
  
  LCD_Screen = LCD_CreateBuffer(96,32);
}

void LCD_PowerDown(void) 
{
  LCD_WriteCmd(0xae);
  LCD_WriteCmd(0x28);
  LCD_WriteCmd(0xa5);
}

void LCD_SetBrightness(uint8_t value) 
{
  LCD_WriteCmd(0x81);
  LCD_WriteCmd(value);
}

void LCD_ClearStatus(void) 
{
  unsigned char i;
  LCD_WriteCmd(0xee);
  LCD_WriteCmd(0xb8);
  LCD_WriteCmd(0x10);
  LCD_WriteCmd(0x00);
  LCD_WriteCmd(0xe0); 
  for (i=0;i<96;i++)
    LCD_WriteDat(0x00);
}

void LCD_SetStatus(unsigned char id,unsigned char status) 
{
  LCD_WriteCmd(0xee);
  LCD_WriteCmd(0xb8);
  LCD_WriteCmd(0x10|(id>>4));
  LCD_WriteCmd(0x00|(id&0x0f));
  LCD_WriteCmd(0xe0);
  LCD_WriteDat(status);
}

void LCD_SetCursorPos(uint8_t x,uint8_t y) 
{
  if ((x<96)&&(y<4))
  {
    LCD_CursorX = x;
    LCD_CursorY = y;
  }
}

void LCD_SetCursorEn(uint8_t en) 
{
  LCD_CursorEn = en;
}

uint8_t LCD_GetCursorEn(void) 
{
  return LCD_CursorEn;
}

void LCD_SelectBuffer(uint8_t *buf) 
{
  LCD_TargetBuffer = buf;
}

uint8_t *LCD_CreateBuffer(uint16_t w, uint16_t h) 
{
  uint8_t *fb;
  uint16_t i;
  h = (h+7)/8;
  fb = (u8 *)FVM_Malloc(w*h+4);
  fb[0]=(w>>8);
  fb[1]=(w&0xFF);
  fb[2]=(h>>8);
  fb[3]=(h&0xFF);
  for (i=4;i<4+w*h;i++)
  {
    fb[i]=0x00;
  }
  return fb;
}

void LCD_Refresh(void) 
{
  LCD_RefreshFlag = 1;
}

uint8_t LCD_GetRefreshFlag(void) 
{
  return LCD_RefreshFlag;
}

void LCD_LowLevelRefresh(uint8_t CursorState) 
{
  uint8_t i,j,rx;
  
  for (i=0;i<4;i++)
  {
    LCD_WriteCmd(0xee);
    LCD_WriteCmd(0xb0|(3-i));
    LCD_WriteCmd(0x10);
    LCD_WriteCmd(0x00);
    LCD_WriteCmd(0xe0); 
    for (j=0;j<96;j++)
      LCD_WriteDat(LCD_Screen[4+i*96+j]);
  }
  LCD_RefreshFlag = 0;
  
  if ((LCD_CursorEn)&&(CursorState))
  {
    rx=LCD_CursorX;
    LCD_WriteCmd(0xee);
    LCD_WriteCmd(0xb0|(3-LCD_CursorY));
    LCD_WriteCmd(0x10|(rx>>4));
    LCD_WriteCmd(0x00|(rx&0x0f));
    LCD_WriteCmd(0xe0);
    LCD_WriteDat(0xFF);
    LCD_WriteDat(0xFF);
  }
}

void LCD_BlitBuffer(uint8_t *dst, int16_t x, int16_t y) 
{
  int16_t fb_w,fb_h;
  int16_t i,j,k;
  uint8_t c;
  uint8_t *src = LCD_TargetBuffer;
  uint8_t *bak = src;

  LCD_TargetBuffer = dst;
  fb_w = *src;src++;
  fb_w = fb_w<<8 | *src;src++;
  fb_h = *src;src++;
  fb_h = fb_h<<8 | *src;src++;
  for (i=0;i<fb_h;i++)
    for (j=0;j<fb_w;j++)
    {
      if (j+x>=0) {
        c = *src;
        for (k=0;k<8;k++)
        {
          if (i*8+k+y>=0)
            LCD_SetPixel(j+x,i*8+k+y,c&0x80);
          c = c << 1;
        }
      }
      src++;
    }
  LCD_TargetBuffer = bak;
}

void LCD_ClearBuffer(void) 
{
  uint16_t w,h,i;
  uint8_t *fb = LCD_TargetBuffer;
  
  w = ((fb[0])<<8)|fb[1];
  h = ((fb[2])<<8)|fb[3];
  for (i=4;i<4+w*h;i++)
  {
    fb[i]=LCD_DrawColor;
  }
}

void LCD_SetPixel(uint16_t x, uint16_t y, uint8_t c) 
{
  int16_t y_byte,y_bit,pos;
  int16_t fb_w,fb_h;
  uint8_t *fb = LCD_TargetBuffer;

  fb_w = *fb;fb++;
  fb_w = fb_w<<8 | *fb;fb++;
  fb_h = *fb;fb++;
  fb_h = fb_h<<8 | *fb;fb++;
  y_byte = y/8;
  y_bit  = y%8;
  pos = y_byte*fb_w+x;

  if (pos < fb_h*fb_w)
  {
    if (c==0)
    {
      fb[pos] &=~(0x80>>y_bit);
    }else{
      fb[pos] |= (0x80>>y_bit);
    }
  } 
}
  
uint8_t LCD_GetPixel(uint16_t x, uint16_t y) 
{
  int16_t y_byte,y_bit,pos;
  int16_t fb_w,fb_h;
  uint8_t c;
  uint8_t *fb = LCD_TargetBuffer;
  
  fb_w = *fb;fb++;
  fb_w = fb_w<<8 | *fb;fb++;
  fb_h = *fb;fb++;
  fb_h = fb_h<<8 | *fb;fb++;
  y_byte = y/8;
  y_bit  = y%8;
  pos = y_byte*fb_w+x;

  if (pos < fb_h*fb_w)
  {
    c=fb[pos];
    if ((c<<y_bit)&0x80)
      return 1;
    else
      return 0;
  }
  return 0;
}

void LCD_SetDrawMode(uint8_t overlap, uint8_t color) 
{
  LCD_Overlap = overlap;
  LCD_DrawColor = color;
}

void LCD_XLine(uint16_t x0, uint16_t y0, uint16_t x1) 
{
  uint16_t i,xx0,xx1;

  xx0=MIN(x0,x1);
  xx1=MAX(x0,x1);
  for (i=xx0;i<=xx1;i++)
  {
    LCD_SetPixel(i,y0,LCD_DrawColor);
  }
}

void LCD_YLine(uint16_t x0, uint16_t y0, uint16_t y1) 
{
  uint16_t i,yy0,yy1;

  yy0=MIN(y0,y1);
  yy1=MAX(y0,y1);
  for (i=yy0;i<=yy1;i++)
  {
    LCD_SetPixel(x0,i,LCD_DrawColor);
  }
}

void LCD_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) 
{
  int16_t temp;
  int16_t dx,dy;            //定义起点到终点的横、纵坐标增加值
  int16_t s1,s2,status,i;
  int16_t Dx,Dy,sub;

  dx=x1-x0;
  if(dx>=0)                //X的方向是增加的
    s1=1;
  else                    //X的方向是降低的
    s1=-1;
  dy=y1-y0;                //判断Y的方向是增加还是降到的
  if(dy>=0)
    s2=1;
  else
    s2=-1;

  Dx=abs(x1-x0);           //计算横、纵标志增加值的绝对值
  Dy=abs(y1-y0);
  if(Dy>Dx)                //
  {                     //以45度角为分界线，靠进Y轴是status=1,靠近X轴是status=0
    temp=Dx;
    Dx=Dy;
    Dy=temp;
    status=1;
  }
  else
    status=0;

/********判断垂直线和水平线********/
  if(dx==0)                   //横向上没有增量，画一条水平线
    LCD_YLine(x0,y0,y1);
  if(dy==0)                   //纵向上没有增量，画一条垂直线
    LCD_XLine(x0,y0,x1);


/*********Bresenham算法画任意两点间的直线********/
  sub=2*Dy-Dx;                 //第1次判断下个点的位置
  for(i=0;i<Dx;i++)
  {
    LCD_SetPixel(x0,y0,LCD_DrawColor);           //画点
    if(sub>=0)
    {
      if(status==1)               //在靠近Y轴区，x值加1
        x0+=s1;
      else                     //在靠近X轴区，y值加1
        y0+=s2;
      sub-=2*Dx;                 //判断下下个点的位置
    }
    if(status==1)
      y0+=s2;
    else
      x0+=s1;
    sub+=2*Dy;
  }
}

void LCD_Rect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) 
{
  LCD_XLine(x0,y0,x1);
  LCD_XLine(x0,y1,x1);
  LCD_YLine(x0,y0,y1);
  LCD_YLine(x1,y0,y1);
}

void LCD_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) 
{
  uint16_t x,y;
  for (x=x0;x<x1;x++)
    for (y=y0;y<y1;y++)
      LCD_SetPixel(x,y,LCD_DrawColor);
}

void LCD_Circle(uint16_t x, uint16_t y, uint16_t r) 
{
  int16_t a,b;
  int16_t di;
  
  a=0;
  b=r;
  di=3-2*r;             //判断下个点位置的标志
  while(a<=b)
  {
    LCD_SetPixel(x-b,y-a,LCD_DrawColor);             //3           
    LCD_SetPixel(x+b,y-a,LCD_DrawColor);             //0           
    LCD_SetPixel(x-a,y+b,LCD_DrawColor);             //1       
    LCD_SetPixel(x-b,y-a,LCD_DrawColor);             //7           
    LCD_SetPixel(x-a,y-b,LCD_DrawColor);             //2             
    LCD_SetPixel(x+b,y+a,LCD_DrawColor);             //4               
    LCD_SetPixel(x+a,y-b,LCD_DrawColor);             //5
    LCD_SetPixel(x+a,y+b,LCD_DrawColor);             //6 
    LCD_SetPixel(x-b,y+a,LCD_DrawColor);             
    a++;
    /***使用Bresenham算法画圆**/     
    if(di<0)
      di +=4*a+6;
    else
    {
      di +=10+4*(a-b);   
      b--;
    } 
    LCD_SetPixel(x+a,y+b,LCD_DrawColor);
  }
}

void LCD_SetFont(uint8_t* font) 
{
  LCD_CurrFont = font;
}

void LCD_Char(uint16_t x, uint16_t y, uint8_t chr) 
{
  uint16_t x0,y0,b;
  uint8_t w,h,c;
  uint16_t ptr;

  if ((chr=='\r')||(chr=='\n')) chr=' ';
  w = LCD_CurrFont[0];
  h = LCD_CurrFont[1];
  if ((0x20<=chr)&&(chr<=0x7F))
    ptr=2+(chr-0x20)*(w*h+1);
  w = LCD_CurrFont[ptr++];
  for (x0=0;x0<w;x0++)
  {
    for (y0=0;y0<h;y0++)
    {
      c = LCD_CurrFont[ptr++];
      for (b=0;b<8;b++)
      {
        if ((c>>b)&0x01)
          LCD_SetPixel(x+x0,y+y0*8+b,LCD_DrawColor);
        else if (LCD_Overlap)
          LCD_SetPixel(x+x0,y+y0*8+b,1-LCD_DrawColor);
      }
    }
  }
}

uint8_t LCD_GetCharWidth(uint8_t chr)
{
  uint16_t ptr;
  uint8_t w,h;
  
  w = LCD_CurrFont[0];
  h = LCD_CurrFont[1];
  ptr = 0;
  if ((0x20<=chr)&&(chr<=0x7F))
    ptr = 2+(chr-0x20)*(w*h+1);
  
  return LCD_CurrFont[ptr];
}

void LCD_String(uint8_t x, uint8_t y, uint8_t* str) 
{
  uint16_t x0;
  
  x0=0;
  while(*str)
  {
    if (*str<128)
    {
      LCD_Char(x+x0,y,*str);
      x0 += LCD_GetCharWidth(*str++) + 1;
    }
    else
    {
      //Not supported
    }
  }
}