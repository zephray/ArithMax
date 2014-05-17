#include "sysbase.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "defs.h"
extern "C" {
#include "dConsole.h"
}
extern int esc_flag;
int execution_in_progress = 0;
static char expr[INPUTBUFLEN];

void run_script(char* filename);
void run_startup_script();
void check_execution_abort();
void input_eval_loop();
void updateRandomUserObject();
#define DIRNAME (unsigned char*)"@EIGEN"
#define SCRIPTFILE (unsigned char*)"Script"

extern "C" void eigenmath_main()
{
  LCD_Clear(0xffff); 
  dPrintf("ArithMax EV3\nOS 1.0.DEV\n");
  input_eval_loop();
}

void input_eval_loop() {
  char** recHistory = NULL; int curRecHistEntry = 0;
  while (1) {
    dPrintf(">");
    dConsoleRedraw();
    int res = dGetLine(expr,INPUTBUFLEN);
    dPuts(expr);
    update_cmd_history(expr);
    dConsoleRedraw();
    if(strcmp(expr, "testmode") == 0) {
      //TestMode(1);
    } else if(strcmp(expr, "meminfo") == 0) {
      print_mem_info();
    } else if(strcmp(expr, "memgc") == 0) {
      gc();
    } else if(strcmp(expr, "2048()") == 0) {
      G2048_main();
    } else {
      execution_in_progress = 1;
      PM_SetCPUFreq(168);
      run(expr);
      PM_SetCPUFreq(16);
      execution_in_progress = 0;     
    }
  }
}

void run_script(char* filename) {
  /*unsigned short pFile[MAX_FILENAME_SIZE+1];
  Bfile_StrToName_ncpy(pFile, (unsigned char*)filename, strlen(filename)+1); 
  int hFile = Bfile_OpenFile_OS(pFile, READWRITE, 0); // Get handle
  
  // Check for file existence
  if(hFile >= 0) // Check if it opened
  {
    // Returned no error, file exists, open it
    int size = Bfile_GetFileSize_OS(hFile);
    // File exists and has size 'size'
    // Read file into a buffer
    if ((unsigned int)size > MAX_TEXTVIEWER_FILESIZE) {
      Bfile_CloseFile_OS(hFile);
      printf("Stop: script too big\n");
      return; //file too big, return
    }
    unsigned char* asrc = (unsigned char*)alloca(size*sizeof(unsigned char)+5); // 5 more bytes to make sure it fits...
    memset(asrc, size+5, 0); //alloca does not clear the allocated space. Make sure the string is null-terminated this way.
    int rsize = Bfile_ReadFile_OS(hFile, asrc, size, 0);
    Bfile_CloseFile_OS(hFile); //we got file contents, close it
    asrc[rsize]='\0';
    execution_in_progress = 1;
    run((char*)asrc);
    execution_in_progress = 0;
  }*/
}

void check_execution_abort() {
  if(execution_in_progress) {
   // HourGlass();
  }
}

void
clear_term()
{
  // user issued "clear" command.
  dConsoleCls();
}

extern void eval_print(void);

void
eval_display(void)
{
        eval_print();
}

void
printstr(char *s)
{
        while (*s)
                printchar(*s++);
}

extern int test_flag;

#define OUTBUFLEN 1000
char out_buf[OUTBUFLEN + 1];
int out_count;

void
printchar(int c)
{
  if (test_flag && out_count < OUTBUFLEN)
		out_buf[out_count++] = c;
  dConsolePutChar(c);
}

void
printchar_nowrap(int c)
{
        printchar(c);
}

void
eval_draw(void)
{
        push(symbol(NIL));
}

void
eval_sample(void)
{
}

// Command history related:
char *
get_curr_cmd(void)
{
        int i, len;
        char *s;

        len=strlen(expr);
        s = (char*)malloc(len+1);
        strcpy(s, expr);

        // trim trailing spaces

        for (i = len - 1; i >= 0; i--) {
                if (isspace(s[i]))
                        s[i] = 0;
                else
                        break;
        }

        return s;
}
void
update_curr_cmd(char *s)
{
        strcpy(expr, s);
}

// Entropy related:

static unsigned int lastrandom=0x12345678;

unsigned int random( int seed = 0 ){
    if (seed) lastrandom=seed;
    lastrandom = ( 0x41C64E6D*lastrandom ) + 0x3039;
    return ( lastrandom >> 16 );
}

void updateRandomUserObject() {
/*  int rd = random( RTC_GetTicks() );
  char b[50] = "";
  char c[20] = "";
  strcpy(b, "prizmRandomSeed=");
  itoa(rd, (unsigned char*)c);
  strcat(b, c);
  run(b);*/
}