#include "gpio.h"

void GPIO_ModeSelect_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  //开启端口（PB0）的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 

  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t GPIO_ModeSelect_Read(void)
{
  uint8_t mode;

  mode = (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)<<1)|GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
  return mode;
}

void GPIO_RS_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  //开启端口（PB0）的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

  //初始化RS引脚
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  //初始化BUSY引脚
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
}