/* test-simplex2.c - unit tests for simplex2.c
 *
 * Copyright (C) 2013  Jochen Voss, Andreas Voss.
 */

#undef NDEBUG

#include "simplex2.c"

#include <stdio.h>
#include <math.h>

static void
fn1(const double *x, double res[2], void *data)
{
	res[0] = x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
	res[1] = 0;

}

static void
fn1b(const double *x, double res[2], void *data)
{
	res[0] = x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
	res[1] = x[0]+100;

}

static void
fn2(const double *x, double res[2], void *data)
{
	res[0] = 0;
	res[1] = x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
}

static void
fn3(const double *x, double res[2], void *data)
{
	res[0] = (x[0]<=0) ? 1-x[0] : 0;
	res[1] = (x[0]+1)*(x[0]+1) + x[1]*x[1] + x[2]*x[2];
}

static void
fn4(const double *x, double res[2], void *data)
{
	res[0] = (x[0]>=0) ? 1+x[0] : 0;
	res[1] = (x[0]-1)*(x[0]-1) + x[1]*x[1] + x[2]*x[2];
}

static void
doit(void (*fn)(const double *x, double res[2], void *data), const char *name)
{
	double  eps[3] = { 1.0, 1.0, 1.0 };
	double  x[3], val[2], d;
	double  prec = 1e-6;
	static int  n = 1;

	x[0] = 10; x[1] = 2; x[2] = 3;
	simplex2(3, eps, prec, fn, x, val, NULL, NULL);
	d = fabs(x[0]);
	if (d < fabs(x[1])) d = fabs(x[1]);
	if (d < fabs(x[2])) d = fabs(x[2]);
	if (d <= 2*prec) {
		printf ("test %d (%s): %g %g -> ok\n",
				n++, name, val[0], val[1]);
	} else {
		printf ("test %d (%s): x=(%g,%g,%g), val=(%g,%g) -> failed\n",
				n++, name, x[0], x[1], x[2], val[0], val[1]);
	}
}

int
main()
{
	doit(fn1, "only first value set");
	doit(fn1b, "first value used, second ignored");
	doit(fn2, "only second value set");
	doit(fn3, "penalise x[0]<=0");
	doit(fn4, "penalise x[0]>=0");

	return  0;
}
