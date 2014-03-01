#ifndef __OLED_H__
#define __OLED_H__		  	 
#include "main.h"    
//SSD1306 OLED 驱动IC驱动代码
//驱动方式:8080并口/4线串口
//版本:V1.1
//正点原子@ALIENTEK
//2010-6-3

//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
		    						  
//-----------------OLED端口定义----------------  					   
#define OLED_CS_High()   GPIOB->BSRRL = GPIO_Pin_9
#define OLED_CS_Low()    GPIOB->BSRRH = GPIO_Pin_9

#define OLED_RST_High()  GPIOB->BSRRL = GPIO_Pin_3
#define OLED_RST_Low()   GPIOB->BSRRH = GPIO_Pin_3

#define OLED_DC_High()   GPIOB->BSRRL = GPIO_Pin_4
#define OLED_DC_Low()    GPIOB->BSRRH = GPIO_Pin_4

#define OLED_SCLK_High() GPIOB->BSRRL = GPIO_Pin_10
#define OLED_SCLK_Low()  GPIOB->BSRRH = GPIO_Pin_10

#define OLED_SDIN_High() GPIOC->BSRRL = GPIO_Pin_3
#define OLED_SDIN_Low()  GPIOC->BSRRH = GPIO_Pin_3
		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p);	 
#endif  
	 



