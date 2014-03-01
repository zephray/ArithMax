//-----------------------------------------------------------------------------
//
//	Author : philippe.billet@noos.fr
//
//	sgn sign function
//
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"

void
eval_sgn(void)
{
	push(cadr(p1));
	eval();
	sgn();
}

void
sgn(void)
{
	save();
	yysgn();
	restore();
}

#define X p1

void
yysgn(void)
{
	
	X = pop();

	
	if (isdouble(p1)) {
		if (p1->u.d > 0) 
			{push_integer(1);
			 return;}
		else 
			if (p1->u.d == 0) 
				{push_integer(1);
				 return;}
			else
				{push_integer(-1);
				 return;}
	}

	if (isrational(p1)) {
		if (MSIGN(mmul(p1->u.q.a,p1->u.q.b)) == -1) 
			{push_integer(-1);
			 return;}
		else 
			if (MZERO(mmul(p1->u.q.a,p1->u.q.b))) 
				{push_integer(0);
				 return;}
			else
				{push_integer(1);
				 return;}
	}

	if (iscomplexnumber(X)){
		push_integer(-1);
		push(X);
		absval();
		power();
		push(X);
		multiply();
		return;
	}
	
	
	if (isnegativeterm(X)) {
		push_symbol(SGN);
		push(X);
		negate();
		list(2);
		push_integer(-1);
		multiply();
		return;
	}
	
/*	push_integer(2);
	push(X);
	heaviside();
	multiply();
	push_integer(-1);
	add();	*/
	
	push_symbol(SGN);
	push(X);
	list(2);
}

#if SELFTEST

static char *s[] = {


	"sgn(-3)",
	"-1",
	

	"sgn(0)",
	"0",
	
	"sgn(3)",
	"1",

};

void
test_sgn(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
