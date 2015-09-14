/*******************************************************************************
  ArithMax Open Source Calculator Project
  www.zephray.com
*******************************************************************************/
#ifndef __FVM_H__
#define __FVM_H__

#define FVM_BlkSize     32

extern void * *FVM_GlobalBlk;
extern void * *FVM_CurrBlk;

void FVM_Init(void);
void * *FVM_Create(void);
void FVM_SelectBlk(void * * memblk);
void * *FVM_FindSlot(void *memblk[]);
void *FVM_Malloc(unsigned int size);
void FVM_FreeBlk(void * *memblk);
void FVM_FreeCurr();

#endif