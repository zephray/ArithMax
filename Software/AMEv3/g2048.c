/*2048*/
#include "g2048.h"
#include "stn.h"
#include "key.h"

int code[4][4]={0,0,0,0,
                0,0,0,0,
                0,0,0,0,
                0,0,0,0};/*游戏中的16个格子*/
int temp[5];/*中间变量*/
int move=0;/*移动次数*/
int score=0;/*分数*/

void print(void)/*显示游戏界面*/
{
    int i,j;

    LCD_Clear(0);
    for(i=0;i<=3;i++)
    {
        for(j=0;j<=3;j++)
        {
            if(code[i][j]!=0)
            {
                LCD_Fill(24*j,8*i,24*j+22,8*i+7,1);
                LCD_DispNum_6X8(24*j,8*i,code[i][j],4,0);/*显示数字和分隔线*/
            }
        }
    }
    LCD_Update();
}

int add(void)/*对中间变量数组进行处理*/
{
    int i;
    int t=0;
    int change=0;/*判断数组是否有改变，0不变，1变化*/

    do
    {
        for(i=0;i<=3;i++)
        {
            if(temp[i]==0)
            {
                if(temp[i]!=temp[i+1])
                    change=1;/*当一个0后面不是0时数组改变*/
                temp[i]=temp[i+1];
                temp[i+1]=0;
            }
        }/*去掉中间的0*/
        t++;
    }while(t<=3);/*重复多次*/
    for(i=1;i<=3;i++)
    {
        if(temp[i]==temp[i-1])
        {

            if(temp[i]!=0)
            {
                change=1;/*当两个非零相同的数相加时数组改变*/
                score=score+temp[i];/*加分*/
            }
            temp[i-1]=temp[i-1]*2;
            temp[i]=0;
        }
    }/*把两个相邻的相同的数加起来*/
    do
    {
        for(i=0;i<=3;i++)
        {
            if(temp[i]==0)
            {
                temp[i]=temp[i+1];
                temp[i+1]=0;
            }
        }/*去掉中间的0*/
        t++;
    }while(t<=3);/*重复多次*/
    return change;
}

int G2048_main(void)
{
    int gameover=0;/*判断游戏是否结束，1结束，0继续*/
    int i,j,k;
    int change=1;/*判断格子中的数是否改变，0不变*/
    char input;

    srand(Timing);/*设置随机数的起点*/
    for (i=0;i<4;i++)
      for (j=0;j<4;j++)
        code[i][j]=0;
    score=0;
    move=0;
    while(gameover==0)
    {
        if(change>=1)/*仅当数发生改变时添加新数*/
        {
            for (k=0;k<10;k++)
            {
                i=((unsigned)rand())%4;
                j=((unsigned)rand())%4;
                if (code[i][j]==0) break;
            }
            if (code[i][j]!=0)
            {
              i=0;
              j=0;
              while(code[i][j]!=0)
              {
                if (j<3) j++;
                else
                {
                  i++;
                  j=0;
                }
              }
            }
            if(((unsigned)rand())%4==0)
            {
                code[i][j]=4;
            }
            else
            {
                code[i][j]=2;/*随机选一个空格填上2或4*/
            }
            move++;/*增加次数*/
        }
        print();/*显示*/
        input=GetKey();/*输入方向*/

        change=0;
        switch(input)
        {
            case KEY_CTRL_AC:/*退出*/
                LCD_Clear(0);
                LCD_String_6X8(0,0,"Exit?",1);
                LCD_String_6X8(0,16,"[AC] = Cancel",1);
                LCD_String_6X8(0,24,"[=]  = OK",1);
                LCD_Update();
                input=GetKey();
                if(input==KEY_CTRL_EXE)
                    return 0;
                break;

            case KEY_CTRL_UP:
            case KEY_CHAR_8:/*上*/
                for(j=0;j<=3;j++)
                {
                    for(i=0;i<=3;i++)
                    {
                        temp[i]=code[i][j];/*把一列数移到中间变量*/
                    }
                    temp[4]=0;
                    change=change+add();
                    for(i=0;i<=3;i++)
                    {
                        code[i][j]=temp[i];/*把处理好的中间变量移回来*/
                    }
                }
                break;

            case KEY_CTRL_LEFT:
            case KEY_CHAR_4:/*左*/
                for(i=0;i<=3;i++)
                {
                    for(j=0;j<=3;j++)
                    {
                        temp[j]=code[i][j];/*把一行数移到中间变量*/
                    }
                    temp[4]=0;
                    change=change+add();
                    for(j=0;j<=3;j++)
                    {
                        code[i][j]=temp[j];/*把处理好的中间变量移回来*/
                    }
                }

                break;

            case KEY_CTRL_DOWN:
            case KEY_CHAR_2:/*下*/
                for(j=0;j<=3;j++)
                {
                    for(i=0;i<=3;i++)
                    {
                        temp[i]=code[3-i][j];/*把一列数移到中间变量*/
                    }
                    temp[4]=0;
                    change=change+add();
                    for(i=0;i<=3;i++)
                    {
                        code[3-i][j]=temp[i];/*把处理好的中间变量移回来*/
                    }
                }
                break;

            case KEY_CTRL_RIGHT:
            case KEY_CHAR_6:/*右*/
                for(i=0;i<=3;i++)
                {
                    for(j=0;j<=3;j++)
                    {
                        temp[j]=code[i][3-j];/*把一行数移到中间变量*/
                    }
                    temp[4]=0;
                    change=change+add();
                    for(j=0;j<=3;j++)
                    {
                        code[i][3-j]=temp[j];/*把处理好的中间变量移回来*/
                    }
                }
                break;
        }
        gameover=1;
        for(i=0;i<=3;i++)
            for(j=0;j<=3;j++)
                if(code[i][j]==0)
                    gameover=0;/*所有格子都填满则游戏结束*/

    }
    LCD_Clear(0);
    LCD_String_6X8(0,0,"Game over!",1);
    LCD_String_6X8(0,8,"Score:",1);
    LCD_String_6X8(0,16,"Move:",1);
    LCD_DispNum_6X8(36,8,score,10,1);
    LCD_DispNum_6X8(30,16,move,11,1);
    LCD_String_6X8(0,24,"Press any key",1);
    LCD_Update();
    GetKey();
    return 0;
}
