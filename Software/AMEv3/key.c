#include "key.h"
#include "stn.h"

uint8_t __IO LastKey = 255;
uint8_t __IO NowKey = 255;

void KBD_Config(void)
{
  //IO初始化
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);

  //PA1-PA7输出
  GPIO_InitStructure.GPIO_Pin = 0x00fe;
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //PE0-PE6输入
  GPIO_InitStructure.GPIO_Pin = 0x007f;
  GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  ShiftState=0x00;
}

void KBD_EXTIConfig(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
  
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_Init(&EXTI_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
}

void KBD_TIMConfig(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  uint16_t PrescalerValue = 0;
  
  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  
  /* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
  
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) 0x200;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 0x400;
  TIM_TimeBaseStructure.TIM_Prescaler = 0x200;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  
  /* TIM IT enable */
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
  
  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);
}

void DelayUs(u32 us)
{
    u32 time=100*us/7;   
    while(--time);  
}

uint8_t KBD_Scan(void)
{
  uint8_t i;
  uint32_t line,rowb,rowa;
  uint8_t key;
  
  GPIOA->ODR &= 0xffffff00;//PA0-PA7置0
  line = 0x00000002;//从PA1开始扫描
  key=255;
  for(i=0;i<7;i++)
  {
    GPIOA->ODR |= 0xff;
    GPIOA->ODR &= (~line);//对应脚置0
    //Delay(1);//等待IO状态变化
    //asm("nop");
		//_nop_();
    rowb=0xff;
    rowa=0x00;
    while(rowb!=rowa)
    {
      rowb = (GPIOE->IDR)&0x7f;//读出相应数据
      //Delay(1);//10ms去抖
      DelayUs(1);
      rowa = (GPIOE->IDR)&0x7f;//再次读出
      if (rowb==rowa) //状态相同
      switch(rowb)
      {
        case 0x7e:key=0*7+i;break;
        case 0x7d:key=1*7+i;break;
        case 0x7b:key=2*7+i;break;
        case 0x77:key=3*7+i;break;
        case 0x6f:key=4*7+i;break;
        case 0x5f:key=5*7+i;break;
        case 0x3f:key=6*7+i;break;
      }
    }
    line = line << 1;//左移一位
  }
  return key;
}

void WaitForKey(void)
{
  uint8_t key=255;
  while (key==255)
  {
    key=LastKey;
  }
}

void WaitForCertainKey(u8 ck)
{
  u8 key=255;
  while (key!=ck)
    key=GetKey();
}

unsigned char GetKey(void)
{
  uint8_t key=255;
  uint8_t keyB=0;
  keyB=LastKey;
  key=NowKey;
  while (key==keyB) key=NowKey;
  while (key==255)
  {
    key=NowKey;
  }
  if ((key!=0x06)&&(key!=0x05))
  {
    key=NowKey|ShiftState; 
    ShiftState=0;
    LCD_StatusSet(LCD_STB_Alpha,0);
    LCD_StatusSet(LCD_STB_Shift,0);
  }
  /*while (keyB!=255)
  {
    keyB=NowKey;
  }*/ 
  return key;
}