#ifndef DCONSOLE_H
#define DCONSOLE_H
#define INPUTBUFLEN 1000
//typedef char *va_list ;

int dGetLineBox (char * s,int max,int width,int x,int y);

void dConsoleRedraw ();

void dConsolePut(const char * str);
void dConsolePutChar (char c);

int dGetLine (char * s,int max);

int dPrintf (const char * format,...);

void dConsoleCls ();
void dPuts(const char *);

/*
#define printf          dPrintf
#define puts            dPuts
#define putchar         dConsolePutChar
#define gets            dGetLine
*/

#endif