/* test-method-ks.c - Unit tests for method-ks.c
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

#include "method-ks.c"

#include <stdio.h>

static double
manually_get_KS_distance(const struct samples *samples, const double *para,
						 double z)
{
	struct F_calculator *fc;
	double dist = 0.0;
	double before, after, exact, d;
	int i, n_minus, n_plus;

	n_plus = samples->plus_used;
	n_minus = samples->minus_used;

	fc = F_new(para);

	F_start(fc, b_upper);
	for (i=0; i<n_plus; ++i) {
		exact = F_get_val(fc, samples->plus_data[i], z);

		before = (n_minus + i) / (double)(n_minus + n_plus);
		d = fabs(exact - before);
		if (d > dist) dist = d;

		after = (n_minus + i + 1) / (double)(n_minus + n_plus);
		d = fabs(exact - after);
		if (d > dist) dist = d;
	}

	F_start(fc, b_lower);
	for (i=0; i<n_minus; ++i) {
		exact = F_get_val(fc, samples->minus_data[i], z);

		before = (n_minus - i - 1) / (double)(n_minus + n_plus);
		d = fabs(exact - before);
		if (d > dist) dist = d;

		after = (n_minus - i) / (double)(n_minus + n_plus);
		d = fabs(exact - after);
		if (d > dist) dist = d;
	}

	F_delete (fc);
	return dist;
}

static int
do_one_test(const char *test_name, const struct samples *samples,
			const double *para, double zr)
{
	struct distance_ks *dist;
	double dist_ks, dist_manual;
	double z;

	z = para[p_a] * zr;

	dist = KS_get_distance(samples, para);
	dist_ks = distance_interpolate(dist, z);
	dist_manual = manually_get_KS_distance(samples, para, z);

	if (fabs(dist_ks - dist_manual) < 0.005) {
		printf("%s: ok\n", test_name);
		return 0;
	} else {
		fprintf(stderr, "%s: FAILED, %g != %g\n",
				test_name, dist_ks, dist_manual);
		return 1;
	}
}

int
main()
{
	struct samples *samples;
	double para[p_count], zr;
	int error = 0;

	samples = new_samples("test");
	samples_add_sample(samples, 0.5, 0);
	samples_add_sample(samples, 0.5, 1);
	samples_add_sample(samples, 1.0, 0);
	samples_add_sample(samples, 1.0, 1);
	samples_add_sample(samples, 1.5, 0);
	samples_add_sample(samples, 1.5, 1);
	samples_sort(samples);

	set_precision(3.0);

	para[p_a] = 2.0;
	zr = 0.5;
	para[p_v] = 0.1;
	para[p_t0] = 0.3;
	para[p_d] = 0.0;
	para[p_szr] = 0.0;
	para[p_sv] = 0.0;
	para[p_st0] = 0.0;
	error |= do_one_test("test 1", samples, para, zr);

	para[p_a] = 2.0;
	zr = 0.5;
	para[p_v] = 0.1;
	para[p_t0] = 0.3;
	para[p_d] = 0.0;
	para[p_szr] = 0.1;
	para[p_sv] = 0.0;
	para[p_st0] = 0.0;
	error |= do_one_test("test 2", samples, para, zr);

	para[p_a] = 2.0;
	zr = 0.5;
	para[p_v] = 0.1;
	para[p_t0] = 0.3;
	para[p_d] = 0.2;
	para[p_szr] = 0.0;
	para[p_sv] = 0.0;
	para[p_st0] = 0.0;
	error |= do_one_test("test 3", samples, para, zr);

	para[p_a] = 2.0;
	zr = 0.5;
	para[p_v] = 0.1;
	para[p_t0] = 0.3;
	para[p_d] = 0.0;
	para[p_szr] = 0.0;
	para[p_sv] = 0.2;
	para[p_st0] = 0.0;
	error |= do_one_test("test 4", samples, para, zr);

	para[p_a] = 2.0;
	zr = 0.5;
	para[p_v] = 0.1;
	para[p_t0] = 0.3;
	para[p_d] = 0.0;
	para[p_szr] = 0.0;
	para[p_sv] = 0.0;
	para[p_st0] = 0.2;
	error |= do_one_test("test 5", samples, para, zr);

	if (error) {
		fprintf(stderr, "some tests failed!\n");
	}
	return error;
}
