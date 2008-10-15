/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; -*- */

#include <hpcc.h>

/* Common Block Declarations */

static struct {
    int irand[2], ias[2], ics[2];
} rancom_;

#define rancom_1 rancom_

int
xjumpm_(int *jumpm, int *mult, int *iadd,
	int *irann, int *iranm, int *iam, int *icm) {
    /* System generated locals */
    int i__1;

    int i__, j[2];

/*  -- ScaLAPACK routine (version 1.7) -- */
/*     University of Tennessee, Knoxville, Oak Ridge National Laboratory, */
/*     and University of California, Berkeley. */
/*     May 1, 1997 */

    /* Parameter adjustments */
    --icm;
    --iam;
    --iranm;
    --irann;
    --iadd;
    --mult;

    if (*jumpm > 0) {
	for (i__ = 1; i__ <= 2; ++i__) {
	    iam[i__] = mult[i__];
	    icm[i__] = iadd[i__];
/* L10: */
	}
	i__1 = *jumpm - 1;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    HPL_lmul(&iam[1], &mult[1], j);
	    iam[1] = j[0];
	    iam[2] = j[1];
	    HPL_lmul(&icm[1], &mult[1], j);
	    HPL_ladd(&iadd[1], j, &icm[1]);
/* L20: */
	}
	HPL_lmul(&irann[1], &iam[1], j);
	HPL_ladd(j, &icm[1], &iranm[1]);
    } else {
	iranm[1] = irann[1];
	iranm[2] = irann[2];
    }

    return 0;
} /* xjumpm_ */


int setran_(int *iran, int *ia, int *ic) {
    int i__;
/*  -- ScaLAPACK routine (version 1.7) -- */
/*     University of Tennessee, Knoxville, Oak Ridge National Laboratory, */
/*     and University of California, Berkeley. */
/*     May 1, 1997 */

    /* Parameter adjustments */
    --ic;
    --ia;
    --iran;

    for (i__ = 1; i__ <= 2; ++i__) {
	rancom_1.irand[i__ - 1] = iran[i__];
	rancom_1.ias[i__ - 1] = ia[i__];
	rancom_1.ics[i__ - 1] = ic[i__];
/* L10: */
    }

    return 0;
} /* setran_ */


int jumpit_(int *mult, int *iadd, int *irann, int *iranm) {
    int j[2];

/*  -- ScaLAPACK routine (version 1.7) -- */
/*     University of Tennessee, Knoxville, Oak Ridge National Laboratory, */
/*     and University of California, Berkeley. */
/*     May 1, 1997 */

    /* Parameter adjustments */
    --iranm;
    --irann;
    --iadd;
    --mult;

    HPL_lmul(&irann[1], &mult[1], j);
    HPL_ladd(j, &iadd[1], &iranm[1]);

    rancom_1.irand[0] = iranm[1];
    rancom_1.irand[1] = iranm[2];

    return 0;
} /* jumpit_ */

double
pdrand() {
    /* System generated locals */
    double ret_val;

    /* Local variables */
    int j[2];
/*  -- ScaLAPACK routine (version 1.7) -- */
/*     University of Tennessee, Knoxville, Oak Ridge National Laboratory, */
/*     and University of California, Berkeley. */
/*     May 1, 1997 */

    ret_val = ((double) rancom_1.irand[0] + (double) rancom_1.irand[1]
	     * 65536.) / 2147483648.;

    HPL_lmul(rancom_1.irand, rancom_1.ias, j);
    HPL_ladd(j, rancom_1.ics, rancom_1.irand);

    return ret_val;
} /* pdrand */
