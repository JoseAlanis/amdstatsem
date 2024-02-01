/* test-pde.c - unit tests for pde.c
 *
 * Copyright (C) 2013  Jochen Voss, Andreas Voss.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#undef NDEBUG

#include "pde.c"

#include <stdio.h>
#include <assert.h>

#include "jvrandom.h"

static void
xmul(int n, double Al, double Am, double Ar, const double *in, double *out)
/* Left-multiply 'in' with the tri-band matrix which has 'Al' on the
 * sub-diagonal, 'Am' on the diagonal and 'Ar' on the super-diagonal.
 * Store the result in 'out'.  */
{
	double xl, xm, xr;
	int i;

	xl = 0, xm = in[0], xr = in[1];
	out[0] = Al*xl + Am*xm + Ar*xr;
	for (i=1; i<n-1; ++i) {
		xl = xm, xm = xr, xr=in[i+1];
		out[i] = Al*xl + Am*xm + Ar*xr;
	}
	xl = xm, xm = xr, xr = 0;
	out[n-1] = Al*xl + Am*xm + Ar*xr;
}

static double
ddot(int n, const double *x, const double *y)
/* Compute the inner product between 'x' and 'y'.  */
{
	double res = 0;
	int i;
	for (i=0; i<n; ++i)  res += x[i] * y[i];
	return res;
}

static double
residual(int n, const double *rhs, double *x, double Al, double Am, double Ar)
{
	double *y;
	double  res;
	int  i;

	y = xnew(double, n);
	xmul(n, Al, Am, Ar, x, y);
	for (i=0; i<n; ++i)  y[i] -= rhs[i];
	res = ddot(n, y, y);
	xfree(y);
	return res;
}

int
main()
{
	int  n;

	init_noise(1853523060);
	for (n=4; n<10000; n+=71) {
		double *x, *rhs;
		int j;

		x = xnew(double, n);
		rhs = xnew(double, n);
		for (j=0; j<10; ++j) {
			double Al, Am, Ar;
			int  i;

			for (i=0; i<n; ++i) {
				rhs[i] = jvrand_standard_normal();
			}
			Al = 0.1 * jvrand_standard_normal();
			Ar = 0.1 * jvrand_standard_normal();
			do {
				Am = 1 + 0.1*jvrand_standard_normal();
			} while (Am <= fabs(Al)+fabs(Ar)+1e-6);

			solve_tridiag(n, rhs, x, Al, Am, Ar);
			assert(residual(n, rhs, x, Al, Am, Ar) < 1e-20);
		}
		xfree(rhs);
		xfree(x);
	}
	return 0;
}
