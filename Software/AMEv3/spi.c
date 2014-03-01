#include "spi.h"

void SPI1_Config(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  //IO初始化
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE); 

  //Configure SPI_SLAVE pins: SCK and MISO
  //Configure SCK and MOSI pins as Input Floating
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //Configure MISO pin as Alternate Function Push Pull
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  SPI_Cmd(SPI1, DISABLE);            //必须先禁能,才能改变MODE
  SPI_InitStructure.SPI_Direction =SPI_Direction_1Line_Rx;  //单线只接收
  SPI_InitStructure.SPI_Mode =SPI_Mode_Slave;       //从
  SPI_InitStructure.SPI_DataSize =SPI_DataSize_8b;  //8位
  SPI_InitStructure.SPI_CPOL =SPI_CPOL_Low;        //CPOL=0  时钟悬空低
  SPI_InitStructure.SPI_CPHA =SPI_CPHA_2Edge;       //CPHA=1 数据捕获第2个
  SPI_InitStructure.SPI_NSS =SPI_NSS_Hard;          //硬件NSS
  SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_2;  //2分频（从模式无效）
  SPI_InitStructure.SPI_FirstBit =SPI_FirstBit_MSB; //高位在前
  SPI_InitStructure.SPI_CRCPolynomial =7;           //CRC7，未用
   
  SPI_Init(SPI1,&SPI_InitStructure);
  SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPI1_Recv(void)
{
  // Wait to receive a byte  
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET);  
  
  // Return the byte read from the SPI bus  
  return SPI_I2S_ReceiveData(SPI1);
}  

void SPI1_ITConfig(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;           //SPI1中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //响应优先级0
  NVIC_Init(&NVIC_InitStructure);
           
  SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);
}
