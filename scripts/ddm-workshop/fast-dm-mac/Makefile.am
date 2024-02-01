## Process this file with automake to produce Makefile.in
# Copyright 2006-2013  Jochen Voss

bin_PROGRAMS = fast-dm
noinst_PROGRAMS = plot-cdf plot-density construct-samples

fast_dm_SOURCES = main.c method-ks.c method-ml.c method-cs.c		\
	EZ-diff.c density.c cdf.c pde.c parameters.c experiment.c	\
	dataset.c file.c simplex2.c container.c xmalloc.c phi.c		\
	precision.c fast-dm.h

plot_density_SOURCES = plot-density.c density.c precision.c util.c	\
	util.h fast-dm.h

plot_cdf_SOURCES = plot-cdf.c cdf.c phi.c pde.c xmalloc.c precision.c	\
	util.c util.h fast-dm.h

construct_samples_SOURCES = construct-samples.c cdf.c phi.c pde.c	\
	xmalloc.c precision.c util.c util.h jvrandom.h fast-dm.h

######################################################################
# testing

MYTESTS = test-phi test-simplex2 test-container test-pde test-density \
	test-file test-method-ks
check_PROGRAMS = $(MYTESTS) fast-dm construct-samples
TESTS = $(MYTESTS) test-full

test_phi_SOURCES = test-phi.c fast-dm.h

test_simplex2_SOURCES = test-simplex2.c xmalloc.c fast-dm.h

test_container_SOURCES = test-container.c xmalloc.c fast-dm.h

test_pde_SOURCES = test-pde.c precision.c xmalloc.c fast-dm.h

test_density_SOURCES = test-density.c cdf.c phi.c pde.c precision.c	\
	xmalloc.c fast-dm.h

test_file_SOURCES = test-file.c container.c xmalloc.c fast-dm.h

test_method_ks_SOURCES = test-method-ks.c cdf.c simplex2.c	\
	experiment.c parameters.c pde.c phi.c file.c dataset.c	\
	EZ-diff.c precision.c container.c xmalloc.c fast-dm.h

######################################################################
# extra files

EXTRA_DIST = README.txt NEWS.txt MANUAL.txt test-full test-helper	\
	test-generate win32erf.c win32dir.c win32dir.h win32getopt.c	\
	win32getopt.h
CLEANFILES = *.aux *.dvi
