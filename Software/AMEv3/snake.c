#define GN 200
#include "main.h"
#include "lcd.h"
#include "key.h"
#define LEFT 62
#define RIGHT 73
#define DOWN 63
#define UP 72
#define ESC 24

int i;
uint8_t key;
int score=0;/*得分*/
int gamespeed=50;/*游戏速度自己调整*/

struct Food
{
   int x;/*食物的横坐标*/
   int y;/*食物的纵坐标*/
   int yes;/*判断是否要出现食物的变量*/
}food;/*食物的结构体*/

struct Snake
{
   int x[GN];
   int y[GN];
   int node;/*蛇的节数*/
   int direction;/*蛇移动方向*/
   int life;/* 蛇的生命,0活着,1死亡*/
}snake;

void Init(void);/*图形驱动*/
void Close(void);/*图形结束*/
void DrawK(void);/*开始画面*/
void GameOver(void);/*结束游戏*/
void GamePlay(void);/*玩游戏具体过程*/
void PrScore(void);/*输出成绩*/
/*主函数*/
void snake_main(void)
{
   Init();/*图形驱动*/
   DrawK();/*开始画面*/
   GamePlay();/*玩游戏具体过程*/
   Close();/*图形结束*/
}
/*图形驱动*/
void Init(void)
{
   LCD_Fill(0,0,319,239,0x0000);
}
/*开始画面，左上角坐标为（50，40），右下角坐标为（610，460）的围墙*/
void DrawK(void)
{
/*setbkcolor(LIGHTGREEN);*/
  LCD_Fill(13,24,305,29,LCD_COLOR_WHITE); /*上边*/
  LCD_Fill(13,222,305,227,LCD_COLOR_WHITE);/*下边*/
  LCD_Fill(13,24,18,227,LCD_COLOR_WHITE); /*左边*/
  LCD_Fill(300,24,305,227,LCD_COLOR_WHITE);/*右边*/
}
/*玩游戏具体过程*/
void GamePlay(void)
{
   srand();/*随机数发生器*/
   food.yes=0;/*1表示需要出现新食物,0表示已经存在食物*/
   snake.life=0;/*活着*/
   snake.direction=1;/*方向往右*/
   snake.x[0]=50;snake.y[0]=50;/*蛇头*/
   snake.x[1]=60;snake.y[1]=50;
   snake.node=2;/*节数*/
   food.x=150;
   food.y=150;
   PrScore();/*输出得分*/
   key=255;
     while(1)/*在没有按键的情况下,蛇自己移动身体*/
     {
        if(food.yes==1)/*需要出现新食物*/
        {
          food.x=rand()%200+40;
          food.y=rand()%175+30;
          while(food.x%10!=0)/*食物随机出现后必须让食物能够在整格内,这样才可以让蛇吃到*/
            food.x++;
          while(food.y%10!=0)
            food.y++;
          food.yes=0;/*画面上有食物了*/
        }
        if(food.yes==0)/*画面上有食物了就要显示*/
        {
              //setcolor(GREEN);
              LCD_Fill(food.x,food.y-9,food.x+9,food.y,LCD_COLOR_GREEN);
        }
        for(i=snake.node-1;i>0;i--)/*蛇的每个环节往前移动,也就是贪吃蛇的关键算法*/
        {
          snake.x[i]=snake.x[i-1];
          snake.y[i]=snake.y[i-1];
        }
       /*1,2,3,4表示右,左,上,下四个方向,通过这个判断来移动蛇头*/
        switch(snake.direction)
        {
          case 1:snake.x[0]+=10;break;
          case 2:snake.x[0]-=10;break;
          case 3:snake.y[0]-=10;break;
          case 4:snake.y[0]+=10;break;
        }
        for(i=3;i<snake.node;i++)/*从蛇的第四节开始判断是否撞到自己了，因为蛇头为两节，第三节不可能拐过来*/
        {
          if(snake.x[i]==snake.x[0]&&snake.y[i]==snake.y[0])
          {
               GameOver();/*显示失败*/
               snake.life=1;
               break;
          }
        }
        if(snake.x[0]<20||snake.x[0]>290||snake.y[0]<40||
            snake.y[0]>227)/*蛇是否撞到墙壁*/
        {
          GameOver();/*本次游戏结束*/
          snake.life=1; /*蛇死*/
          break;
        }
        if(snake.life==1)/*以上两种判断以后,如果蛇死就跳出内循环，重新开始*/
           break;
        if(snake.x[0]==food.x&&snake.y[0]==food.y)/*吃到食物以后*/
        {
           //setcolor(0);/*把画面上的食物东西去掉*/
          LCD_Fill(food.x,food.y-9,food.x+9,food.y,0);
          snake.x[snake.node]=-5;snake.y[snake.node]=-5;
          /*新的一节先放在看不见的位置,下次循环就取前一节的位置*/
          snake.node++;/*蛇的身体长一节*/
          food.yes=1;/*画面上需要出现新的食物*/
          score+=10;
          PrScore();/*输出新得分*/
        }
        //setcolor(4);/*画出蛇*/
        for(i=0;i<snake.node;i++)
          LCD_Fill(snake.x[i],snake.y[i]-9,snake.x[i]+9,
               snake.y[i],LCD_COLOR_RED);
        Delay(gamespeed);
        //setcolor(0);/*用黑色去除蛇的的最后一节*/
        LCD_Fill(snake.x[snake.node-1],snake.y[snake.node-1]-9,
          snake.x[snake.node-1]+9,snake.y[snake.node-1],0);
        key=LastKey;/*接收按键*/
        if (key!=255)
          LCD_DispNum(0,0,key,3,0xffff);
        if(key==ESC)/*按ESC键退出*/
          break;
        else
        if((key==UP)&&(snake.direction!=4))
        /*判断是否往相反的方向移动*/
          snake.direction=3;
        else
        if((key==RIGHT)&&(snake.direction!=2))
          snake.direction=1;
        else
        if((key==LEFT)&&(snake.direction!=1))
          snake.direction=2;
        else
        if((key==DOWN)&&(snake.direction!=3))
          snake.direction=4;
        LastKey=255;
        key=255;
    }  /*endwhile（！kbhit）*/
}
/*游戏结束*/
void GameOver(void)
{
    PrScore();
   //setcolor(RED);
   //settextstyle(0,0,4);
   LCD_String(100,100,"游戏结束,按任意键继续",LCD_COLOR_RED);
   WaitForKey();
}
/*输出成绩*/
void PrScore(void)
{   
   char str[10];
   LCD_Fill(25,3,110,21,LCD_COLOR_YELLOW);
   sprintf(str,"score:%d",score);
   LCD_String(27,4,str,LCD_COLOR_BLACK);
   
}
/*图形结束*/
void Close(void)
{   
}