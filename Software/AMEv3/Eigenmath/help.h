
static char *help_factor_polynomial[] = {
	"f=(x^2+3*x+2)*(y^2+7*y+12)",
	"factor(f)",
	"factor(f,x,y)",
};

// linear algebra

static char *help_adj[] = {
	"A=((a,b),(c,d))",
	"adj(A)",
	"inv(A)-adj(A)/det(A)",
};

static char *help_contract[] = {
	"A=((a,b),(c,d))",
	"A",
	"contract(A,1,2)",
};

static char *help_det[] = {
	"A=((a,b),(c,d))",
	"A",
	"det(A)",
};

static char *help_dot[] = {
	"A=((a11,a12),(a21,a22))",
	"X=(x1,x2)",
	"dot(A,X)",
};

static char *help_inv[] = {
	"A=((1,2),(3,4))",
	"inv(A)",
	"dot(A,inv(A))",
};

static char *help_outer[] = {
	"A=(A1,A2)",
	"B=(B1,B2)",
	"outer(A,B)",
	"dot(A,B)-contract(outer(A,B))",
};

static char *help_transpose[] = {
	"A=((1,2),(3,4))",
	"transpose(A)",
};

static char *help_unit[] = {
	"I=unit(2)",
	"I",
};

static char *help_zero[] = {
	"A=zero(2,2)",
	"A",
	"A[1,1]=A11",
	"A[1,2]=A12",
	"A",
};

// calculus

static char *help_derivative[] = {
	"y=sin(x)",
	"d(y)",
	"d(y,x,x)",
};

static char *help_gradient[] = {
	"f=cos(x)+sin(y)",
	"d(f,(x,y))",
};

static char *help_integral[] = {
	"y=x^2",
	"integral(y)",
	"integral(y,x,x)",
};

// complex number functions

static char *help_arg[] = {
	"z=1+i",
	"arg(z)",
};

static char *help_conj[] = {
	"z=3+4*i",
	"conj(z)",
};

static char *help_imag[] = {
	"z=3+4*i",
	"imag(z)",
};

static char *help_mag[] = {
	"z=3+4*i",
	"mag(z)",
};

static char *help_nroots[] ={
	"y=x^4+1",
	"nroots(y)",
	"last[1]",
};

static char *help_polar[] = {
	"z=1+i",
	"polar(z)",
};

static char *help_real[] = {
	"z=3+4*i",
	"real(z)",
};

static char *help_rect[] = {
	"z=exp(i*theta)",
	"rect(z)",
};

// circular functions

static char *help_arccos[] = {
	"xrange=(-1,1)",
	"yrange=(0,pi)",
	"draw(arccos(x))",
};

static char *help_arcsin[] = {
	"xrange=(-1,1)",
	"yrange=(-pi/2,pi/2)",
	"draw(arcsin(x))",
};

static char *help_arctan[] = {
	"draw(arctan(x))",
};

static char *help_cos[] = {
	"draw(cos(x))",
};

static char *help_sin[] = {
	"draw(sin(x))",
};

static char *help_tan[] = {
	"xrange=(-pi,pi)",
	"draw(tan(x))",
};

// hyperbolic functions

static char *help_arccosh[] = {
	"draw(arccosh(x))",
};

static char *help_arcsinh[] = {
	"draw(arcsinh(x))",
};

static char *help_arctanh[] = {
	"draw(arctanh(x))",
};

static char *help_cosh[] = {
	"draw(cosh(x))",
};

static char *help_sinh[] = {
	"draw(sinh(x))",
};

static char *help_tanh[] = {
	"draw(tanh(x))",
};

// special functions

static char *help_besselj[] = {
	"n=5/2",
	"y=besselj(x,n)",
	"y",
	"x^2 d(y,x,x) + x d(y,x) + (x^2 - n^2) y",
};

static char *help_hermite[] = {
	"n=7",
	"y=hermite(x,n)",
	"y",
	"d(y,x,x) - 2 x d(y,x) + 2 n y",
};

static char *help_laguerre[] = {
	"n=5",
	"y=laguerre(x,n)",
	"y",
	"x d(y,x,x) + (1 - x) d(y,x) + n y",
	"y=laguerre(x,n,a)",
	"y",
	"x d(y,x,x) + (a + 1 - x) d(y,x) + n y",
};

static char *help_legendre[] = {
	"n=7",
	"y=legendre(x,n)",
	"y",
	"(1 - x^2) d(y,x,x) - 2 x d(y,x) + n (n + 1) y",
};

// abs

static char *help_abs[] = {
	"abs(-1)",
	"X=(x1,x2)",
	"abs(X)",
};

// choose

static char *help_choose[] = {
	"choose(13,1)*choose(4,2)*choose(12,3)*choose(4,1)^3/choose(52,5)",
	"float",
	"choose(n,k)",
};

// circexp

static char *help_circexp[] = {
	"f=cos(x)+sin(x)",
	"circexp(f)",
};

// coeff

static char *help_coeff[] = {
	"y=a*x^2+b*x+c",
	"coeff(y,x,2)",
};

// cofactor

static char *help_cofactor[] = {
	"A=((1,2),(3,4))",
	"B=((0,0),(0,0))",
	"for(i,1,2,for(j,1,2,B[i,j]=cofactor(A,i,j)))",
	"B",
	"adj(A)-transpose(B)",
};

static char *help_defint[] = {
	"f=x^2",
	"defint(f,x,a,b)",
	"f=x*y",
	"defint(f,x,0,1,y,0,1)",
};

// deg

static char *help_deg[] = {
	"y=x^2+x+1",
	"deg(y,x)",
};

// denominator

static char *help_denominator[] = {
	"Q=A/B",
	"denominator(Q)",
};

// erf

static char *help_erf[] = {
	"draw(erf(x))",
};

// erfc

static char *help_erfc[] = {
	"draw(erfc(x))",
};

// eval

static char *help_eval[] = {
	"f=taylor(cos(t),t,6)",
	"f",
	"eval(f,t,pi/3)",
	"float",
	"r=sqrt(x^2+y^2)",
	"eval(r,x,2,y,3)",
};

// exp

static char *help_exp[] = {
	"exp(1.0)",
};

// expand

static char *help_expand[] = {
	"expand(1/(x^3+x^2))",
};

// expcos

static char *help_expcos[] = {
	"expcos(x)",
};

// expsin

static char *help_expsin[] = {
	"expsin(x)",
};

// factor

static char *help_factor[] = {
	"factor(10^20 - 1)",
};

// factorial

static char *help_factorial[] = {
	"100!",
	"float",
};

// for

static char *help_for[] = {
	"x=0",
	"y=2",
	"for(k,1,9,x=sqrt(2+x),y=2*y/x)",
	"y",
	"float(y)",
};

// leading

static char *help_leading[] = {
	"leading(5x^2+x+1)",
};

// log

static char *help_log[] = {
	"log(e)",
};

// numerator

static char *help_numerator[] = {
	"Q=A/B",
	"numerator(Q)",
};

// product

static char *help_product[] = {
	"product(k,1,4,prime(k)^2)",
	"factor",
};

// quotient

static char *help_quotient[] = {
	"p=x^2+x+1",
	"q=x+2",
	"quotient(p,q)",
};

// roots

static char *help_roots[] = {
	"y=125*x^3-1",
	"roots(y)",
	"last[1]",
};

// sqrt

static char *help_sqrt[] = {
	"sqrt(2)",
};

// sum

static char *help_sum[] = {
	"sum(k,1,10,2k-1)",
};

// taylor

static char *help_taylor[] = {
	"taylor(cos(x),x,8)",
};

#define HELP(x) do_help(x, sizeof x / sizeof (char *))

static void do_help(char **, int);

