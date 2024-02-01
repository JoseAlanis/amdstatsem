AC_INIT(fast-dm, 30.2, voss@seehuhn.de)
AC_CONFIG_SRCDIR(fast-dm.h)
AM_INIT_AUTOMAKE([dist-zip])

AC_PROG_CC
AC_CHECK_LIB([m], [exp])
AC_CHECK_LIB([m],[fabs],[AC_DEFINE([HAVE_FABS],[1],[libm includes fabs])])
AC_CHECK_LIB([blas], [daxpy_])

AC_OUTPUT([Makefile])
