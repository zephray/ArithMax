//-----------------------------------------------------------------------------
//
//	Scan expr for vars, return in vector
//
//	Input:		Expression on stack
//
//	Output:		Vector
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"
static void vscan(U *);
static int __cmp(const void *, const void *);
static int h;

void
variables(void)
{
	int i, n;
	save();
	p1 = pop();
	h = tos;
	vscan(p1);
	n = tos - h;
	if (n > 1)
		qsort(stack + h, n, sizeof (U *), __cmp);
	p1 = alloc_tensor(n);
	p1->u.tensor->ndim = 1;
	p1->u.tensor->dim[0] = n;
	for (i = 0; i < n; i++)
		p1->u.tensor->elem[i] = stack[i];
	tos = h;
	push(p1);
	restore();
}

static void
vscan(U *p)
{
	int i;
	if (iscons(p)) {
		p = cdr(p);
		while (iscons(p)) {
			vscan(car(p));
			p = cdr(p);
		}
	} else if (issymbol(p) && p != symbol(E)) {
		for (i = h; i < tos; i++)
			if (stack[i] == p)
				return;
		push(p);
	}
}

static int
__cmp(const void *p1, const void *p2)
{
	return cmp_expr(*((U **) p1), *((U **) p2));
}
