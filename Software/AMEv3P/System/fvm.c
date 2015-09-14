/*******************************************************************************
  ArithMax Open Source Calculator Project
  www.zephray.com
*******************************************************************************/
#include "main.h"
#include "fvm.h"

//为了避免出现内存泄露，所有分配的内存被统一管理，保存在一种叫memblk的结构中，
//其实就是一个32*4=128字节大小的数组，其中前31*4字节用来保存分配的内存首地址，
//最后4个字节用来保存下一个memblk的地址，当下一个memblk不存在时为NULL
//注意所有memblk都为动态分配，可能出现不存在memblk的情况！

void * *FVM_GlobalBlk;
void * *FVM_CurrBlk;

void FVM_Init()
{
  FVM_GlobalBlk = FVM_Create();
  FVM_SelectBlk(FVM_GlobalBlk);
}

//建立新的memblk
void * *FVM_Create()
{
        unsigned char i;
        void * * memblk;
        memblk = (void * *)malloc(sizeof(void *)*FVM_BlkSize);
        for (i=0;i<FVM_BlkSize ;i++)
        {
                memblk[i]=NULL;
        }
        return memblk;
}

void FVM_SelectBlk(void * * memblk)
{
        FVM_CurrBlk = memblk;
}
//寻找memblk结构的下一个空位
void * *FVM_FindSlot(void *memblk[])
{
        unsigned char i;
        void *nextblk;
        for (i=0;i<(FVM_BlkSize-1);i++)
                if (memblk[i]==NULL)
                        return &(memblk[i]);
        if (memblk[(FVM_BlkSize-1)]==NULL)
        {
                nextblk = (void *)malloc(sizeof(void *)*FVM_BlkSize);
                memblk[(FVM_BlkSize-1)] = nextblk;
                return FVM_FindSlot((void * *)nextblk);
        }else{
                return FVM_FindSlot((void * *)memblk[(FVM_BlkSize-1)]);
        }
}

void *FVM_Malloc(unsigned int size)
{
        void *mem;
        void * *slot;

        mem = (void *)malloc(size);
        slot = FVM_FindSlot(FVM_CurrBlk);
        *slot = mem;
        return mem;
}

void FVM_FreeBlk(void * *memblk)
{
        unsigned char i;
        for (i=0;i<(FVM_BlkSize-1);i++)
        {
                if (memblk[i]!=NULL)
                {
                        free(memblk[i]);
                        memblk[i]=NULL;
                }
        }
        if (memblk[(FVM_BlkSize-1)]==NULL)
                free((void *)memblk);
        else
                FVM_FreeBlk((void * *)memblk[(FVM_BlkSize-1)]);
}

void FVM_FreeCurr()
{
        FVM_FreeBlk(FVM_CurrBlk);
        FVM_CurrBlk = NULL;
}
