//pccon.c
//与上位机通讯

#include "pccon.h"
#include "lcd.h"
#include "usart.h"
#include "spiflash.h"

u8 RecBuf[4096];
//u8 Buf2[256];
//u8 Buf_Ok;
//u8 WB1_Ok;
//u8 WB2_Ok;
//u8 Free_Buf_No;
DMA_InitTypeDef DMA_InitStructure;

void PCCon_DisableUSARTIRQ()
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void PCCon_EnableUSARTIRQ()
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void PCCon_EnableDMA()
{  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
  /* DMA1 Channel5 (triggered by USART1 Rx event) Config*///参见 STM32 datasheet 
  DMA_DeInit(DMA2_Stream5); 

  DMA_InitStructure.DMA_Channel = DMA_Channel_4;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; 
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)RecBuf; 
  DMA_InitStructure.DMA_BufferSize = 4096; 
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR; 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  
  DMA_Init(DMA2_Stream5, &DMA_InitStructure); 
  
  /*NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);*/
  
  USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);//启动DMA
  
  //DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);
  /* Enable DMA1 Channel5 */ 
  DMA_Cmd(DMA2_Stream5, ENABLE); 
}

void PCCon_DisableDMA()
{
  USART_DMACmd(USART1, USART_DMAReq_Rx, DISABLE); 
  DMA_Cmd(DMA2_Stream5, DISABLE); 
}

void PCCon_RecData()
{
  u16 dtr,i;
  
  dtr=4096;
  i=0;
  while (i<dtr)
  if((USART1->SR & USART_IT_RXNE) != (uint16_t)RESET)
  {
      RecBuf[i] = (USART1->DR & (uint16_t)0x01FF);
      i++;
      USART_ClearITPendingBit(USART1,USART_IT_RXNE);
  }
}

void PCCon_Start()
{
  u8 LastCh;
  u8 i;
  u32 CurrAddr;
  u8 needContinue=1;
  u16 ss,size;
  FunctionalState DmaEnableStatus;
  //初始化连接
  PCCon_DisableUSARTIRQ();
  USART1_PutCh(0x02);
  //初始化完成，开始消息处理循环
  while (needContinue)
  {
    LastCh=USART1_GetCh();
    if (LastCh==0x50)//字库写入
    { 
      //PCCon_EnableDMA();//初始化DMA
      for (i=0;i<53;i++)
      {
        USART1->DR = (0x52 & (uint16_t)0x01FF);
        PCCon_RecData();
        CurrAddr=i*4096;//1个Page 4096字节       
        SPI_FLASH_SectorErase(CurrAddr);
        //while((DMA2_Stream5->NDTR)!=0);
        SPI_FLASH_BufferWrite(RecBuf,CurrAddr,4096);
      }
      USART1_PutCh(0x53);
      //PCCon_DisableDMA();
    }
    if (LastCh==0x40)//批量任意写入
    {
      USART1->DR = (0x41 & (uint16_t)0x01FF);//请求地址和大小
      ss=USART1_WaitCh();
      ss<=8;
      ss|=USART1_WaitCh();
      size=USART1_WaitCh();
      size<=8;
      size|=USART1_WaitCh();
      for(i=ss;i<ss+size;i++)
      {
        USART1->DR = (0x42 & (uint16_t)0x01FF);
        PCCon_RecData();
        CurrAddr=i*4096;//1个Page 4096字节       
        SPI_FLASH_SectorErase(CurrAddr);
        //while((DMA2_Stream5->NDTR)!=0);
        SPI_FLASH_BufferWrite(RecBuf,CurrAddr,4096);
      }
      USART1_PutCh(0x43);
    }
    if (LastCh==0x03)
    { needContinue=0; }
  }
  USART1_PutCh(0x04);
}
