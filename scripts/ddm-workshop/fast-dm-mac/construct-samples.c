/* construct-samples.c - construct sample data for the article
 *
 * Copyright (C) 2006  Jochen Voss, Andreas Voss.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include "win32getopt.h"
#endif
#include <math.h>
#include <time.h>
#include <assert.h>

#include "fast-dm.h"
#include "util.h"
#include "jvrandom.h"


static int
compare_doubles (const void *a, const void *b)
{
	const double *da = a;
	const double *db = b;

	if (*da < *db)  return -1;
	if (*da > *db)  return 1;
	return  0;
}

static int
find_slot(double target, const double *value, int l, int r)
{
	int m = (l+r)/2;
	if (m == l) {
		return l;
	} else if (value[m] > target) {
		return find_slot(target, value, l, m);
	} else {
		return find_slot(target, value, m, r);
	}
}

int
main (int argc, char **argv)
{
	struct F_calculator *fc;
	const char *outname, *infoname;
	double  para[p_count], zr, precision;
	double  *F;
	double  **Fs, Fs_min, Fs_max;
	double  t_min, t_max, dt;
	int  help_flag = 0;
	int  random_flag = 0;
	int  optc, i, j, N, s_size, s_count;

	outname = NULL;
	infoname = NULL;
	para[p_a] = 1.0;
	zr = 0.5;
	para[p_v] = 0.0;
	para[p_t0] = 0.3;
	para[p_d] = 0.0;
	para[p_szr] = 0.0;
	para[p_sv] = 0.0;
	para[p_st0] = 0.0;
	precision = 4;
	s_size = 100;
	s_count = 1;

	while ((optc = getopt (argc, argv, "a:z:v:t:d:Z:V:T:n:N:i:rp:o:h")) != -1) {
		switch (optc) {
		case 'a':
			para[p_a] = atof (optarg);
			break;
		case 'z':
			zr = atof (optarg);
			break;
		case 'v':
			para[p_v] = atof (optarg);
			break;
		case 't':
			para[p_t0] = atof (optarg);
			break;
		case 'd':
			para[p_d] = atof (optarg);
			break;
		case 'Z':
			para[p_szr] = atof (optarg);
			break;
		case 'V':
			para[p_sv] = atof (optarg);
			break;
		case 'T':
			para[p_st0] = atof (optarg);
			break;
		case 'n':
			s_size = atoi (optarg);
			if (s_size<1 || s_size>1000000) {
				fprintf(stderr,
						"error: invalid sample size\n");
				exit(1);
			}
			break;
		case 'N':
			s_count = atoi (optarg);
			if (s_count<1 || s_count>1000000) {
				fprintf(stderr,
						"error: invalid number of samples\n");
				exit(1);
			}
			break;
		case 'i':
			infoname = optarg;
			break;
		case 'r':
			random_flag = 1;
			break;
		case 'p':
			precision = atof(optarg);
			break;
		case 'o':
			outname = optarg;
			break;
		case 'h':
			help_flag = 1;
			break;
		default:
			help_flag = 1;
			break;
		}
	}

	if (s_count>1 && ! random_flag) {
		fprintf (stderr,
				 "error: generating multiple samples but -r not given\n");
		exit(1);
	}
	if (s_count>1 && (!outname || !strchr(outname,'%'))) {
		fprintf (stderr, "error: generating multiple samples, need -o with '%%d'\n");
		exit(1);
	}
	if (help_flag) {
		fputs("usage: construct-samples [options]\n\n", stderr);
		fputs("Construct samples from the diffusion model.\n\n", stderr);
		fputs("Options:\n", stderr);
		fputs("  -a VAL    fix parameter a (interval width) to VAL\n", stderr);
		fputs("  -z VAL    fix parameter zr (starting point) to VAL\n", stderr);
		fputs("  -v VAL    fix parameter v (drift) to VAL\n", stderr);
		fputs("  -t VAL    fix parameter t0 (nondec. component) to VAL\n", stderr);
		fputs("  -d VAL    fix parameter d (nondec. difference) to VAL\n", stderr);
		fputs("  -Z VAL    fix parameter szr (width of supp. of zr) to VAL\n", stderr);
		fputs("  -V VAL    fix parameter sv (std. dev. of v) to VAL\n", stderr);
		fputs("  -T VAL    fix parameter st0 (width of supp. of t0) to VAL\n", stderr);
		fputs("  -n COUNT  set the sample size\n", stderr);
		fputs("  -N COUNT  number of samples to generate\n", stderr);
		fputs("  -i FILE   store parameter information in FILE\n", stderr);
		fputs("  -r        generate a random sample (default: output quantiles)\n", stderr);
		fputs("  -p VAL    set the computational precision\n", stderr);
		fputs("  -o FILE   output data file name ('%d' is replaced with sample no.)\n", stderr);
		fputs("  -h        show this help text\n\n", stderr);
		fputs("Please report bugs to <voss@seehuhn.de>.\n", stderr);
		exit(0);
	}

	fprintf(stderr, "generating %d %s sample%s of size %d\n",
			s_count, random_flag?"random":"deterministic",
			s_count!=1?"s":"", s_size);
	params_write(stderr, para, zr, precision, s_size);
	params_check(para, zr);

	set_precision (precision);

	/* get the F-values for the samples */
	Fs = xnew(double *, s_count);
	Fs_min = 1;
	Fs_max = 0;
	if (random_flag)  init_noise(time(NULL));
	for (j=0; j<s_count; ++j) {
		Fs[j] = xnew(double, s_size);
		if (random_flag) {
			for (i=0; i<s_size; ++i) {
				Fs[j][i] = jvrand_real2();
				if (Fs[j][i] > Fs_max)  Fs_max = Fs[j][i];
				if (Fs[j][i] < Fs_min)  Fs_min = Fs[j][i];
			}
		} else {
			for (i=0; i<s_size; ++i) {
				Fs[j][i] = (i+0.5)/s_size;
			}
			Fs_min = Fs[j][0];
			Fs_max = Fs[j][s_size-1];
		}
	}

	fc = F_new (para);

	/* get the required t-range */
	t_max = 0.5;
	F_start (fc, b_upper);
	while (F_get_val (fc, t_max, zr*para[p_a]) < Fs_max)
		t_max += 0.1;
	t_min = -0.5;
	F_start (fc, b_lower);
	while (F_get_val (fc, -t_min, zr*para[p_a]) > Fs_min)
		t_min -= 0.1;

	/* get a table of F-values */
	N = (int)((t_max-t_min)/0.001 + 0.5);
	dt = (t_max-t_min)/N;
	F = xnew(double, N+1);

	F_start (fc, b_upper);
	for (i=0; i<=N; ++i) {
		double  t = t_min+i*dt;
		if (t < 0)  continue;
		F[i] = F_get_val (fc, t, zr*para[p_a]);
	}
	F_start (fc, b_lower);
	for (i=N; i>=0; --i) {
		double  t = -(t_min+i*dt);
		if (t < 0)  continue;
		F[i] = F_get_val (fc, t, zr*para[p_a]);
	}

	F_delete (fc);

	/* protect against rounding errors:
	 * make F increasing and restrict to valid range */
	for (i=0; i<=N; ++i) {
		if (F[i] < 0)
			F[i] = 0;
		if (F[i] > 1)
			F[i] = 1;
	}
	qsort(F, N+1, sizeof(double), compare_doubles);
	if (F[0] > Fs_min)
		F[0] = Fs_min;
	if (F[N] < Fs_max)
		F[N] = Fs_max;

	for (j=0; j<s_count; ++j) {
		FILE *out;

		if (outname) {
			char buffer[1024];
			snprintf(buffer, 1024, outname, j);
			out = fopen(buffer, "w");
			if (! out) {
				fprintf(stderr,
						"error: cannot open '%s' for writing\n",
						buffer);
				exit(1);
			}
			fprintf(stderr, "writing %s ...\n", buffer);
		} else {
			out = stdout;
		}
		for (i=0; i<s_size; ++i) {
			double  y = Fs[j][i];
			int  k = find_slot(y, F, 0, N);
			double  t = t_min + (k + (y-F[k])/(F[k+1]-F[k]))*dt;

			assert (F[k]<=y && y<=F[k+1]);
			fprintf (out, "%3d %6.3f\n", t >= 0, fabs(t));
		}
		if (outname)  fclose(out);
	}

	if (infoname) {
		FILE *fd = fopen(infoname, "w");
		if (! fd) {
			fprintf(stderr,
					"error: cannot open '%s' for writing\n",
					infoname);
			exit(1);
		}
		params_write(fd, para, zr, precision, s_size);
		fclose(fd);
	}

	xfree(F);
	for (j=0; j<s_count; ++j) xfree(Fs[j]);
	xfree(Fs);

	return  0;
}
