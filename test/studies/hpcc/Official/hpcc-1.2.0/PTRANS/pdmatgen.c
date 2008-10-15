/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; -*- */

#include <hpcc.h>

int
pdmatgen(int *ictxt, char *aform, char *diag, int *m, int *n, int *mb, int *nb, double *aMat,
  int *lda, int *iarow, int *iacol, int *iseed, int *iroff, int *irnum, int *icoff, int *icnum,
  int * myrow, int *mycol, int *nprow, int *npcol, double alpha) {
  /* System generated locals */
  long a_dim1, a_offset;
  int i__1, i__2, i__3, i__4;
  double d__1, d_tmp;

    /* Local variables */
    static int i__, j, ic, ik, jk, ir, mp, nq, ia1[2], ia2[2], ia3[2],
	    ia4[2], ia5[2], ib1[2], ib2[2], ib3[2], ic1[2], ic2[2], ic3[2],
	    ic4[2], ic5[2], iadd[2], mend, nend, moff, noff;
    static int herm;
    static int info, npmb, nqnb;
    static int tran;
    static int mult[2];
    static int symm;
    static int iran1[2], iran2[2], iran3[2], iran4[2], itmp1[2], itmp2[2],
	     jump1, jump2, jump3, jump4, jump5, jump6, jump7, itmp3[2];
    static int ioffc, jseed[2];
    static int ioffr, mrcol, maxmn, mrrow;

/*  -- ScaLAPACK routine (version 1.0) -- */
/*     University of Tennessee, Knoxville, Oak Ridge National Laboratory, */
/*     and University of California, Berkeley. */
/*     February 28, 1995 */

/*  Purpose */

/*  PDMATGEN : Parallel Real Double precision MATrix GENerator. */
/*  Generate (or regenerate) a distributed matrix A (or sub-matrix of A). */

/*  Arguments */

/*  ICTXT   (global input) INTEGER */
/*          The BLACS context handle, indicating the global context of */
/*          the operation. The context itself is global. */

/*  AFORM   (global input) CHARACTER*1 */
/*          if AFORM = 'S' : A is returned is a symmetric matrix. */
/*          if AFORM = 'H' : A is returned is a Hermitian matrix. */
/*          if AFORM = 'T' : A is overwritten with the transpose of */
/*                           what would normally be generated. */
/*          if AFORM = 'C' : A is overwritten with the conjugate trans- */
/*                           pose of what would normally be generated. */
/*          otherwise a random matrix is generated. */

/*  DIAG    (global input) CHARACTER*1 */
/*          if DIAG = 'D' : A is diagonally dominant. */

/*  M       (global input) INTEGER */
/*          The number of rows in the generated distributed matrix. */

/*  N       (global input) INTEGER */
/*          The number of columns in the generated distributed */
/*          matrix. */

/*  MB      (global input) INTEGER */
/*          The row blocking factor of the distributed matrix A. */

/*  NB      (global input) INTEGER */
/*          The column blocking factor of the distributed matrix A. */

/*  A       (local output) DOUBLE PRECISION, pointer into the local */
/*          memory to an array of dimension ( LDA, * ) containing the */
/*          local pieces of the distributed matrix. */

/*  LDA     (local input) INTEGER */
/*          The leading dimension of the array containing the local */
/*          pieces of the distributed matrix A. */

/*  IAROW   (global input) INTEGER */
/*          The row processor coordinate which holds the first block */
/*          of the distributed matrix A. */

/*  IACOL   (global input) INTEGER */
/*          The column processor coordinate which holds the first */
/*          block of the distributed matrix A. */

/*  ISEED   (global input) INTEGER */
/*          The seed number to generate the distributed matrix A. */

/*  IROFF   (local input) INTEGER */
/*          The number of local rows of A that have already been */
/*          generated.  It should be a multiple of MB. */

/*  IRNUM   (local input) INTEGER */
/*          The number of local rows to be generated. */

/*  ICOFF   (local input) INTEGER */
/*          The number of local columns of A that have already been */
/*          generated.  It should be a multiple of NB. */

/*  ICNUM   (local input) INTEGER */
/*          The number of local columns to be generated. */

/*  MYROW   (local input) INTEGER */
/*          The row process coordinate of the calling process. */

/*  MYCOL   (local input) INTEGER */
/*          The column process coordinate of the calling process. */

/*  NPROW   (global input) INTEGER */
/*          The number of process rows in the grid. */

/*  NPCOL   (global input) INTEGER */
/*          The number of process columns in the grid. */

/*  ALPHA   (global input) DOUBLE PRECISION, multiplication factor for old content of `A':
            A(I,J) <- ALPHA * A(I,J) + RANDOM(I,J)
 If ALPHA is zero then A is overwritten with random numbers. For non-zero ALPHAs, A gets updated.
 */

/*  Notes */

/*  The code is originally developed by David Walker, ORNL, */
/*  and modified by Jaeyoung Choi, ORNL. */

/*  Reference: G. Fox et al. */
/*  Section 12.3 of "Solving problems on concurrent processors Vol. I" */

/*     Test the input arguments */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    aMat -= a_offset;

    /* Function Body */
    mp = numroc_(m, mb, myrow, iarow, nprow);
    nq = numroc_(n, nb, mycol, iacol, npcol);
    symm = (*aform == 'S' ? 1 : 0);
    herm = (*aform == 'H' ? 1 : 0);
    tran = (*aform == 'T' ? 1 : 0);

    info = 0;
    if (! (symm || herm || tran) && *aform != 'C' && *aform != 'N') {
      info = 2;
    } else if (*diag != 'D' && *diag != 'N') {
      info = 3;
    } else if (symm || herm) {
	if (*m != *n) {
	    info = 5;
	} else if (*mb != *nb) {
	    info = 7;
	}
    } else if (*m < 0) {
	info = 4;
    } else if (*n < 0) {
	info = 5;
    } else if (*mb < 1) {
	info = 6;
    } else if (*nb < 1) {
	info = 7;
    } else if (*lda < 0) {
	info = 9;
    } else if (*iarow < 0 || *iarow >= *nprow) {
	info = 10;
    } else if (*iacol < 0 || *iacol >= *npcol) {
	info = 11;
    } else if (*iroff % *mb > 0) {
	info = 13;
    } else if (*irnum > mp - *iroff) {
	info = 14;
    } else if (*icoff % *nb > 0) {
	info = 15;
    } else if (*icnum > nq - *icoff) {
	info = 16;
    } else if (*myrow < 0 || *myrow >= *nprow) {
	info = 17;
    } else if (*mycol < 0 || *mycol >= *npcol) {
	info = 18;
    } else if (*diag == 'D' && alpha != 0.0) {
	info = 19; /* diagonal scaling is not implemented with matrix update (rather than overwrite) */
    }
    if (info != 0) {
	pxerbla( ictxt, "PDMATGEN", &info );
	return 0;
    }

    mrrow = (*nprow + *myrow - *iarow) % *nprow;
    mrcol = (*npcol + *mycol - *iacol) % *npcol;
    npmb = *nprow * *mb;
    nqnb = *npcol * *nb;
    moff = *iroff / *mb;
    noff = *icoff / *nb;
    mend = iceil_(irnum, mb) + moff;
    nend = iceil_(icnum, nb) + noff;

    mult[0] = 20077;
    mult[1] = 16838;
    iadd[0] = 12345;
    iadd[1] = 0;
    jseed[0] = *iseed;
    jseed[1] = 0;

/*     Symmetric or Hermitian matrix will be generated. */

    if (symm || herm) {

/*        First, generate the lower triangular part (with diagonal block) */

	jump1 = 1;
	jump2 = npmb;
	jump3 = *m;
	jump4 = nqnb;
	jump5 = *nb;
	jump6 = mrcol;
	jump7 = *mb * mrrow;

	xjumpm_(&jump1, mult, iadd, jseed, iran1, ia1, ic1);
	xjumpm_(&jump2, mult, iadd, iran1, itmp1, ia2, ic2);
	xjumpm_(&jump3, mult, iadd, iran1, itmp1, ia3, ic3);
	xjumpm_(&jump4, ia3, ic3, iran1, itmp1, ia4, ic4);
	xjumpm_(&jump5, ia3, ic3, iran1, itmp1, ia5, ic5);
	xjumpm_(&jump6, ia5, ic5, iran1, itmp3, itmp1, itmp2);
	xjumpm_(&jump7, mult, iadd, itmp3, iran1, itmp1, itmp2);
	xjumpm_(&noff, ia4, ic4, iran1, itmp1, itmp2, itmp3);
	xjumpm_(&moff, ia2, ic2, itmp1, iran1, itmp2, itmp3);
	setran_(iran1, ia1, ic1);

	for (i__ = 1; i__ <= 2; ++i__) {
	    ib1[i__ - 1] = iran1[i__ - 1];
	    ib2[i__ - 1] = iran1[i__ - 1];
	    ib3[i__ - 1] = iran1[i__ - 1];
/* L10: */
	}

	jk = 1;
	i__1 = nend;
	for (ic = noff + 1; ic <= i__1; ++ic) {
	    ioffc = ((ic - 1) * *npcol + mrcol) * *nb;
	    i__2 = *nb;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		if (jk > *icnum) {
		    goto L90;
		}

		ik = 1;
		i__3 = mend;
		for (ir = moff + 1; ir <= i__3; ++ir) {
		    ioffr = ((ir - 1) * *nprow + mrrow) * *mb;

		    if (ioffr > ioffc) {
			i__4 = *mb;
			for (j = 1; j <= i__4; ++j) {
			    if (ik > *irnum) {
				goto L60;
			    }
			    aMat[ik + jk * a_dim1] = alpha * aMat[ik + jk * a_dim1] + 1. - pdrand() * 2.;
			    ++ik;
/* L20: */
			}

		    } else if (ioffc == ioffr) {
			ik = ik + i__ - 1;
			if (ik > *irnum) {
			    goto L60;
			}
			i__4 = i__ - 1;
			for (j = 1; j <= i__4; ++j) {
			    aMat[ik + jk * a_dim1] = alpha * aMat[ik + jk * a_dim1] + 1. - pdrand() * 2.;
/* L30: */
			}
			aMat[ik + jk * a_dim1] = alpha * aMat[ik + jk * a_dim1] + 1. - pdrand() * 2.;
			i__4 = *mb - i__;
			for (j = 1; j <= i__4; ++j) {
			    if (ik + j > *irnum) {
				goto L60;
			    }
           d_tmp =  1. - pdrand() * 2.;
			    aMat[ik + j + jk * a_dim1] = alpha * aMat[ik + j + jk * a_dim1] + d_tmp;
			    aMat[ik + (jk + j) * a_dim1] = alpha * aMat[ik + (jk + j) * a_dim1] + d_tmp;
/* L40: */
			}
			ik = ik + *mb - i__ + 1;
		    } else {
			ik += *mb;
		    }

		    jumpit_(ia2, ic2, ib1, iran2);
		    ib1[0] = iran2[0];
		    ib1[1] = iran2[1];
/* L50: */
		}

L60:
		++jk;
		jumpit_(ia3, ic3, ib2, iran3);
		ib1[0] = iran3[0];
		ib1[1] = iran3[1];
		ib2[0] = iran3[0];
		ib2[1] = iran3[1];
/* L70: */
	    }

	    jumpit_(ia4, ic4, ib3, iran4);
	    ib1[0] = iran4[0];
	    ib1[1] = iran4[1];
	    ib2[0] = iran4[0];
	    ib2[1] = iran4[1];
	    ib3[0] = iran4[0];
	    ib3[1] = iran4[1];
/* L80: */
	}

/*        Next, generate the upper triangular part. */

L90:
	mult[0] = 20077;
	mult[1] = 16838;
	iadd[0] = 12345;
	iadd[1] = 0;
	jseed[0] = *iseed;
	jseed[1] = 0;

	jump1 = 1;
	jump2 = nqnb;
	jump3 = *n;
	jump4 = npmb;
	jump5 = *mb;
	jump6 = mrrow;
	jump7 = *nb * mrcol;

	xjumpm_(&jump1, mult, iadd, jseed, iran1, ia1, ic1);
	xjumpm_(&jump2, mult, iadd, iran1, itmp1, ia2, ic2);
	xjumpm_(&jump3, mult, iadd, iran1, itmp1, ia3, ic3);
	xjumpm_(&jump4, ia3, ic3, iran1, itmp1, ia4, ic4);
	xjumpm_(&jump5, ia3, ic3, iran1, itmp1, ia5, ic5);
	xjumpm_(&jump6, ia5, ic5, iran1, itmp3, itmp1, itmp2);
	xjumpm_(&jump7, mult, iadd, itmp3, iran1, itmp1, itmp2);
	xjumpm_(&moff, ia4, ic4, iran1, itmp1, itmp2, itmp3);
	xjumpm_(&noff, ia2, ic2, itmp1, iran1, itmp2, itmp3);
	setran_(iran1, ia1, ic1);

	for (i__ = 1; i__ <= 2; ++i__) {
	    ib1[i__ - 1] = iran1[i__ - 1];
	    ib2[i__ - 1] = iran1[i__ - 1];
	    ib3[i__ - 1] = iran1[i__ - 1];
/* L100: */
	}

	ik = 1;
	i__1 = mend;
	for (ir = moff + 1; ir <= i__1; ++ir) {
	    ioffr = ((ir - 1) * *nprow + mrrow) * *mb;
	    i__2 = *mb;
	    for (j = 1; j <= i__2; ++j) {
		if (ik > *irnum) {
		    goto L160;
		}
		jk = 1;
		i__3 = nend;
		for (ic = noff + 1; ic <= i__3; ++ic) {
		    ioffc = ((ic - 1) * *npcol + mrcol) * *nb;
		    if (ioffc > ioffr) {
			i__4 = *nb;
			for (i__ = 1; i__ <= i__4; ++i__) {
			    if (jk > *icnum) {
				goto L130;
			    }
			    aMat[ik + jk * a_dim1] = alpha * aMat[ik + jk * a_dim1] + 1. - pdrand() * 2.;
			    ++jk;
/* L110: */
			}
		    } else {
			jk += *nb;
		    }
		    jumpit_(ia2, ic2, ib1, iran2);
		    ib1[0] = iran2[0];
		    ib1[1] = iran2[1];
/* L120: */
		}

L130:
		++ik;
		jumpit_(ia3, ic3, ib2, iran3);
		ib1[0] = iran3[0];
		ib1[1] = iran3[1];
		ib2[0] = iran3[0];
		ib2[1] = iran3[1];
/* L140: */
	    }

	    jumpit_(ia4, ic4, ib3, iran4);
	    ib1[0] = iran4[0];
	    ib1[1] = iran4[1];
	    ib2[0] = iran4[0];
	    ib2[1] = iran4[1];
	    ib3[0] = iran4[0];
	    ib3[1] = iran4[1];
/* L150: */
	}
L160:

/*     (Conjugate) Transposed matrix A will be generated. */

	;
    } else if (tran || *aform == 'C') {

	jump1 = 1;
	jump2 = nqnb;
	jump3 = *n;
	jump4 = npmb;
	jump5 = *mb;
	jump6 = mrrow;
	jump7 = *nb * mrcol;

	xjumpm_(&jump1, mult, iadd, jseed, iran1, ia1, ic1);
	xjumpm_(&jump2, mult, iadd, iran1, itmp1, ia2, ic2);
	xjumpm_(&jump3, mult, iadd, iran1, itmp1, ia3, ic3);
	xjumpm_(&jump4, ia3, ic3, iran1, itmp1, ia4, ic4);
	xjumpm_(&jump5, ia3, ic3, iran1, itmp1, ia5, ic5);
	xjumpm_(&jump6, ia5, ic5, iran1, itmp3, itmp1, itmp2);
	xjumpm_(&jump7, mult, iadd, itmp3, iran1, itmp1, itmp2);
	xjumpm_(&moff, ia4, ic4, iran1, itmp1, itmp2, itmp3);
	xjumpm_(&noff, ia2, ic2, itmp1, iran1, itmp2, itmp3);
	setran_(iran1, ia1, ic1);

	for (i__ = 1; i__ <= 2; ++i__) {
	    ib1[i__ - 1] = iran1[i__ - 1];
	    ib2[i__ - 1] = iran1[i__ - 1];
	    ib3[i__ - 1] = iran1[i__ - 1];
/* L170: */
	}

	ik = 1;
	i__1 = mend;
	for (ir = moff + 1; ir <= i__1; ++ir) {
	    ioffr = ((ir - 1) * *nprow + mrrow) * *mb;
	    i__2 = *mb;
	    for (j = 1; j <= i__2; ++j) {
		if (ik > *irnum) {
		    goto L230;
		}
		jk = 1;
		i__3 = nend;
		for (ic = noff + 1; ic <= i__3; ++ic) {
		    ioffc = ((ic - 1) * *npcol + mrcol) * *nb;
		    i__4 = *nb;
		    for (i__ = 1; i__ <= i__4; ++i__) {
			if (jk > *icnum) {
			    goto L200;
			}
			aMat[ik + jk * a_dim1] = alpha * aMat[ik + jk * a_dim1] + 1. - pdrand() * 2.;
			++jk;
/* L180: */
		    }
		    jumpit_(ia2, ic2, ib1, iran2);
		    ib1[0] = iran2[0];
		    ib1[1] = iran2[1];
/* L190: */
		}

L200:
		++ik;
		jumpit_(ia3, ic3, ib2, iran3);
		ib1[0] = iran3[0];
		ib1[1] = iran3[1];
		ib2[0] = iran3[0];
		ib2[1] = iran3[1];
/* L210: */
	    }

	    jumpit_(ia4, ic4, ib3, iran4);
	    ib1[0] = iran4[0];
	    ib1[1] = iran4[1];
	    ib2[0] = iran4[0];
	    ib2[1] = iran4[1];
	    ib3[0] = iran4[0];
	    ib3[1] = iran4[1];
/* L220: */
	}
L230:

/*     A random matrix is generated. */

	;
    } else {

	jump1 = 1;
	jump2 = npmb;
	jump3 = *m;
	jump4 = nqnb;
	jump5 = *nb;
	jump6 = mrcol;
	jump7 = *mb * mrrow;

	xjumpm_(&jump1, mult, iadd, jseed, iran1, ia1, ic1);
	xjumpm_(&jump2, mult, iadd, iran1, itmp1, ia2, ic2);
	xjumpm_(&jump3, mult, iadd, iran1, itmp1, ia3, ic3);
	xjumpm_(&jump4, ia3, ic3, iran1, itmp1, ia4, ic4);
	xjumpm_(&jump5, ia3, ic3, iran1, itmp1, ia5, ic5);
	xjumpm_(&jump6, ia5, ic5, iran1, itmp3, itmp1, itmp2);
	xjumpm_(&jump7, mult, iadd, itmp3, iran1, itmp1, itmp2);
	xjumpm_(&noff, ia4, ic4, iran1, itmp1, itmp2, itmp3);
	xjumpm_(&moff, ia2, ic2, itmp1, iran1, itmp2, itmp3);
	setran_(iran1, ia1, ic1);

	for (i__ = 1; i__ <= 2; ++i__) {
	    ib1[i__ - 1] = iran1[i__ - 1];
	    ib2[i__ - 1] = iran1[i__ - 1];
	    ib3[i__ - 1] = iran1[i__ - 1];
/* L240: */
	}

	jk = 1;
	i__1 = nend;
	for (ic = noff + 1; ic <= i__1; ++ic) {
	    ioffc = ((ic - 1) * *npcol + mrcol) * *nb;
	    i__2 = *nb;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		if (jk > *icnum) {
		    goto L300;
		}
		ik = 1;
		i__3 = mend;
		for (ir = moff + 1; ir <= i__3; ++ir) {
		    ioffr = ((ir - 1) * *nprow + mrrow) * *mb;
		    i__4 = *mb;
		    for (j = 1; j <= i__4; ++j) {
			if (ik > *irnum) {
			    goto L270;
			}
			aMat[ik + jk * a_dim1] = alpha * aMat[ik + jk * a_dim1] + 1. - pdrand() * 2.;
			++ik;
/* L250: */
		    }
		    jumpit_(ia2, ic2, ib1, iran2);
		    ib1[0] = iran2[0];
		    ib1[1] = iran2[1];
/* L260: */
		}

L270:
		++jk;
		jumpit_(ia3, ic3, ib2, iran3);
		ib1[0] = iran3[0];
		ib1[1] = iran3[1];
		ib2[0] = iran3[0];
		ib2[1] = iran3[1];
/* L280: */
	    }

	    jumpit_(ia4, ic4, ib3, iran4);
	    ib1[0] = iran4[0];
	    ib1[1] = iran4[1];
	    ib2[0] = iran4[0];
	    ib2[1] = iran4[1];
	    ib3[0] = iran4[0];
	    ib3[1] = iran4[1];
/* L290: */
	}
L300:
	;
    }

/*     Diagonally dominant matrix will be generated. */

    if (*diag == 'D') {
	if (*mb != *nb) {
            printf( "Diagonally dominant matrices with rowNB not equal colNB is not supported!" );
	    return 0;
	}

	maxmn = Mmax(*m,*n);
	jk = 1;
	i__1 = nend;
	for (ic = noff + 1; ic <= i__1; ++ic) {
	    ioffc = ((ic - 1) * *npcol + mrcol) * *nb;
	    ik = 1;
	    i__2 = mend;
	    for (ir = moff + 1; ir <= i__2; ++ir) {
		ioffr = ((ir - 1) * *nprow + mrrow) * *mb;
		if (ioffc == ioffr) {
		    i__3 = *mb - 1;
		    for (j = 0; j <= i__3; ++j) {
			if (ik > *irnum) {
			    goto L330;
			}
			aMat[ik + (jk + j) * a_dim1] = (d__1 = aMat[ik + (jk + j) * a_dim1], fabs(d__1)) + maxmn;
			++ik;
/* L310: */
		    }
		} else {
		    ik += *mb;
		}
/* L320: */
	    }
L330:
	    jk += *nb;
/* L340: */
	}
    }

    return 0;
} /* pdmatgen */
