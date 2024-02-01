/* test-density.c - unit tests for density.c
 *
 * Copyright (C) 2013  Andreas Voss, Jochen Voss.
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

#include "density.c"

#include <stdlib.h>
#include <stdio.h>

void
cdf_test(double tmax, double a, double zr, double v, double t0, double szr,
		 double sv, double st0)
{
	struct para P;
	struct function F = {
		integral_t0_g_minus,
		&P
	};
	double cdf_para[p_count];
	struct F_calculator *fc;
	double cdf1, cdf2;
	double diff;

	cdf_para[p_a] = a;
	cdf_para[p_v] = v;
	cdf_para[p_t0] = t0;
	cdf_para[p_szr] = szr;
	cdf_para[p_sv] = sv;
	cdf_para[p_st0] = st0;
	fc = F_new(cdf_para);

	F_start(fc, b_lower);
	cdf1 = F_get_val(fc, 0, zr*a) - F_get_val(fc, tmax, zr*a) ;

	P.a = a;
	P.zr = zr;
	P.v = v;
	P.szr = szr;
	P.sv = sv;
	P.st0 = st0;
	cdf2 = integrate(&F, -t0, tmax-t0, 0.001);

	diff = fabs(cdf1-cdf2);
	if (diff<0.01)
		printf("test-density: |%g - %g| = %g, ok\n", cdf1, cdf2, diff);
	else {
		fprintf(stderr, "test-density: |%g - %g| = %g, failed!\n",
				cdf1, cdf2, diff);
		exit(1);
	}
}

int
main()
{
	set_precision(2.5);

	// general tests
	cdf_test(1, 2, 0.5, 1, .5, 0.9, 1, .5);
	cdf_test(1, 2, 0.2, 1, .5, .3, 5, .5);
	cdf_test(10, 2, 0.5, -1, .5, .5, 1, .5);

	// zero variabilities
	cdf_test(1, 2, 0.3, 1, .5, 0, 0, 0);
	cdf_test(1, 2, 0.3, 1, .5, 0.1, 0, 0);
	cdf_test(1, 2, 0.3, 1, .5, 0, 0.1, 0);
	cdf_test(1, 2, 0.3, 1, .5, 0, 0, 0.1);

	// boundary cases
	cdf_test(2, 3, 0.25, 1, .5, .5, 0, 0);
	cdf_test(1, 2, 0.3, 1, .3, 0, 0, .6);
	return 0;
}
