/* main.c - main program for the fast-dm project
 *
 * Copyright (C) 2006-2012  Jochen Voss, Andreas Voss.
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
#ifndef _WIN32
#include <unistd.h>
#else
#include "win32getopt.h"
#endif
#include <float.h>
#include <time.h>
#include <assert.h>

#include "fast-dm.h"


int
main(int argc, char **argv)
{
	int  continue_flag = 0;
	int  help_flag = 0;
	const char *ex_name = "experiment.ctl";
	struct experiment *ex;
	struct dataset *ds;
	clock_t  start_total, stop_total;
	double  cpu_time_used;
	int  N, optc;

	while ((optc = getopt (argc, argv, "ch")) != -1) {
		switch (optc) {
		case 'c':
			continue_flag = 1;
			break;
		case 'h':
			help_flag = 1;
			break;
		default:
			help_flag = 1;
			break;
		}
	}
	if (argc > optind+1) {
		fprintf(stderr, "error: too many arguments\n");
		help_flag = 1;
	} else if (argc > optind) {
		ex_name = argv[optind];
	}
	if (help_flag) {
		fputs("usage: fast-dm [options] [file.ctl]\n\n", stderr);
		fputs("Estimate parameters in the diffusion model.\n\n", stderr);
		fputs("Options:\n", stderr);
		fputs("  -c    continue an interrupted fast-dm run\n", stderr);
		fputs("  -h    show this help text\n\n", stderr);
		fputs("Please report bugs to <voss@seehuhn.de>.\n", stderr);
		exit(0);
	}

	ex = new_experiment(ex_name);
	if (! ex) {
		fprintf(stderr,
				"error: failed to load experiment control file \"%s\"\n",
				ex_name);
		exit(1);
	}
	experiment_print(ex);

	N=0;
	start_total = clock();
	while ((ds = experiment_get_dataset(ex, continue_flag))) {
		N++;
		dataset_print(ds);
		if (ds->samples_used == 0) {
			printf("  nothing to do!\n");
			break;
		}
		switch (ds->method) {
		case METHOD_KS:
			analyse_dataset_ks(ex, ds);
			break;
		case METHOD_ML:
			analyse_dataset_ml(ex, ds);
			break;
		case METHOD_CS:
			analyse_dataset_cs(ex, ds);
			break;
		default:
			assert(0);
		}
		delete_dataset(ds);
		fflush(NULL);
	}

	stop_total = clock();
	cpu_time_used = (double)(stop_total - start_total) / CLOCKS_PER_SEC;
	printf("%d dataset%s processed, total CPU time used: %.1fs\n",
		   N, N==1?"":"s", cpu_time_used);

	delete_experiment(ex);

	return  0;
}
