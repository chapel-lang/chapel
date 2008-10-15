/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; -*- */

#include <hpcc.h>

#include "cblacslt.h"

/* Common Block Declarations */

struct {
    int iaz, jaz, itz, jtz;
} commtrb_;

#define commtrb_1 commtrb_

extern struct {
    int ictxt;
} context_;

#define context_1 context_

/* Table of constant values */

static int c__0 = 0;

static int
dtr2mx_(double *a, int *lda, double *beta, double *t, int *ldt, int *nrow, int *ncol, int *
	mb, int *nb, int *ilt, int *jlt) {
    /* System generated locals */
    long a_dim1, a_offset, t_dim1, t_offset;
    int i__1, i__2, i__3, i__4;

    /* Local variables */
    static int k, ia, ja, jj, ki, kj, it, jt, mr, irm, jrm;


/*  -- PUMMA Package routine (version 2.1) -- */
/*     Jaeyoung Choi, Oak Ridge National Laboratory. */
/*     Jack Dongarra, Univ. of Tennessee, Oak Ridge National Laboratory. */
/*     David Walker,  Oak Ridge National Laboratory. */
/*     October 31, 1994. */

/*  Purpose */

/*  T <== A' + beta*T (assume beta = 0.0, or 1.0) */
/*  T is a scattered 2-D array from a scattered 2-D array A */

/*     T = A' */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    t_dim1 = *ldt;
    t_offset = 1 + t_dim1;
    t -= t_offset;

    /* Function Body */
    ia = 0;
    jt = 0;

    if (*beta == 0.) {
	i__1 = *nrow - 2;
	for (ki = 0; ki <= i__1; ++ki) {
	    ja = 0;
	    it = 0;
	    i__2 = *ncol - 2;
	    for (kj = 0; kj <= i__2; ++kj) {
		i__3 = *nb;
		for (jj = 1; jj <= i__3; ++jj) {
		    i__4 = *mb;
		    for (k = 1; k <= i__4; ++k) {
			t[it + jj + (jt + k) * t_dim1] = a[ia + k + (ja + jj) * a_dim1];
/* L10: */
		    }
		}
		ja += commtrb_1.jaz;
		it += commtrb_1.itz;
/* L20: */
	    }

	    jrm = *jlt - ja;
	    if (jrm > 0) {
		i__2 = Mmin(*nb,jrm);
		for (jj = 1; jj <= i__2; ++jj) {
		    i__4 = *mb;
		    for (k = 1; k <= i__4; ++k) {
			t[it + jj + (jt + k) * t_dim1] = a[ia + k + (ja + jj) * a_dim1];
/* L30: */
		    }
		}
	    }
	    ia += commtrb_1.iaz;
	    jt += commtrb_1.jtz;
/* L40: */
	}

	irm = *ilt - ia;
	if (irm > 0) {
	    ja = 0;
	    it = 0;
	    mr = Mmin(irm,*mb);
	    i__1 = *ncol - 2;
	    for (kj = 0; kj <= i__1; ++kj) {
		i__4 = *nb;
		for (jj = 1; jj <= i__4; ++jj) {
		    i__2 = mr;
		    for (k = 1; k <= i__2; ++k) {
			t[it + jj + (jt + k) * t_dim1] = a[ia + k + (ja + jj) * a_dim1];
/* L50: */
		    }
		}
		ja += commtrb_1.jaz;
		it += commtrb_1.itz;
/* L60: */
	    }

	    jrm = *jlt - ja;
	    if (jrm > 0) {
		i__1 = Mmin(*nb,jrm);
		for (jj = 1; jj <= i__1; ++jj) {
		    i__2 = mr;
		    for (k = 1; k <= i__2; ++k) {
			t[it + jj + (jt + k) * t_dim1] = a[ia + k + (ja + jj) * a_dim1];
/* L70: */
		    }
		}
	    }
	}

    } else {

/*         T = A' + T */

	i__2 = *nrow - 2;
	for (ki = 0; ki <= i__2; ++ki) {
	    ja = 0;
	    it = 0;
	    i__1 = *ncol - 2;
	    for (kj = 0; kj <= i__1; ++kj) {
		i__4 = *nb;
		for (jj = 1; jj <= i__4; ++jj) {
		    i__3 = *mb;
		    for (k = 1; k <= i__3; ++k) {
			t[it + jj + (jt + k) * t_dim1] += a[ia + k + (ja + jj) * a_dim1];
/* L80: */
		    }
		}
		ja += commtrb_1.jaz;
		it += commtrb_1.itz;
/* L90: */
	    }

	    jrm = *jlt - ja;
	    if (jrm > 0) {
		i__1 = Mmin(*nb,jrm);
		for (jj = 1; jj <= i__1; ++jj) {
		    i__3 = *mb;
		    for (k = 1; k <= i__3; ++k) {
			t[it + jj + (jt + k) * t_dim1] += a[ia + k + (ja + jj) * a_dim1];
/* L100: */
		    }
		}
	    }
	    ia += commtrb_1.iaz;
	    jt += commtrb_1.jtz;
/* L110: */
	}

	irm = *ilt - ia;
	if (irm > 0) {
	    ja = 0;
	    it = 0;
	    mr = Mmin(irm,*mb);
	    i__2 = *ncol - 2;
	    for (kj = 0; kj <= i__2; ++kj) {
		i__3 = *nb;
		for (jj = 1; jj <= i__3; ++jj) {
		    i__1 = mr;
		    for (k = 1; k <= i__1; ++k) {
			t[it + jj + (jt + k) * t_dim1] += a[ia + k + (ja + jj) * a_dim1];
/* L120: */
		    }
		}
		ja += commtrb_1.jaz;
		it += commtrb_1.itz;
/* L130: */
	    }

	    jrm = *jlt - ja;
	    if (jrm > 0) {
		i__2 = Mmin(*nb,jrm);
		for (jj = 1; jj <= i__2; ++jj) {
		    i__1 = mr;
		    for (k = 1; k <= i__1; ++k) {
			t[it + jj + (jt + k) * t_dim1] += a[ia + k + (ja + jj) * a_dim1];
/* L140: */
		    }
		}
	    }
	}
    }

    return 0;
} /* dtr2mx_ */

static int
dtr2bf_(double *a, int *lda, double *t, int *ldt, int *nrow, int *ncol, int *mb, int *nb,
	int *ilt, int *jlt) {
    /* System generated locals */
    long a_dim1, a_offset, t_dim1, t_offset;
    int i__1, i__2, i__3, i__4;

    /* Local variables */
    static int k, ia, ja, jj, ki, kj, it, jt, mr, irm, jrm;


/*  -- PUMMA Package routine (version 2.1) -- */
/*     Jaeyoung Choi, Oak Ridge National Laboratory. */
/*     Jack Dongarra, Univ. of Tennessee, Oak Ridge National Laboratory. */
/*     David Walker,  Oak Ridge National Laboratory. */
/*     October 31, 1994. */

/*  Purpose */

/*  T <== A' */
/*  T is a condensed 2-D buffer from a scattered 2-D array A */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    t_dim1 = *ldt;
    t_offset = 1 + t_dim1;
    t -= t_offset;

    /* Function Body */
    ia = 0;
    jt = 0;
    i__1 = *nrow - 2;
    for (ki = 0; ki <= i__1; ++ki) {
	ja = 0;
	it = 0;
	i__2 = *ncol - 2;
	for (kj = 0; kj <= i__2; ++kj) {
	    i__3 = *nb;
	    for (jj = 1; jj <= i__3; ++jj) {
		i__4 = *mb;
		for (k = 1; k <= i__4; ++k) {
		    t[it + jj + (jt + k) * t_dim1] = a[ia + k + (ja + jj) * a_dim1];
/* L10: */
		}
	    }
	    ja += commtrb_1.jaz;
	    it += *nb;
/* L20: */
	}

	jrm = *jlt - ja;
	if (jrm > 0) {
	    i__2 = Mmin(*nb,jrm);
	    for (jj = 1; jj <= i__2; ++jj) {
		i__4 = *mb;
		for (k = 1; k <= i__4; ++k) {
		    t[it + jj + (jt + k) * t_dim1] = a[ia + k + (ja + jj) * a_dim1];
/* L30: */
		}
	    }
	}
	ia += commtrb_1.iaz;
	jt += *mb;
/* L40: */
    }

    irm = *ilt - ia;
    if (irm > 0) {
	ja = 0;
	it = 0;
	mr = Mmin(*mb,irm);
	i__1 = *ncol - 2;
	for (kj = 0; kj <= i__1; ++kj) {
	    i__4 = *nb;
	    for (jj = 1; jj <= i__4; ++jj) {
		i__2 = mr;
		for (k = 1; k <= i__2; ++k) {
		    t[it + jj + (jt + k) * t_dim1] = a[ia + k + (ja + jj) * a_dim1];
/* L50: */
		}
	    }
	    ja += commtrb_1.jaz;
	    it += *nb;
/* L60: */
	}

	jrm = *jlt - ja;
	if (jrm > 0) {
	    i__1 = Mmin(*nb,jrm);
	    for (jj = 1; jj <= i__1; ++jj) {
		i__2 = mr;
		for (k = 1; k <= i__2; ++k) {
		    t[it + jj + (jt + k) * t_dim1] = a[ia + k + (ja + jj) * a_dim1];
/* L70: */
		}
	    }
	}
    }

    return 0;
} /* dtr2bf_ */


static int
dmv2mx_(double *t, int *ldt, double *beta, double *a, int *lda, int *nrow, int *ncol,
  int *mb, int *nb, int *ilt, int *jlt) {
    /* System generated locals */
    long t_dim1, t_offset, a_dim1, a_offset;
    int i__1, i__2, i__3, i__4;

    /* Local variables */
    static int k, ia, ja, jj, ki, kj, it, jt, mr, irm, jrm;


/*  -- PUMMA Package routine (version 2.1) -- */
/*     Jaeyoung Choi, Oak Ridge National Laboratory. */
/*     Jack Dongarra, Univ. of Tennessee, Oak Ridge National Laboratory. */
/*     David Walker,  Oak Ridge National Laboratory. */
/*     October 31, 1994. */

/*  Purpose */

/*  A <== T + beta*A (assume beta = 0.0, or 1.0) */
/*  A is a scattered 2-D array from a condensed 2-D buffer T */

    /* Parameter adjustments */
    t_dim1 = *ldt;
    t_offset = 1 + t_dim1;
    t -= t_offset;
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    it = 0;
    ia = 0;

/*     A <== T */

    if (*beta == 0.) {

/*         If NPROW = 1, use DCOPY */

	if (*nrow == 1) {
	    jt = 0;
	    ja = 0;
	    i__1 = *ncol - 2;
	    for (kj = 0; kj <= i__1; ++kj) {
		i__2 = *nb;
		for (jj = 1; jj <= i__2; ++jj) {
		    i__3 = Mmin(*mb,*ilt);
		    HPL_dcopy(i__3, &t[(jt + jj) * t_dim1 + 1], 1, &a[(ja + jj) * a_dim1 + 1], 1);
/* L10: */
		}
		jt += *nb;
		ja += commtrb_1.jtz;
/* L20: */
	    }

	    jrm = *jlt - ja;
	    if (jrm > 0) {
		i__1 = Mmin(*nb,jrm);
		for (jj = 1; jj <= i__1; ++jj) {
		    i__2 = Mmin(*mb,*ilt);
		    HPL_dcopy(i__2, &t[(jt + jj) * t_dim1 + 1], 1, &a[(ja + jj) * a_dim1 + 1], 1);
/* L30: */
		}
	    }

	} else {

	    i__1 = *nrow - 2;
	    for (ki = 0; ki <= i__1; ++ki) {
		jt = 0;
		ja = 0;
		i__2 = *ncol - 2;
		for (kj = 0; kj <= i__2; ++kj) {
		    i__3 = *nb;
		    for (jj = 1; jj <= i__3; ++jj) {
			i__4 = *mb;
			for (k = 1; k <= i__4; ++k) {
			    a[ia + k + (ja + jj) * a_dim1] = t[it + k + (jt + jj) * t_dim1];
/* L40: */
			}
		    }
		    jt += *nb;
		    ja += commtrb_1.jtz;
/* L50: */
		}

		jrm = *jlt - ja;
		if (jrm > 0) {
		    i__2 = Mmin(*nb,jrm);
		    for (jj = 1; jj <= i__2; ++jj) {
			i__4 = *mb;
			for (k = 1; k <= i__4; ++k) {
			    a[ia + k + (ja + jj) * a_dim1] = t[it + k + (jt + jj) * t_dim1];
/* L60: */
			}
		    }
		}
		it += *mb;
		ia += commtrb_1.itz;
/* L70: */
	    }

	    irm = *ilt - ia;
	    if (irm > 0) {
		jt = 0;
		ja = 0;
		mr = Mmin(*mb,irm);
		i__1 = *ncol - 2;
		for (kj = 0; kj <= i__1; ++kj) {
		    i__4 = *nb;
		    for (jj = 1; jj <= i__4; ++jj) {
			i__2 = mr;
			for (k = 1; k <= i__2; ++k) {
			    a[ia + k + (ja + jj) * a_dim1] = t[it + k + (jt + jj) * t_dim1];
/* L80: */
			}
		    }
		    jt += *nb;
		    ja += commtrb_1.jtz;
/* L90: */
		}

		jrm = *jlt - ja;
		if (jrm > 0) {
		    i__1 = Mmin(*nb,jrm);
		    for (jj = 1; jj <= i__1; ++jj) {
			i__2 = mr;
			for (k = 1; k <= i__2; ++k) {
			    a[ia + k + (ja + jj) * a_dim1] = t[it + k + (jt + jj) * t_dim1];
/* L100: */
			}
		    }
		}
	    }
	}

/*         A <== T + A */

    } else {

/*         If NPROW = 1, use DAXPY */

	if (*nrow == 1) {
	    jt = 0;
	    ja = 0;
	    i__2 = *ncol - 2;
	    for (kj = 0; kj <= i__2; ++kj) {
		i__1 = *nb;
		for (jj = 1; jj <= i__1; ++jj) {
		    i__4 = Mmin(*mb,*ilt);
		    HPL_daxpy(i__4, 1.0, &t[(jt + jj) * t_dim1 + 1], 1, &a[(ja + jj) * a_dim1 + 1], 1);
/* L110: */
		}
		jt += *nb;
		ja += commtrb_1.jtz;
/* L120: */
	    }

	    jrm = *jlt - ja;
	    if (jrm > 0) {
		i__2 = Mmin(*nb,jrm);
		for (jj = 1; jj <= i__2; ++jj) {
		    i__1 = Mmin(*mb,*ilt);
		    HPL_daxpy(i__1, 1.0, &t[(jt + jj) * t_dim1 + 1], 1, & a[(ja + jj) * a_dim1 + 1], 1);
/* L130: */
		}
	    }

	} else {
	    i__2 = *nrow - 2;
	    for (ki = 0; ki <= i__2; ++ki) {
		jt = 0;
		ja = 0;
		i__1 = *ncol - 2;
		for (kj = 0; kj <= i__1; ++kj) {
		    i__4 = *nb;
		    for (jj = 1; jj <= i__4; ++jj) {
			i__3 = *mb;
			for (k = 1; k <= i__3; ++k) {
			    a[ia + k + (ja + jj) * a_dim1] += t[it + k + (jt + jj) * t_dim1];
/* L140: */
			}
		    }
		    jt += *nb;
		    ja += commtrb_1.jtz;
/* L150: */
		}

		jrm = *jlt - ja;
		if (jrm > 0) {
		    i__1 = Mmin(*nb,jrm);
		    for (jj = 1; jj <= i__1; ++jj) {
			i__3 = *mb;
			for (k = 1; k <= i__3; ++k) {
			    a[ia + k + (ja + jj) * a_dim1] += t[it + k + (jt + jj) * t_dim1];
/* L160: */
			}
		    }
		}
		it += *mb;
		ia += commtrb_1.itz;
/* L170: */
	    }

	    irm = *ilt - ia;
	    if (irm > 0) {
		jt = 0;
		ja = 0;
		mr = Mmin(*mb,irm);
		i__2 = *ncol - 2;
		for (kj = 0; kj <= i__2; ++kj) {
		    i__3 = *nb;
		    for (jj = 1; jj <= i__3; ++jj) {
			i__1 = mr;
			for (k = 1; k <= i__1; ++k) {
			    a[ia + k + (ja + jj) * a_dim1] += t[it + k + (jt + jj) * t_dim1];
/* L180: */
			}
		    }
		    jt += *nb;
		    ja += commtrb_1.jtz;
/* L190: */
		}

		jrm = *jlt - ja;
		if (jrm > 0) {
		    i__2 = Mmin(*nb,jrm);
		    for (jj = 1; jj <= i__2; ++jj) {
			i__1 = mr;
			for (k = 1; k <= i__1; ++k) {
			    a[ia + k + (ja + jj) * a_dim1] += t[it + k + (jt + jj) * t_dim1];
/* L200: */
			}
		    }
		}
	    }
	}
    }

    return 0;
} /* dmv2mx_ */

int
pdtrans(char *trans, int *m, int *n, int * mb, int *nb, double *a, int *lda, double *beta,
	double *c__, int *ldc, int *imrow, int *imcol, double *work, int *iwork) {
    /* System generated locals */
    long a_dim1, a_offset, c_dim1, c_offset;
    int i__1, i__2, i__3, i__4;

    /* Local variables */
    int j1, k1, k2, ml, nl, mp, mq, np, nq, mb0, mb1, mb2, nb0,
	    nb1, nb2, kia, kja, kic, kjc, lbm, lbn, lcm, ldt, lbm0, lbm1,
	     lbm2, lbn0, lbn1, lbn2, igcd;
    long ipt;
    int mcol, info, lcmp, lcmq, item, ncol, kmod1, kmod2;
    double tbeta;
    int kpcol, mpcol, npcol, mrcol, mycol, kprow, mprow, nprow, mrrow, myrow;

/*  -- PUMMA Package routine (version 2.1) -- */
/*     Jaeyoung Choi, Oak Ridge National Laboratory. */
/*     Jack Dongarra, Univ. of Tennessee, Oak Ridge National Laboratory. */
/*     David Walker,  Oak Ridge National Laboratory. */
/*     October 31, 1994. */

/*  Purpose */

/*  PDTRANS routine is one of the PUMMA package based on block cyclic */
/*  data distribution on 2-D process configuration. */

/*  It is used for the following matrix transposition, */

/*     Form  C := A' + beta*C */

/*  where beta is a scalar, and A and C are matrices, with A an M by N */
/*  matrix (globally), and C an N by M matrix (globally). */

/*  Parameters */

/*  TRANS  - (input) CHARACTER*1 */
/*           TRANS specifies whether A is transposed or conjugate */
/*           transposed. */

/*              TRANS = 'T',   transpose; */

/*              TRANS = 'C',   conjugate transpose. */

/*  M      - (input) INTEGER */
/*           M specifies the (global) number of rows of the matrix A and */
/*           the (global) number of rows of the matrix C.  M >= 0. */

/*  N      - (input) INTEGER */
/*           N specifies the (global) number of columns of the matrix A */
/*           and columns of the matrix B.  N >= 0. */

/*  MB     - (input) INTEGER */
/*           MB specifies the row block size of the matrix A and the */
/*           column block of the matrix C.  MB >= 1. */

/*  NB     - (input) INTEGER */
/*           NB specifies the column block size of the matrix A and the */
/*           row block size of the matrix C.  NB >= 1. */

/*  A      - (input) DOUBLE PRECISION array of DIMENSION ( LDA, Nq ). */
/*           The leading Mp by Nq part of the array A must contain the */
/*           local matrix  A.  Mp and Nq are local variables */
/*           (see description of local parameters). */

/*  LDA    - (input) INTEGER */
/*           The leading dimension of the (local) array A. */
/*           LDA >= MAX( 1, Mp ). */

/*  BETA   - (input) DOUBLE PRECISION */
/*           BETA  specifies the scalar beta.  When BETA is supplied as */
/*           zero then C need not be set on input. */

/*  C      - (input/ouput) DOUBLE PRECISION array of DIMENSION (LDC, Mq). */
/*           On entry the leading Np by Mq part of the array C must */
/*           contain the local matrix C, except when beta is zero, */
/*           in which case C need not be set on entry. */
/*           On exit, the array C is overwritten by the Np by Mq matrix */
/*           (A'+bata*C).  Np and Mq are local variables */
/*           (see description of local parameters). */

/*  LDC    - (input) INTEGER */
/*           The leading dimension of the (local) array C. */
/*           LDC >= MAX( 1, Np ). */

/*  IMROW  - (input) INTEGER */
/*           IMROW specifies a row of the process template, which holds */
/*           the first block of the matrices.  0 <= IMROW < NPROW. */

/*  IMCOL  - (input) INTEGER */
/*           IMCOL specifies a column of the process template, which */
/*           holds the first block of the matrices.  0 <= IMCOL < NPCOL. */

/*  WORK   - (workspace) DOUBLE PRECISION array */
/*           See requirements. */

/*  IWORK  - (workspace) INTEGER array */
/*           See requirements. */

/*  Local  Parameters */

/*  LCM   =  the lowest common multiple of P and Q */
/*  LCMP  =  LCM/P = number of template rows in LCM block */
/*  LCMQ  =  LCM/Q = number of template columns in LCM block */
/*  IGCD   =  the greatest common divisor (GCD) of P and Q */
/*  MpxNq =  size of (local) matrix A in the process, iam */
/*  NpxMq =  size of (local) matrix C in the process, iam */
/*  KMOD  =  Define Group I.D. */
/*  item  =  temporal integer parameter */

/*    Two buffers for storing A' and T(= subblock of A') */
/*       WORK       <== A' */
/*       WORK(IPT)  <== T */

/*    Three interger buffers */
/*       IWORK(1,k) <== starting point of row subblock of A  to send and */
/*                      C to receive in K2 loop (rowwise communication) */
/*       IWORK(2,k) <== starting point of column subblock of A to send in */
/*                      J1 loop (columnwise communication) */
/*       IWORK(3,k) <== starting point of column subblock of C to receive */
/*                      in J1 loop (columnwise communication) */

/*  Requirements (approximate) */

/*   Size(IWORK) = 3 x MAX(P, Q) */
/*   Size(WORK)  = 2 x Ceil(Ceil(M,MB),LCM)xMB x Ceil(Ceil(N,NB),LCM)xNB */

/*     Get grid parameters */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    c_dim1 = *ldc;
    c_offset = 1 + c_dim1;
    c__ -= c_offset;
    --work;
    --iwork;

    /* Function Body */
    Cblacs_gridinfo(context_1.ictxt, &nprow, &npcol, &myrow, &mycol);

/*     Test for the input parameters. */

    info = 0;
    if (*trans != 'T' && *trans != 'C') {
	info = 1;
    } else if (*m < 0) {
	info = 2;
    } else if (*n < 0) {
	info = 3;
    } else if (*mb < 1) {
	info = 4;
    } else if (*nb < 1) {
	info = 5;
    } else if (*lda < 1) {
	info = 7;
    } else if (*ldc < 1) {
	info = 10;
    } else if (*imrow < 0 || *imrow >= nprow) {
	info = 11;
    } else if (*imcol < 0 || *imcol >= npcol) {
	info = 12;
    }

L10:
    if (info != 0) {
	pxerbla( &context_1.ictxt, "PDTRANS", &info );
	return 0;
    }

/*     Initialize parameters */

    mprow = nprow + myrow;
    mpcol = npcol + mycol;
    mrrow = (mprow - *imrow) % nprow;
    mrcol = (mpcol - *imcol) % npcol;

    lcm = ilcm_(&nprow, &npcol);
    lcmp = lcm / nprow;
    lcmq = lcm / npcol;
    igcd = nprow / lcmq;

    mp = numroc_(m, mb, &mrrow, &c__0, &nprow);
    mq = numroc_(m, mb, &mrcol, &c__0, &npcol);
    np = numroc_(n, nb, &mrrow, &c__0, &nprow);
    nq = numroc_(n, nb, &mrcol, &c__0, &npcol);

    i__1 = iceil_(m, mb);
    lbm = iceil_(&i__1, &lcm);
    i__1 = iceil_(n, nb);
    lbn = iceil_(&i__1, &lcm);

/*     Test for the input parameters again with local parameters */

    if (*lda < mp) {
	info = 7;
    } else if (*ldc < np) {
	info = 10;
    }
    if (info != 0) {
	goto L10;
    }

/*     Quick return if possible. */

    if (*m == 0 || *n == 0) {
	return 0;
    }

/*     At first, scale C with beta if beta != 0.0 & beta != 1.0 */

    tbeta = *beta;
    if (*beta != 0. && *beta != 1.) {
	i__1 = mq;
	for (j1 = 1; j1 <= i__1; ++j1) {
	    HPL_dscal( np, *beta, &c__[j1 * c_dim1 + 1], 1 );
/* L20: */
	}
	tbeta = 1.;
    }

    commtrb_1.iaz = lcmp * *mb;
    commtrb_1.jaz = lcmq * *nb;
    commtrb_1.itz = lcmp * *nb;
    commtrb_1.jtz = lcmq * *mb;

    ml = lbm * *mb;
    nl = lbn * *nb;
    ipt = (long)ml * (long)nl + 1;
    ldt = nl;
    kprow = mrrow + nprow;
    kpcol = mrcol + npcol;

/*     Initialize Parameters -- Compute the positions of subblocks */

    i__1 = npcol - 1;
    for (k1 = 0; k1 <= i__1; ++k1) {
	ncol = (kpcol - k1) % npcol;
	i__2 = lcmq - 1;
	for (j1 = 0; j1 <= i__2; ++j1) {
	    item = npcol * j1 + ncol;
	    if (item % nprow == mrrow) {
		iwork[ncol * 3 + 1] = item / nprow;
	    }
/* L30: */
	}
    }

    i__2 = lcmq - 1;
    for (j1 = 0; j1 <= i__2; ++j1) {
	item = (npcol * j1 + mrcol) % nprow;
	iwork[item * 3 + 2] = j1;
	iwork[item * 3 + 3] = j1;
	i__1 = igcd - 1;
	for (k1 = 1; k1 <= i__1; ++k1) {
	    iwork[(item + nprow - k1) % nprow * 3 + 2] = j1;
	    iwork[(item + k1) % nprow * 3 + 3] = j1;
/* L40: */
	}
    }

/*     Set parameters for efficient copying */

    lbm0 = lbm;
    lbm1 = lbm;
    lbm2 = lbm;
    lbn0 = lbn;
    lbn1 = lbn;
    lbn2 = lbn;
    mb0 = *mb;
    mb1 = *mb;
    mb2 = *mb;
    nb0 = *nb;
    nb1 = *nb;
    nb2 = *nb;

    if (nprow == npcol) {
	lbm0 = 1;
	lbn0 = 1;
	mb0 = mp;
	nb0 = nq;
    }
    if (nprow == lcm) {
	lbm1 = 1;
	lbn2 = 1;
	mb1 = mp;
	nb2 = np;
    }
    if (npcol == lcm) {
	lbn1 = 1;
	lbm2 = 1;
	nb1 = nq;
	mb2 = mq;
    }

/*     For each K2 loop (rowwise), Copy A' to WORK & Send it to KTPROC */
/*                                 then, Receive WORK and Copy WORK to C */

    kmod1 = (nprow + mrcol - mrrow) % igcd;
    kmod2 = (igcd - kmod1) % igcd;

    i__1 = lcmp - 1;
    for (k2 = 0; k2 <= i__1; ++k2) {

/*        Copy A' to WORK in the appropriate order & Send it */

	k1 = k2 * igcd + kmod1;
	mcol = (kpcol - k1) % npcol;
	kia = iwork[mcol * 3 + 1] * *mb;
	mcol = (mcol + *imcol) % npcol;
	ncol = (mrcol + k2 * igcd + kmod2) % npcol;
	kic = iwork[ncol * 3 + 1] * *nb;
	ncol = (ncol + *imcol) % npcol;

	i__2 = lcmq - 1;
	for (j1 = 0; j1 <= i__2; ++j1) {
	    kja = iwork[(mrrow + igcd * j1) % nprow * 3 + 2] * *nb;

	    if (myrow == (myrow + igcd * j1 + kmod1) % nprow && mycol == mcol)
		     {
		kjc = iwork[(kprow - igcd * j1) % nprow * 3 + 3] * *mb;
		i__3 = mp - kia;
		i__4 = nq - kja;
		dtr2mx_(&a[kia + 1 + (kja + 1) * a_dim1], lda, &tbeta, &c__[
			kic + 1 + (kjc + 1) * c_dim1], ldc, &lbm0, &lbn0, &
			mb0, &nb0, &i__3, &i__4);

	    } else {
		i__3 = mp - kia;
		i__4 = nq - kja;
		dtr2bf_(&a[kia + 1 + (kja + 1) * a_dim1], lda, &work[1], &ldt,
			 &lbm1, &lbn1, &mb1, &nb1, &i__3, &i__4);

		if (nprow == npcol && *beta == 0. && *ldc == ldt) {
		    i__3 = (myrow + igcd * j1 + kmod1) % nprow;
		    i__4 = (mprow - igcd * j1 - kmod2) % nprow;
		    kjc = iwork[(kprow - igcd * j1) % nprow * 3 + 3] * *mb;
#if 0
		    Cdgesd2d(context_1.ictxt,nl,ml,&work[1],nl,i__3,mcol);
		    Cdgerv2d(context_1.ictxt,nl,ml,&c__[(kjc + 1) * c_dim1 + 1],*ldc,i__4,ncol);
#else
		    Cblacs_dSendrecv( context_1.ictxt,
                          nl, ml, &work[1], nl, i__3, mcol,
                          nl, ml, &c__[(kjc + 1) * c_dim1 + 1], *ldc, i__4, ncol );
#endif

		} else {
		    i__3 = (myrow + igcd * j1 + kmod1) % nprow;
		    i__4 = (mprow - igcd * j1 - kmod2) % nprow;
#if 0
		    Cdgesd2d(context_1.ictxt,nl,ml,&work[1],nl,i__3,mcol);
		    Cdgerv2d(context_1.ictxt,nl,ml,&work[ipt],nl, i__4,ncol);
#else
        Cblacs_dSendrecv( context_1.ictxt,
                          nl, ml, &work[1],   nl, i__3, mcol,
                          nl, ml, &work[ipt], nl, i__4, ncol );
#endif

		    kjc = iwork[(kprow - igcd * j1) % nprow * 3 + 3] * *mb;
		    i__3 = np - kic;
		    i__4 = mq - kjc;
		    dmv2mx_(&work[ipt], &ldt, &tbeta, &c__[kic + 1 + (kjc + 1)
			     * c_dim1], ldc, &lbn2, &lbm2, &nb2, &mb2, &i__3,
			    &i__4);
		}
	    }
	}
    }

    return 0;
} /* pdtrans_ */
