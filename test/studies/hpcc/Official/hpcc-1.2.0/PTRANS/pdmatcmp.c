/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; -*- */
/*
pdmatcmp.c
*/

#include <hpcc.h>

#include "cblacslt.h"

/*
Purpose

PDMATCMP : Parallel Real Double precision MATrix CoMPare.
           Finds ERROR = MAX(||A(i,j) - ACOPY(i,j)||) / MAX(|A(i,j)|)
           locally.  Then, returns the global maximum from the local
           values.

Arguments

M         (input) INTEGER
          Number of rows of the local matrices A and B. M  >= 0.

N         (input) INTEGER
          Number of columns of the local matrices A and B. N >= 0.

A         (input/output) DOUBLE PRECISION, dimension ( LDA, N )
          The pointer to the local matrix A.

LDA       (input) INTEGER
          Leading Dimension of A.  LDA >= M

ACOPY     (output) DOUBLE PRECISION, dimension ( LDB, N )
          The pointer to the local matrix ACOPY.

LDB       (input) INTEGER
          Leading Dimension of B.  LDB >= M.

ERROR     (output) DOUBLE PRECISION
          ERROR = MAX(|A(i,j) - ACOPY(i,j)|) / MAX(|A(i,j)|) (unscaled residual)
*/
int
pdmatcmp(int *ictxt, int *m_, int *n_, double *a, int *lda_, double *aCopy, int *ldc_,
  double *error) {
  int ctxt = *ictxt, m = *m_, n = *n_;
  long lda = *lda_, ldc = *ldc_;
  int j, info, ix; double v, aMax, vals[2];

  info = 0;
  aMax = *error = 0.0;

  if      (m < 0) info = 1;
  else if (n < 0) info = 2;
  else if (lda < m) info = 4;
  else if (ldc < m) info = 6;
  if (info != 0) {
    pxerbla( ictxt, "pdmatcmp", &info );
    return 0;
  }

  for (j = 0; j < n; j++) {
    /* the largest absolute value in column */
    ix = HPL_idamax( m, a + j * lda, 1 );
    v = fabs( a[ix + j * lda] );
    if (aMax < v) aMax = v;

    /* difference of two columns */
    HPL_daxpy( m, -1.0, a + j * lda, 1, aCopy + j * ldc, 1 );

    /* the largest absolute value in column */
    ix = HPL_idamax( m, aCopy + j * ldc, 1 );
    v = fabs( aCopy[ix + j * ldc] );
    if (*error < v) *error = v;
  }

  /* calculate max of error and max-of-a over all processes */
  vals[0] = *error;
  vals[1] = aMax;
  Cdgamx2d( ctxt, "All", " ", 2,1, vals, 2, &j, &j, -1, -1, -1 );
  *error = vals[0];
  aMax  = vals[1];

  *error /= aMax; /* calculate unscaled residual */

  return 0;
}
