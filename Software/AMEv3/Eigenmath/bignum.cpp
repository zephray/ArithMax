#include "stdafx.h"
#include "defs.h"

#define MP_MIN_SIZE 2
#define MP_MAX_FREE 500

double convert_rational_to_double(U *);
double convert_bignum_to_double(unsigned int *);
int ge(unsigned int *, unsigned int *, int);

int mtotal, mfreecount;
static unsigned int *free_stack[MP_MAX_FREE];

unsigned int *
mnew(int n)
{
	unsigned int *p;
	if (n < MP_MIN_SIZE)
		n = MP_MIN_SIZE;
	if (n == MP_MIN_SIZE && mfreecount)
		p = free_stack[--mfreecount];
	else {
		p = (unsigned int *) malloc((n + 3) * sizeof (int));
		if (p == 0)
			stop("malloc failure");
	}
	p[0] = n;
	mtotal += n;
	return p + 3;
}

void
mfree(unsigned int *p)
{
	p -= 3;
	mtotal -= p[0];
	if (p[0] == MP_MIN_SIZE && mfreecount < MP_MAX_FREE)
		free_stack[mfreecount++] = p;
	else
		free(p);
}

// convert int to bignum

unsigned int *
mint(int n)
{
	unsigned int *p = mnew(1);
	if (n < 0)
		MSIGN(p) = -1;
	else
		MSIGN(p) = 1;
	MLENGTH(p) = 1;
	p[0] = abs(n);
	return p;
}

// copy bignum

unsigned int *
mcopy(unsigned int *a)
{
	int i;
	unsigned int *b;

	b = mnew(MLENGTH(a));

	MSIGN(b) = MSIGN(a);
	MLENGTH(b) = MLENGTH(a);

	for (i = 0; i < MLENGTH(a); i++)
		b[i] = a[i];

	return b;
}

// a >= b ?

int
ge(unsigned int *a, unsigned int *b, int len)
{
	int i;
	for (i = len - 1; i > 0; i--)
		if (a[i] == b[i])
			continue;
		else
			break;
	if (a[i] >= b[i])
		return 1;
	else
		return 0;
}

void
add_numbers(void)
{
	double a, b;

	if (isrational(stack[tos - 1]) && isrational(stack[tos - 2])) {
		qadd();
		return;
	}

	save();

	p2 = pop();
	p1 = pop();

	if (isdouble(p1))
		a = p1->u.d;
	else
		a = convert_rational_to_double(p1);

	if (isdouble(p2))
		b = p2->u.d;
	else
		b = convert_rational_to_double(p2);

	push_double(a + b);

	restore();
}

void
subtract_numbers(void)
{
	double a, b;

	if (isrational(stack[tos - 1]) && isrational(stack[tos - 2])) {
		qsub();
		return;
	}

	save();

	p2 = pop();
	p1 = pop();

	if (isdouble(p1))
		a = p1->u.d;
	else
		a = convert_rational_to_double(p1);

	if (isdouble(p2))
		b = p2->u.d;
	else
		b = convert_rational_to_double(p2);

	push_double(a - b);

	restore();
}

void
multiply_numbers(void)
{
	double a, b;

	if (isrational(stack[tos - 1]) && isrational(stack[tos - 2])) {
		qmul();
		return;
	}

	save();

	p2 = pop();
	p1 = pop();

	if (isdouble(p1))
		a = p1->u.d;
	else
		a = convert_rational_to_double(p1);

	if (isdouble(p2))
		b = p2->u.d;
	else
		b = convert_rational_to_double(p2);

	push_double(a * b);

	restore();
}

void
divide_numbers(void)
{
	double a, b;

	if (isrational(stack[tos - 1]) && isrational(stack[tos - 2])) {
		qdiv();
		return;
	}

	save();

	p2 = pop();
	p1 = pop();

	if (iszero(p2))
		stop("divide by zero");

	if (isdouble(p1))
		a = p1->u.d;
	else
		a = convert_rational_to_double(p1);

	if (isdouble(p2))
		b = p2->u.d;
	else
		b = convert_rational_to_double(p2);

	push_double(a / b);

	restore();
}

void
invert_number(void)
{
	unsigned int *a, *b;

	save();

	p1 = pop();

	if (iszero(p1))
		stop("divide by zero");

	if (isdouble(p1)) {
		push_double(1 / p1->u.d);
		restore();
		return;
	}

	a = mcopy(p1->u.q.a);
	b = mcopy(p1->u.q.b);

	MSIGN(b) = MSIGN(a);
	MSIGN(a) = 1;

	p1 = alloc();

	p1->k = NUM;

	p1->u.q.a = b;
	p1->u.q.b = a;

	push(p1);

	restore();
}

int
compare_rationals(U *a, U *b)
{
	int t;
	unsigned int *ab, *ba;
	ab = mmul(a->u.q.a, b->u.q.b);
	ba = mmul(a->u.q.b, b->u.q.a);
	t = mcmp(ab, ba);
	mfree(ab);
	mfree(ba);
	return t;
}

int
compare_numbers(U *a, U *b)
{
	double x, y;
	if (isrational(a) && isrational(b))
		return compare_rationals(a, b);
	if (isdouble(a))
		x = a->u.d;
	else
		x = convert_rational_to_double(a);
	if (isdouble(b))
		y = b->u.d;
	else
		y = convert_rational_to_double(b);
	if (x < y)
		return -1;
	if (x > y)
		return 1;
	return 0;
}

void
negate_number(void)
{
	save();
	p1 = pop();
	if (iszero(p1)) {
		push(p1);
		restore();
		return;
	}
	switch (p1->k) {
	case NUM:
		p2 = alloc();
		p2->k = NUM;
		p2->u.q.a = mcopy(p1->u.q.a);
		p2->u.q.b = mcopy(p1->u.q.b);
		MSIGN(p2->u.q.a) *= -1;
		push(p2);
		break;
	case DOUBLE:
		push_double(-p1->u.d);
		break;
	default:
		stop("bug caught in mp_negate_number");
		break;
	}
	restore();
}

void
bignum_truncate(void)
{
	unsigned int *a;

	save();

	p1 = pop();

	a = mdiv(p1->u.q.a, p1->u.q.b);

	p1 = alloc();

	p1->k = NUM;

	p1->u.q.a = a;
	p1->u.q.b = mint(1);

	push(p1);

	restore();
}

void
mp_numerator(void)
{
	save();

	p1 = pop();

	if (p1->k != NUM) {
		push(one);
		restore();
		return;
	}

	p2 = alloc();

	p2->k = NUM;

	p2->u.q.a = mcopy(p1->u.q.a);
	p2->u.q.b = mint(1);

	push(p2);

	restore();
}

void
mp_denominator(void)
{
	save();

	p1 = pop();

	if (p1->k != NUM) {
		push(one);
		restore();
		return;
	}

	p2 = alloc();

	p2->k = NUM;

	p2->u.q.a = mcopy(p1->u.q.b);
	p2->u.q.b = mint(1);

	push(p2);

	restore();
}

void
bignum_power_number(int expo)
{
	unsigned int *a, *b, *t;

	save();

	p1 = pop();

	a = mpow(p1->u.q.a, abs(expo));
	b = mpow(p1->u.q.b, abs(expo));

	if (expo < 0) {
		t = a;
		a = b;
		b = t;
		MSIGN(a) = MSIGN(b);
		MSIGN(b) = 1;
	}

	p1 = alloc();

	p1->k = NUM;

	p1->u.q.a = a;
	p1->u.q.b = b;

	push(p1);

	restore();
}

double
convert_bignum_to_double(unsigned int *p)
{
	int i;
	double d = 0.0;
	for (i = MLENGTH(p) - 1; i >= 0; i--)
		d = 4294967296.0 * d + p[i];
	if (MSIGN(p) == -1)
		d = -d;
	return d;
}

double
convert_rational_to_double(U *p)
{
	int i, n, na, nb;
	double a = 0.0, b = 0.0;

	na = MLENGTH(p->u.q.a);
	nb = MLENGTH(p->u.q.b);

	if (na < nb)
		n = na;
	else
		n = nb;

	for (i = 0; i < n; i++) {
		a = a / 4294967296.0 + p->u.q.a[i];
		b = b / 4294967296.0 + p->u.q.b[i];
	}

	if (na > nb)
		for (i = nb; i < na; i++) {
			a = a / 4294967296.0 + p->u.q.a[i];
			b = b / 4294967296.0;
		}

	if (na < nb)
		for (i = na; i < nb; i++) {
			a = a / 4294967296.0;
			b = b / 4294967296.0 + p->u.q.b[i];
		}

	if (MSIGN(p->u.q.a) == -1)
		a = -a;

	return a / b;
}

void
push_integer(int n)
{
	save();
	p1 = alloc();
	p1->k = NUM;
	p1->u.q.a = mint(n);
	p1->u.q.b = mint(1);
	push(p1);
	restore();
}

void
push_double(double d)
{
	save();
	p1 = alloc();
	p1->k = DOUBLE;
	p1->u.d = d;
	push(p1);
	restore();
}

void
push_rational(int a, int b)
{
	save();
	p1 = alloc();
	p1->k = NUM;
	p1->u.q.a = mint(a);
	p1->u.q.b = mint(b);
	/* FIXME -- normalize */
	push(p1);
	restore();
}

int
pop_integer(void)
{
	int n;

	save();

	p1 = pop();

	switch (p1->k) {

	case NUM:
		if (isinteger(p1) && MLENGTH(p1->u.q.a) == 1) {
			n = p1->u.q.a[0];
			if (n & 0x80000000)
				n = 0x80000000;
			else
				n *= MSIGN(p1->u.q.a);
		} else
			n = 0x80000000;
		break;

	case DOUBLE:
		n = (int) p1->u.d;
		if ((double) n != p1->u.d)
			n = 0x80000000;
		break;

	default:
		n = 0x80000000;
		break;
	}

	restore();
	return n;
}

void
print_double(U *p, int flag)
{
	static char buf[80];
	sprintf(buf, "%g", p->u.d);
	if (flag == 1 && *buf == '-')
		print_str(buf + 1);
	else
		print_str(buf);
}

void
bignum_scan_integer(char *s)
{
	unsigned int *a;
	char sign;

	save();

	sign = *s;

	if (sign == '+' || sign == '-')
		s++;

	a = mscan(s);

	p1 = alloc();

	p1->k = NUM;

	p1->u.q.a = a;
	p1->u.q.b = mint(1);

	push(p1);

	if (sign == '-')
		negate();

	restore();
}

void
bignum_scan_float(char *s)
{
	push_double(atof(s));
}

// print as unsigned

void
print_number(U *p)
{
	char *s;
	static char buf[100];
	switch (p->k) {
	case NUM:
		s = mstr(p->u.q.a);
		if (*s == '+' || *s == '-')
			s++;
		print_str(s);
		if (isfraction(p)) {
			print_str("/");
			s = mstr(p->u.q.b);
			print_str(s);
		}
		break;
	case DOUBLE:
		sprintf(buf, "%g", p->u.d);
		if (*buf == '+' || *buf == '-')
			print_str(buf + 1);
		else
			print_str(buf);
		break;
	default:
		break;
	}
}

void
gcd_numbers(void)
{
	save();

	p2 = pop();
	p1 = pop();

//	if (!isinteger(p1) || !isinteger(p2))
//		stop("integer args expected for gcd");

	p3 = alloc();

	p3->k = NUM;

	p3->u.q.a = mgcd(p1->u.q.a, p2->u.q.a);
	p3->u.q.b = mgcd(p1->u.q.b, p2->u.q.b);

	MSIGN(p3->u.q.a) = 1;

	push(p3);

	restore();
}

double
pop_double(void)
{
	double d;
	save();
	p1 = pop();
	switch (p1->k) {
	case NUM:
		d = convert_rational_to_double(p1);
		break;
	case DOUBLE:
		d = p1->u.d;
		break;
	default:
		d = 0.0;
		break;
	}
	restore();
	return d;
}

void
bignum_float(void)
{
	double d;
	d = convert_rational_to_double(pop());
	push_double(d);
}

static unsigned int *__factorial(int);

void
bignum_factorial(int n)
{
	save();
	p1 = alloc();
	p1->k = NUM;
	p1->u.q.a = __factorial(n);
	p1->u.q.b = mint(1);
	push(p1);
	restore();
}

static unsigned int *
__factorial(int n)
{
	int i;
	unsigned int *a, *b, *t;

	if (n == 0 || n == 1) {
		a = mint(1);
		return a;
	}

	a = mint(2);

	b = mint(0);

	for (i = 3; i <= n; i++) {
		b[0] = (unsigned int) i;
		t = mmul(a, b);
		mfree(a);
		a = t;
	}

	mfree(b);

	return a;
}

static unsigned int mask[32] = {
	0x00000001,
	0x00000002,
	0x00000004,
	0x00000008,
	0x00000010,
	0x00000020,
	0x00000040,
	0x00000080,
	0x00000100,
	0x00000200,
	0x00000400,
	0x00000800,
	0x00001000,
	0x00002000,
	0x00004000,
	0x00008000,
	0x00010000,
	0x00020000,
	0x00040000,
	0x00080000,
	0x00100000,
	0x00200000,
	0x00400000,
	0x00800000,
	0x01000000,
	0x02000000,
	0x04000000,
	0x08000000,
	0x10000000,
	0x20000000,
	0x40000000,
	0x80000000,
};

void
mp_set_bit(unsigned int *x, unsigned int k)
{
	x[k / 32] |= mask[k % 32];
}

void
mp_clr_bit(unsigned int *x, unsigned int k)
{
	x[k / 32] &= ~mask[k % 32];
}

void
mshiftright(unsigned int *a)
{
	int c, i, n;
	n = MLENGTH(a);
	c = 0;
	for (i = n - 1; i >= 0; i--)
		if (a[i] & 1) {
			a[i] = (a[i] >> 1) | c;
			c = 0x80000000;
		} else {
			a[i] = (a[i] >> 1) | c;
			c = 0;
		}
	if (n > 1 && a[n - 1] == 0)
		MLENGTH(a) = n - 1;
}
