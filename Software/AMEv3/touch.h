#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "main.h"

void Touch_Config(void);
uint8_t Touch_Detect(void);
void Touch_Measure_Filter(void);

extern uint16_t x_ori,y_ori;

#endif
