#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"

extern uint8_t __IO LastKey;
extern uint8_t __IO NowKey;

void KBD_Config(void);
uint8_t KBD_Scan(void);
void KBD_TIMConfig(void);
void WaitForKey(void);
unsigned char GetKey(void);

#define KEY_CTRL_LEFT           4
#define KEY_CTRL_RIGHT          1
#define KEY_CTRL_UP             2
#define KEY_CTRL_DOWN           3
#define KEY_CTRL_DEL            36
#define KEY_CTRL_AC             35
#define KEY_CTRL_MODE           0
#define KEY_CTRL_SETUP          0+128
#define KEY_CTRL_EXE            48
#define KEY_CTRL_FD             29
#define KEY_CHAR_PLUS           40
#define KEY_CHAR_MINUS          47
#define KEY_CHAR_MULT           43
#define KEY_CHAR_DIV            42
#define KEY_CHAR_POW            16
#define KEY_CHAR_SQUARE         17
#define KEY_CHAR_ROOT           18
#define KEY_CHAR_CUBEROOT       18+128
#define KEY_CHAR_POWROOT        16+128
#define KEY_CHAR_LN             14
#define KEY_CHAR_EXPN           14+128
#define KEY_CHAR_SIN            23
#define KEY_CHAR_ASIN           23+128
#define KEY_CHAR_COS            22
#define KEY_CHAR_ACOS           22+128
#define KEY_CHAR_TAN            21
#define KEY_CHAR_ATAN           21+128
#define KEY_CHAR_FRAC           19
#define KEY_CHAR_FACT           8+128
#define KEY_CHAR_IMGNRY         32+64
#define KEY_CHAR_PI             34+128
#define KEY_CHAR_EXP            34
#define KEY_CHAR_PMINUS         26
#define KEY_CHAR_ANS            41
#define KEY_CHAR_ENG            32

#define KEY_CHAR_LB             31
#define KEY_CHAR_RB             30
#define KEY_CHAR_CO             27
#define KEY_CHAR_e              34+64

#define KEY_CHAR_0              20
#define KEY_CHAR_1              13
#define KEY_CHAR_2              12
#define KEY_CHAR_3              11
#define KEY_CHAR_4              46
#define KEY_CHAR_5              45
#define KEY_CHAR_6              44
#define KEY_CHAR_7              39
#define KEY_CHAR_8              38
#define KEY_CHAR_9              37
#define KEY_CHAR_A              26+64
#define KEY_CHAR_B              25+64
#define KEY_CHAR_C              24+64
#define KEY_CHAR_D              23+64
#define KEY_CHAR_E              22+64
#define KEY_CHAR_F              21+64
#define KEY_CHAR_G              33+64
#define KEY_CHAR_H              32+64
#define KEY_CHAR_I              31+64
#define KEY_CHAR_J              30+64
#define KEY_CHAR_K              29+64
#define KEY_CHAR_L              28+64
#define KEY_CHAR_M              39+64
#define KEY_CHAR_N              38+64
#define KEY_CHAR_O              37+64
#define KEY_CHAR_P              36+64
#define KEY_CHAR_Q              35+64
#define KEY_CHAR_R              46+64
#define KEY_CHAR_S              45+64
#define KEY_CHAR_T              44+64
#define KEY_CHAR_U              43+64
#define KEY_CHAR_V              42+64
#define KEY_CHAR_W              13+64
#define KEY_CHAR_X              12+64
#define KEY_CHAR_Y              11+64
#define KEY_CHAR_Z              40+64
#define KEY_CHAR_EQUAL          10+64
#define KEY_CHAR_COMMA          30+128

#endif
