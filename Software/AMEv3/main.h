#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f4xx.h"
#include <stdio.h>

extern __IO uint8_t ShiftState;
extern u32 voltage;
extern const unsigned char gImage_mainlogo[384];
extern const unsigned char Font_Ascii_5X7E_Menu[];
extern const unsigned char Font_Ascii_5X7E[];

void Setup_main();
void About_main();
void Mode_main();

/* Exported macro ------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);

#endif
