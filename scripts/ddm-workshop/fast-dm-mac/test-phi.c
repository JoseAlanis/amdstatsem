/* test-phi.c - unit tests for phi.c
 *
 * Copyright (C) 2013  Jochen Voss, Andreas Voss.
 */

#undef NDEBUG

#include "phi.c"

#include <stdio.h>

int
main()
{
	double  x;
	int  err = 0;

	for (x=-5; x<=5; x+=0.01) {
		double  y = Phi(x);
		double  z = Phi_inverse(y);
		if (fabs(z-x) <= 1e-8)  continue;

		err = 1;
		fprintf(stderr,
				"phi.c: test failure for x=%g (d=%g)\n", x, z-x);
		fprintf(stderr,
				"       Phi(%g)=%g, Phi^{-1}(%g)=%g\n", x, y, y, z);
		break;
	}
	return err;
}
