/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; fill-column: 79; coding: iso-latin-1-unix -*- */
/*
C
C     FFTE: A FAST FOURIER TRANSFORM PACKAGE
C
C     (C) COPYRIGHT SOFTWARE, 2000-2004, ALL RIGHTS RESERVED
C                BY
C         DAISUKE TAKAHASHI
C         GRADUATE SCHOOL OF SYSTEMS AND INFORMATION ENGINEERING
C         UNIVERSITY OF TSUKUBA
C         1-1-1 TENNODAI, TSUKUBA, IBARAKI 305-8573, JAPAN
C         E-MAIL: daisuke@cs.tsukuba.ac.jp
C
C
C     1-D COMPLEX FFT ROUTINE
C
C     FORTRAN77 SOURCE PROGRAM
C
C     CALL ZFFT1D(A,N,IOPT,B)
C
C     A(N) IS COMPLEX INPUT/OUTPUT VECTOR (COMPLEX*16)
C     B(N) IS WORK VECTOR (COMPLEX*16)
C     N IS THE LENGTH OF THE TRANSFORMS (INTEGER*4)
C       -----------------------------------
C         N = (2**IP) * (3**IQ) * (5**IR)
C       -----------------------------------
C     IOPT = 0 FOR INITIALIZING THE COEFFICIENTS (INTEGER*4)
C          = -1 FOR FORWARD TRANSFORM
C          = +1 FOR INVERSE TRANSFORM
C
C     WRITTEN BY DAISUKE TAKAHASHI
C
*/

#include "hpccfft.h"

#ifdef _OPENMP
#include <omp.h>
#endif

int
HPCC_ipow(int x, int p) {
  int i, r;

  if (1 == x || 0 == x) return x;
  if (0 == p) return 1;
  if (-1 == x) return (p & 1) ? -1 : 1;
  if (p < 0) return 0;
  r = 1;
  for (i = 0; i < p; i++) r *= x;
  return r;
}

static int
zfft1d0(fftw_complex *a1, fftw_complex *a2, fftw_complex *b, fftw_complex *c, fftw_complex *d,
  fftw_complex *w1, fftw_complex *w2, fftw_complex *ww1, fftw_complex *ww2, fftw_complex *ww3,
  fftw_complex *ww4, int n1, int n2, int m1, int m2, int *ip1, int *ip2) {

  int lda1, lda2, ldb, ldc, ldww1, ldww2, ldww3, ldww4;
  int ii, ij, ij0, ik, ir, is, jj, i, j;
  int tmin1, tmin2, itmp1;
  fftw_complex ztmp1, ztmp2, ztmp3, ztmp4;

  lda1 = n1;
  lda2 = n2;
  ldb = n1;
  ldc = n2 + FFTE_NP;
  ldww1 = m1;
  ldww2 = m1;
  ldww3 = m2;
  ldww4 = n1/m1;

#ifdef _OPENMP
#pragma omp for private(ij,ij0,ir,jj,i,j,ik,is,ztmp1,ztmp2,ztmp3,ztmp4,tmin1,tmin2,itmp1)
#endif
  for (ii = 0; ii < n1; ii += FFTE_NBLK) {
    for (jj = 0; jj < n2; jj += FFTE_NBLK) {
      tmin1 = ii + FFTE_NBLK;
      V2MIN( tmin1, n1 );

      for (i = ii; i < tmin1; ++i) {
        tmin2 = jj + FFTE_NBLK;
        V2MIN( tmin2, n2 );

        for (j = jj; j < tmin2; ++j) {
          c_assgn( ARR2D(c, j, i-ii, ldc), ARR2D(a1, i, j, lda1) );
        }
      }
    }

    tmin1 = ii + FFTE_NBLK;
    V2MIN( tmin1, n1 );
    for (i = ii; i < tmin1; ++i)
      HPCC_fft235( PTR2D(c, 0, i-ii, ldc), d, w2, n2, ip2 );

    if (HPCC_ipow( 2, ip1[0] ) < FFTE_NBLK || HPCC_ipow( 2, ip2[0] ) < FFTE_NBLK) {
      itmp1 = n2 / m2;
      for (is = 0; is < itmp1; ++is) {
        for (ik = 0; ik < m2; ++ik) {
          j = ik + is * m2;

          tmin1 = ii + FFTE_NBLK;
          V2MIN( tmin1, n1 );
          for (i = ii; i < tmin1; ++i) {
            ir = i / m1;
            ij = i % m1;

            c_assgn(ztmp1, ARR2D(c, j, i-ii, ldc));

            c_assgn(ztmp2, ARR2D(ww1, ij, ik, ldww1));
            c_mul3v(ztmp3, ztmp1, ztmp2);

            c_assgn(ztmp2, ARR2D(ww2, ij, is, ldww2));
            c_mul3v(ztmp1, ztmp3, ztmp2);

            c_assgn(ztmp3, ARR2D(ww3, ik, ir, ldww3));
            c_mul3v(ztmp2, ztmp1, ztmp3);

            c_assgn(ztmp1, ARR2D(ww4, ir, is, ldww4));
            c_mul3v(ztmp3, ztmp2, ztmp1);

            c_assgn(ARR2D(b, i, j, ldb), ztmp3);
          }
        }
      }
    } else {
      ir = ii / m1;
      ij0 = ii % m1;

      itmp1 = n2 / m2;
      for (is = 0; is < itmp1; ++is) {
        for (ik = 0; ik < m2; ++ik) {
          c_assgn(ztmp1, ARR2D(ww3, ik, ir, ldww3));
          c_assgn(ztmp2, ARR2D(ww4, ir, is, ldww4));
          c_mul3v(ztmp4, ztmp1, ztmp2);
          j = ik + is * m2;
          ij = ij0;

          tmin1 = ii + FFTE_NBLK;
          V2MIN( tmin1, n1 );
          for (i = ii; i < tmin1; ++i) {
            c_assgn(ztmp1, ARR2D(ww1, ij, ik, ldww1));
            c_assgn(ztmp2, ARR2D(ww2, ij, is, ldww2));
            c_mul3v(ztmp3, ztmp1, ztmp2);

            c_mul3v(ztmp1, ztmp3, ztmp4);

            c_assgn(ztmp2, ARR2D(c, j, i-ii, ldc));
            c_mul3v(ztmp3, ztmp2, ztmp1);

            c_assgn(ARR2D(b, i, j, ldb), ztmp3);

            ++ij;
          }
        }
      }
    }
  }

#ifdef _OPENMP
#pragma omp for private(i,j,tmin1)
#endif
  for (jj = 0; jj < n2; jj += FFTE_NBLK) {
    tmin1 = jj + FFTE_NBLK;
    V2MIN(tmin1, n2);
    for (j = jj; j < tmin1; ++j) {
      HPCC_fft235( PTR2D(b, 0, j, ldb), c, w1, n1, ip1 );
    }

    for (i = 0; i < n1; ++i)
      for (j = jj; j < tmin1; ++j) {
        c_assgn(ARR2D(a2, j, i, lda2), ARR2D(b, i, j, ldb));
      }
  }

  return 0;
}

static int
settbls(fftw_complex *w1, fftw_complex *w2, fftw_complex *w3, fftw_complex *w4,
  int n1, int n2, int m1, int m2) {

  int j, k, is, ir;
  int ldw1, ldw2, ldw3, ldw4;
  double pi2, px;

  pi2 = 8.0 * atan(1.0);
  px = -pi2 / n1 / n2;

  ldw1 = m1;
  ldw2 = m1;
  ldw3 = m2;
  ldw4 = n1/m1;

#ifdef _OPENMP
#pragma omp parallel
  {
#pragma omp for private(j, ir)
#endif
  for (k = 0; k < m2; ++k) {
    for (j = 0; j < m1; ++j) {
      c_re(ARR2D(w1, j, k, ldw1)) = cos(px * j * k);
      c_im(ARR2D(w1, j, k, ldw1)) = sin(px * j * k);
    }

    for (ir = 0; ir < n1/m1; ++ir) {
      c_re(ARR2D(w3, k, ir, ldw3)) = cos(px * k * ir * m1);
      c_im(ARR2D(w3, k, ir, ldw3)) = sin(px * k * ir * m1);
    }
  }

#ifdef _OPENMP
#pragma omp for private(j, ir)
#endif
  for (is = 0; is < n2/m2; ++is) {
    for (j = 0; j < m1; ++j) {
      c_re(ARR2D(w2, j, is, ldw2)) = cos(px * j * is * m2);
      c_im(ARR2D(w2, j, is, ldw2)) = sin(px * j * is * m2);
    }

    for (ir = 0; ir < n1/m1; ++ir) {
      c_re(ARR2D(w4, ir, is, ldw4)) = cos(px * ir * m1 * is * m2);
      c_im(ARR2D(w4, ir, is, ldw4)) = sin(px * ir * m1 * is * m2);
    }
  }
#ifdef _OPENMP
  }
#endif

  return 0;
}	/* settbls */

int
HPCC_zfft1d(int n, fftw_complex *a, fftw_complex *b, int iopt, hpcc_fftw_plan p) {
  int i;
  int m1, m2, n1, n2, nd, nw2, nw3, nw4;
  double dn;
  int ip[3], ip1[3], ip2[3];
  fftw_complex *w1, *w2, *ww, *c;

  w1 = p->w1;
  w2 = p->w2;
  ww = p->ww;
  c = p->c;

  HPCC_factor235( n, ip );

  if (1 == iopt)
    for (i = 0; i < n; ++i) {
      c_im( a[i] ) = -c_im( a[i] );
    }

  if (n <= FFTE_L2SIZE / 16 / 3 && n <= FFTE_NDA2) {
    if (0 == iopt) {
      HPCC_settbl( w1, n );
      return 0;
    }

    HPCC_fft235( a, b, w1, n, ip );
  } else {
    for (i = 0; i < 3; ++i) {
      ip1[i] = (ip[i] + 1) / 2;
      ip2[i] = ip[i] - ip1[i];
    }

    n1 = HPCC_ipow( 2, ip1[0] ) * HPCC_ipow( 3, ip1[1] ) * HPCC_ipow( 5, ip1[2] );
    n2 = HPCC_ipow( 2, ip2[0] ) * HPCC_ipow( 3, ip2[1] ) * HPCC_ipow( 5, ip2[2] );

    if (HPCC_ipow( 2, ip1[0] ) < FFTE_NBLK || HPCC_ipow( 2, ip2[0] ) < FFTE_NBLK) {
      m1 = HPCC_ipow( 2, ip1[0] / 2 ) * HPCC_ipow( 3, ip1[1] / 2 ) * HPCC_ipow( 5, ip1[2] / 2 );
      V2MIN( m1, n1 );

      m2 = HPCC_ipow( 2, ip2[0] / 2 ) * HPCC_ipow( 3, ip2[1] / 2 ) * HPCC_ipow( 5, ip2[2] / 2 );
      V2MIN( m2, n2 );
    } else {
      m1 = HPCC_ipow( 2, ip1[0] / 2);
      m1 = FFTE_NBLK > m1 ? FFTE_NBLK : m1;
      V2MIN( m1, n1 );

      m2 = HPCC_ipow( 2, ip2[0] / 2);
      m2 = FFTE_NBLK > m2 ? FFTE_NBLK : m2;
      V2MIN( m2, n2 );
    }
    nw2 = m1 * m2 + FFTE_NP;
    nw3 = nw2 + m1 * (n2 / m2) + FFTE_NP;
    nw4 = nw3 + m2 * (n1 / m1) + FFTE_NP;

    if (0 == iopt) {
      HPCC_settbl( w1, n1 );
      HPCC_settbl( w2, n2 );
      settbls( ww, ww + nw2, ww + nw3, ww + nw4, n1, n2, m1, m2 );
      return 0;
    }

    nd = (n2 + FFTE_NP) * FFTE_NBLK + FFTE_NP;

#ifdef _OPENMP
#pragma omp parallel private(c,i)
   {
    i = omp_get_thread_num();
    c = p->c + i*p->c_size;
#endif

    zfft1d0( a, a, b, c, c + nd, w1, w2, ww, ww + nw2, ww + nw3, ww + nw4, n1, n2, m1, m2, ip1, ip2 );

#ifdef _OPENMP
   }
#endif

  }

  if (1 == iopt) {
    dn = 1.0 / (double)n;
    for (i = 0; i < n; ++i) {
      c_re( a[i] ) *= dn;
      c_im( a[i] ) *= -dn;
    }
  }

  return 0;
}	/* HPCC_zfft1d */
