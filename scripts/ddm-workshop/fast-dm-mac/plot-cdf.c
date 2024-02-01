/* plot-cdf.c - plot cumulative distribution functions
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
#include <math.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include "win32getopt.h"
#endif
#include <time.h>

#include "fast-dm.h"
#include "util.h"


#define  N  3000

int
main (int argc, char **argv)
{
	struct F_calculator *fc;
	const char *outname = NULL;
	FILE *out;
	double  para[p_count], z, zr, precision;
	double  dt = 30.0/N;
	double  F[2*N+1];
	clock_t          start, stop;
	int  optc, i;
	int  help_flag = 0;

	para[p_a] = 1.0;
	zr = 0.5;
	para[p_v] = 0.0;
	para[p_t0] = 0.3;
	para[p_d] = 0.0;
	para[p_szr] = 0.0;
	para[p_sv] = 0.0;
	para[p_st0] = 0.0;
	precision = 3;

	while ((optc = getopt (argc, argv, "a:z:v:t:d:Z:V:T:p:o:h")) != -1) {
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

	if (help_flag) {
		fputs("usage: plot-cdf [options]\n\n", stderr);
		fputs("Provides data for the combined CDFs from lower and upper thresholds of\n", stderr);
		fputs("the diffusion model.\n\n", stderr);
		fputs("Options:\n", stderr);
		fputs("  -a VAL    fix parameter a (interval width) to VAL\n", stderr);
		fputs("  -z VAL    fix parameter zr (starting point) to VAL\n", stderr);
		fputs("  -v VAL    fix parameter v (drift) to VAL\n", stderr);
		fputs("  -t VAL    fix parameter t0 (nondec. component) to VAL\n", stderr);
		fputs("  -d VAL    fix parameter d (nondec. difference) to VAL\n", stderr);
		fputs("  -Z VAL    fix parameter szr (width of supp. of zr) to VAL\n", stderr);
		fputs("  -V VAL    fix parameter sv (std. dev. of v) to VAL\n", stderr);
		fputs("  -T VAL    fix parameter st0 (width of supp. of t0) to VAL\n", stderr);
		fputs("  -p VAL    set the computational precision\n", stderr);
		fputs("  -o FILE   output data file name\n", stderr);
		fputs("  -h        show this help text\n\n", stderr);
		fputs("Please report bugs to <voss@seehuhn.de>.\n", stderr);
		exit(0);
	}

	z = zr * para[p_a];

	params_write(stderr, para, zr, precision, 0);
	params_check(para, zr);

	set_precision (precision);

	start = clock ();
	fc = F_new (para);
	F_start (fc, b_upper);
	for (i=0; i<=N; ++i) {
		F[N+i] = F_get_val (fc, i*dt, z);
	}
	F_start (fc, b_lower);
	for (i=1; i<=N; ++i) {
		F[N-i] = F_get_val (fc, i*dt, z);
	}
	F_delete (fc);
	stop = clock ();

	fprintf (stderr, "time = %.2f\n",
			 (double)(stop - start) / CLOCKS_PER_SEC);

	if (outname) {
		char buffer[1024];
		snprintf(buffer, 1024, "%s", outname);
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

	for (i=0; i<=2*N; ++i) {
		fprintf (out, "%f %.20f\n", (i-N)*dt, F[i]);
	}

	if (outname) {
		fclose(out);
	}

	return  0;
}
