/* Eigenmath by G. Weigt

	The starting point for a symbolic computation is in run.cpp

	Input is scanned in scan.cpp

	Expression evaluation is done in eval.cpp

	Output is formatted in display.cpp
*/

#include "defs.h"
#define BUFLEN 100000
static char buf[BUFLEN];

int
main(int argc, char *argv[])
{
	int i;
	FILE *f;

	for (i = 1; i < argc; i++) {
		f = fopen(argv[i], "r");
		if (f == NULL) {
			printf("cannot open %s\n", argv[i]);
			exit(1);
		}
		fread(buf, BUFLEN, 1, f);
		fclose(f);
		run(buf);
	}

	for (;;) {
		printf("> ");
		fgets(buf, BUFLEN, stdin);
		run(buf);
	}
}

void
clear_term()
{
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

#define OUTBUFLEN 10000
char out_buf[OUTBUFLEN + 1];
int out_count;

void
printchar(int c)
{
	if (test_flag && out_count < OUTBUFLEN)
		out_buf[out_count++] = c;
	fputc(c, stdout);
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
