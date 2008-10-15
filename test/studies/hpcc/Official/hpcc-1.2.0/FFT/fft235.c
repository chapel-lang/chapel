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
C     RADIX-2, 3, 4, 5 AND 8 FFT ROUTINE
C
C     FORTRAN77 SOURCE PROGRAM
C
C     WRITTEN BY DAISUKE TAKAHASHI
C
*/

#include "hpccfft.h"

static void
fft2(fftw_complex *a, fftw_complex *b, int m) {
  int i, lda, ldb;
  double x0, x1, y0, y1;

  lda = m;
  ldb = m;

  for (i = 0; i < m; ++i) {
    x0 = c_re( ARR2D( a, i, 0, lda ) );
    y0 = c_im( ARR2D( a, i, 0, lda ) );
    x1 = c_re( ARR2D( a, i, 1, lda ) );
    y1 = c_im( ARR2D( a, i, 1, lda ) );
    c_re( ARR2D( b, i, 0, ldb ) ) = x0 + x1;
    c_im( ARR2D( b, i, 0, ldb ) ) = y0 + y1;
    c_re( ARR2D( b, i, 1, ldb ) ) = x0 - x1;
    c_im( ARR2D( b, i, 1, ldb ) ) = y0 - y1;
  }
}

static void
fft4a(fftw_complex *a, fftw_complex *b, fftw_complex *w, int l) {
  int j, lda, ldb;
  double wr1, wr2, wr3, wi1, wi2, wi3;
  double x0, x1, x2, x3, y0, y1, y2, y3;

  lda = l;
  ldb = 4;

  for (j = 0; j < l; ++j) {
    wr1 = c_re( w[j] );
    wi1 = c_im( w[j] );
    wr2 = wr1*wr1 - wi1*wi1;
    wi2 = wr1*wi1 + wr1*wi1;
    wr3 = wr1*wr2 - wi1*wi2;
    wi3 = wr1*wi2 + wi1*wr2;

    x0 = c_re( ARR2D( a, j, 0, lda ) ) + c_re( ARR2D( a, j, 2, lda ) );
    y0 = c_im( ARR2D( a, j, 0, lda ) ) + c_im( ARR2D( a, j, 2, lda ) );
    x1 = c_re( ARR2D( a, j, 0, lda ) ) - c_re( ARR2D( a, j, 2, lda ) );
    y1 = c_im( ARR2D( a, j, 0, lda ) ) - c_im( ARR2D( a, j, 2, lda ) );

    x2 = c_re( ARR2D( a, j, 1, lda ) ) + c_re( ARR2D( a, j, 3, lda ) );
    y2 = c_im( ARR2D( a, j, 1, lda ) ) + c_im( ARR2D( a, j, 3, lda ) );
    x3 = c_im( ARR2D( a, j, 1, lda ) ) - c_im( ARR2D( a, j, 3, lda ) );
    y3 = c_re( ARR2D( a, j, 3, lda ) ) - c_re( ARR2D( a, j, 1, lda ) );

    c_re( ARR2D( b, 0, j, ldb ) ) = x0 + x2;
    c_im( ARR2D( b, 0, j, ldb ) ) = y0 + y2;
    c_re( ARR2D( b, 2, j, ldb ) ) = wr2 * (x0-x2) - wi2 * (y0-y2);
    c_im( ARR2D( b, 2, j, ldb ) ) = wr2 * (y0-y2) + wi2 * (x0-x2);
    c_re( ARR2D( b, 1, j, ldb ) ) = wr1 * (x1+x3) - wi1 * (y1+y3);
    c_im( ARR2D( b, 1, j, ldb ) ) = wr1 * (y1+y3) + wi1 * (x1+x3);
    c_re( ARR2D( b, 3, j, ldb ) ) = wr3 * (x1-x3) - wi3 * (y1-y3);
    c_im( ARR2D( b, 3, j, ldb ) ) = wr3 * (y1-y3) + wi3 * (x1-x3);
  }
}

static void
fft4b(fftw_complex *a, fftw_complex *b, fftw_complex *w, int m, int l) {
  int i, j, lda1, lda2, ldb1, ldb2;
  double x0, x1, x2, x3, y0, y1, y2, y3;
  double wr1, wr2, wr3, wi1, wi2, wi3;

  lda1 = m;
  lda2 = l;
  ldb1 = m;
  ldb2 = 4;

  for (i = 0; i < m; ++i) {
    x0 = c_re( ARR3D( a, i, 0, 0, lda1, lda2 ) ) + c_re( ARR3D( a, i, 0, 2, lda1, lda2 ) );
    y0 = c_im( ARR3D( a, i, 0, 0, lda1, lda2 ) ) + c_im( ARR3D( a, i, 0, 2, lda1, lda2 ) );
    x1 = c_re( ARR3D( a, i, 0, 0, lda1, lda2 ) ) - c_re( ARR3D( a, i, 0, 2, lda1, lda2 ) );
    y1 = c_im( ARR3D( a, i, 0, 0, lda1, lda2 ) ) - c_im( ARR3D( a, i, 0, 2, lda1, lda2 ) );

    x2 = c_re( ARR3D( a, i, 0, 1, lda1, lda2 ) ) + c_re( ARR3D( a, i, 0, 3, lda1, lda2 ) );
    y2 = c_im( ARR3D( a, i, 0, 1, lda1, lda2 ) ) + c_im( ARR3D( a, i, 0, 3, lda1, lda2 ) );
    x3 = c_im( ARR3D( a, i, 0, 1, lda1, lda2 ) ) - c_im( ARR3D( a, i, 0, 3, lda1, lda2 ) );
    y3 = c_re( ARR3D( a, i, 0, 3, lda1, lda2 ) ) - c_re( ARR3D( a, i, 0, 1, lda1, lda2 ) );

    c_re( ARR3D( b, i, 0, 0, ldb1, ldb2 ) ) = x0 + x2;
    c_im( ARR3D( b, i, 0, 0, ldb1, ldb2 ) ) = y0 + y2;
    c_re( ARR3D( b, i, 2, 0, ldb1, ldb2 ) ) = x0 - x2;
    c_im( ARR3D( b, i, 2, 0, ldb1, ldb2 ) ) = y0 - y2;

    c_re( ARR3D( b, i, 1, 0, ldb1, ldb2 ) ) = x1 + x3;
    c_im( ARR3D( b, i, 1, 0, ldb1, ldb2 ) ) = y1 + y3;
    c_re( ARR3D( b, i, 3, 0, ldb1, ldb2 ) ) = x1 - x3;
    c_im( ARR3D( b, i, 3, 0, ldb1, ldb2 ) ) = y1 - y3;
  }

  for (j = 1; j < l; ++j) {
    wr1 = c_re( w[j] );
    wi1 = c_im( w[j] );
    wr2 = wr1*wr1 - wi1*wi1;
    wi2 = wr1*wi1 + wr1*wi1;
    wr3 = wr1*wr2 - wi1*wi2;
    wi3 = wr1*wi2 + wi1*wr2;

    for (i = 0; i < m; ++i) {
      x0 = c_re( ARR3D( a, i, j, 0, lda1, lda2 ) ) + c_re( ARR3D( a, i, j, 2, lda1, lda2 ) );
      y0 = c_im( ARR3D( a, i, j, 0, lda1, lda2 ) ) + c_im( ARR3D( a, i, j, 2, lda1, lda2 ) );
      x1 = c_re( ARR3D( a, i, j, 0, lda1, lda2 ) ) - c_re( ARR3D( a, i, j, 2, lda1, lda2 ) );
      y1 = c_im( ARR3D( a, i, j, 0, lda1, lda2 ) ) - c_im( ARR3D( a, i, j, 2, lda1, lda2 ) );

      x2 = c_re( ARR3D( a, i, j, 1, lda1, lda2 ) ) + c_re( ARR3D( a, i, j, 3, lda1, lda2 ) );
      y2 = c_im( ARR3D( a, i, j, 1, lda1, lda2 ) ) + c_im( ARR3D( a, i, j, 3, lda1, lda2 ) );
      x3 = c_im( ARR3D( a, i, j, 1, lda1, lda2 ) ) - c_im( ARR3D( a, i, j, 3, lda1, lda2 ) );
      y3 = c_re( ARR3D( a, i, j, 3, lda1, lda2 ) ) - c_re( ARR3D( a, i, j, 1, lda1, lda2 ) );

      c_re( ARR3D( b, i, 0, j, ldb1, ldb2 ) ) = x0 + x2;
      c_im( ARR3D( b, i, 0, j, ldb1, ldb2 ) ) = y0 + y2;
      c_re( ARR3D( b, i, 2, j, ldb1, ldb2 ) ) = wr2 * (x0-x2) - wi2 * (y0-y2);
      c_im( ARR3D( b, i, 2, j, ldb1, ldb2 ) ) = wr2 * (y0-y2) + wi2 * (x0-x2);
      c_re( ARR3D( b, i, 1, j, ldb1, ldb2 ) ) = wr1 * (x1+x3) - wi1 * (y1+y3);
      c_im( ARR3D( b, i, 1, j, ldb1, ldb2 ) ) = wr1 * (y1+y3) + wi1 * (x1+x3);
      c_re( ARR3D( b, i, 3, j, ldb1, ldb2 ) ) = wr3 * (x1-x3) - wi3 * (y1-y3);
      c_im( ARR3D( b, i, 3, j, ldb1, ldb2 ) ) = wr3 * (y1-y3) + wi3 * (x1-x3);
    }
  }
}

static void
fft8a(fftw_complex *a, fftw_complex *b, fftw_complex *w, int l) {
  int j, lda, ldb;
  double x0, x1, x2, x3, x4, x5, x6, x7, y0, y1, y2, y3, y4, y5, y6, y7;
  double wr1, wr2, wr3, wr4, wr5, wr6, wr7, wi1, wi2, wi3, wi4, wi5, wi6, wi7;
  double u0, u1, u2, u3, v0, v1, v2, v3;
  double c81 = 0.70710678118654752;

  lda = l;
  ldb = 8;

  for (j = 0; j < l; ++j) {
    wr1 = c_re( w[j] );
    wi1 = c_im( w[j] );
    wr2 = wr1*wr1 - wi1*wi1;
    wi2 = wr1*wi1 + wr1*wi1;
    wr3 = wr1*wr2 - wi1*wi2;
    wi3 = wr1*wi2 + wi1*wr2;
    wr4 = wr2*wr2 - wi2*wi2;
    wi4 = wr2*wi2 + wr2*wi2;
    wr5 = wr2*wr3 - wi2*wi3;
    wi5 = wr2*wi3 + wi2*wr3;
    wr6 = wr3*wr3 - wi3*wi3;
    wi6 = wr3*wi3 + wr3*wi3;
    wr7 = wr3*wr4 - wi3*wi4;
    wi7 = wr3*wi4 + wi3*wr4;

    x0 = c_re( ARR2D( a, j, 0, lda ) ) + c_re( ARR2D( a, j, 4, lda ) );
    y0 = c_im( ARR2D( a, j, 0, lda ) ) + c_im( ARR2D( a, j, 4, lda ) );
    x1 = c_re( ARR2D( a, j, 0, lda ) ) - c_re( ARR2D( a, j, 4, lda ) );
    y1 = c_im( ARR2D( a, j, 0, lda ) ) - c_im( ARR2D( a, j, 4, lda ) );

    x2 = c_re( ARR2D( a, j, 2, lda ) ) + c_re( ARR2D( a, j, 6, lda ) );
    y2 = c_im( ARR2D( a, j, 2, lda ) ) + c_im( ARR2D( a, j, 6, lda ) );
    x3 = c_im( ARR2D( a, j, 2, lda ) ) - c_im( ARR2D( a, j, 6, lda ) );
    y3 = c_re( ARR2D( a, j, 6, lda ) ) - c_re( ARR2D( a, j, 2, lda ) );

    u0 = x0 + x2;
    v0 = y0 + y2;
    u1 = x0 - x2;
    v1 = y0 - y2;

    x4 = c_re( ARR2D( a, j, 1, lda ) ) + c_re( ARR2D( a, j, 5, lda ) );
    y4 = c_im( ARR2D( a, j, 1, lda ) ) + c_im( ARR2D( a, j, 5, lda ) );
    x5 = c_re( ARR2D( a, j, 1, lda ) ) - c_re( ARR2D( a, j, 5, lda ) );
    y5 = c_im( ARR2D( a, j, 1, lda ) ) - c_im( ARR2D( a, j, 5, lda ) );

    x6 = c_re( ARR2D( a, j, 3, lda ) ) + c_re( ARR2D( a, j, 7, lda ) );
    y6 = c_im( ARR2D( a, j, 3, lda ) ) + c_im( ARR2D( a, j, 7, lda ) );
    x7 = c_re( ARR2D( a, j, 3, lda ) ) - c_re( ARR2D( a, j, 7, lda ) );
    y7 = c_im( ARR2D( a, j, 3, lda ) ) - c_im( ARR2D( a, j, 7, lda ) );

    u2 = x4 + x6;
    v2 = y4 + y6;
    u3 = y4 - y6;
    v3 = x6 - x4;

    c_re( ARR2D( b, 0, j, ldb ) ) = u0 + u2;
    c_im( ARR2D( b, 0, j, ldb ) ) = v0 + v2;
    c_re( ARR2D( b, 4, j, ldb ) ) = wr4 * (u0-u2) - wi4 * (v0-v2);
    c_im( ARR2D( b, 4, j, ldb ) ) = wr4 * (v0-v2) + wi4 * (u0-u2);
    c_re( ARR2D( b, 2, j, ldb ) ) = wr2 * (u1+u3) - wi2 * (v1+v3);
    c_im( ARR2D( b, 2, j, ldb ) ) = wr2 * (v1+v3) + wi2 * (u1+u3);
    c_re( ARR2D( b, 6, j, ldb ) ) = wr6 * (u1-u3) - wi6 * (v1-v3);
    c_im( ARR2D( b, 6, j, ldb ) ) = wr6 * (v1-v3) + wi6 * (u1-u3);

    u0 = x1 + c81 * (x5 - x7);
    v0 = y1 + c81 * (y5 - y7);
    u1 = x1 - c81 * (x5 - x7);
    v1 = y1 - c81 * (y5 - y7);
    u2 = x3 + c81 * (y5 + y7);
    v2 = y3 - c81 * (x5 + x7);
    u3 = x3 - c81 * (y5 + y7);
    v3 = y3 + c81 * (x5 + x7);

    c_re( ARR2D( b, 1, j, ldb ) ) = wr1 * (u0+u2) - wi1 * (v0+v2);
    c_im( ARR2D( b, 1, j, ldb ) ) = wr1 * (v0+v2) + wi1 * (u0+u2);
    c_re( ARR2D( b, 5, j, ldb ) ) = wr5 * (u1+u3) - wi5 * (v1+v3);
    c_im( ARR2D( b, 5, j, ldb ) ) = wr5 * (v1+v3) + wi5 * (u1+u3);
    c_re( ARR2D( b, 3, j, ldb ) ) = wr3 * (u1-u3) - wi3 * (v1-v3);
    c_im( ARR2D( b, 3, j, ldb ) ) = wr3 * (v1-v3) + wi3 * (u1-u3);
    c_re( ARR2D( b, 7, j, ldb ) ) = wr7 * (u0-u2) - wi7 * (v0-v2);
    c_im( ARR2D( b, 7, j, ldb ) ) = wr7 * (v0-v2) + wi7 * (u0-u2);
  }
}

static void
fft8b(fftw_complex *a, fftw_complex *b, fftw_complex *w, int m, int l) {
  int i, j, lda1, lda2, ldb1, ldb2;
  double x0, x1, x2, x3, x4, x5, x6, x7, y0, y1, y2, y3, y4, y5, y6, y7;
  double wr1, wr2, wr3, wr4, wr5, wr6, wr7, wi1, wi2, wi3, wi4, wi5, wi6, wi7;
  double u0, u1, u2, u3, v0, v1, v2, v3;
  double c81 = 0.70710678118654752;

  lda1 = m;
  lda2 = l;
  ldb1 = m;
  ldb2 = 8;

  for (i = 0; i < m; ++i) {
    x0 = c_re( ARR3D( a, i, 0, 0, lda1, lda2 ) ) + c_re( ARR3D( a, i, 0, 4, lda1, lda2 ) );
    y0 = c_im( ARR3D( a, i, 0, 0, lda1, lda2 ) ) + c_im( ARR3D( a, i, 0, 4, lda1, lda2 ) );
    x1 = c_re( ARR3D( a, i, 0, 0, lda1, lda2 ) ) - c_re( ARR3D( a, i, 0, 4, lda1, lda2 ) );
    y1 = c_im( ARR3D( a, i, 0, 0, lda1, lda2 ) ) - c_im( ARR3D( a, i, 0, 4, lda1, lda2 ) );

    x2 = c_re( ARR3D( a, i, 0, 2, lda1, lda2 ) ) + c_re( ARR3D( a, i, 0, 6, lda1, lda2 ) );
    y2 = c_im( ARR3D( a, i, 0, 2, lda1, lda2 ) ) + c_im( ARR3D( a, i, 0, 6, lda1, lda2 ) );
    x3 = c_im( ARR3D( a, i, 0, 2, lda1, lda2 ) ) - c_im( ARR3D( a, i, 0, 6, lda1, lda2 ) );
    y3 = c_re( ARR3D( a, i, 0, 6, lda1, lda2 ) ) - c_re( ARR3D( a, i, 0, 2, lda1, lda2 ) );

    u0 = x0 + x2;
    v0 = y0 + y2;
    u1 = x0 - x2;
    v1 = y0 - y2;

    x4 = c_re( ARR3D( a, i, 0, 1, lda1, lda2 ) ) + c_re( ARR3D( a, i, 0, 5, lda1, lda2 ) );
    y4 = c_im( ARR3D( a, i, 0, 1, lda1, lda2 ) ) + c_im( ARR3D( a, i, 0, 5, lda1, lda2 ) );
    x5 = c_re( ARR3D( a, i, 0, 1, lda1, lda2 ) ) - c_re( ARR3D( a, i, 0, 5, lda1, lda2 ) );
    y5 = c_im( ARR3D( a, i, 0, 1, lda1, lda2 ) ) - c_im( ARR3D( a, i, 0, 5, lda1, lda2 ) );

    x6 = c_re( ARR3D( a, i, 0, 3, lda1, lda2 ) ) + c_re( ARR3D( a, i, 0, 7, lda1, lda2 ) );
    y6 = c_im( ARR3D( a, i, 0, 3, lda1, lda2 ) ) + c_im( ARR3D( a, i, 0, 7, lda1, lda2 ) );
    x7 = c_re( ARR3D( a, i, 0, 3, lda1, lda2 ) ) - c_re( ARR3D( a, i, 0, 7, lda1, lda2 ) );
    y7 = c_im( ARR3D( a, i, 0, 3, lda1, lda2 ) ) - c_im( ARR3D( a, i, 0, 7, lda1, lda2 ) );

    u2 = x4 + x6;
    v2 = y4 + y6;
    u3 = y4 - y6;
    v3 = x6 - x4;

    c_re( ARR3D( b, i, 0, 0, ldb1, ldb2 ) ) = u0 + u2;
    c_im( ARR3D( b, i, 0, 0, ldb1, ldb2 ) ) = v0 + v2;
    c_re( ARR3D( b, i, 4, 0, ldb1, ldb2 ) ) = u0 - u2;
    c_im( ARR3D( b, i, 4, 0, ldb1, ldb2 ) ) = v0 - v2;

    c_re( ARR3D( b, i, 2, 0, ldb1, ldb2 ) ) = u1 + u3;
    c_im( ARR3D( b, i, 2, 0, ldb1, ldb2 ) ) = v1 + v3;
    c_re( ARR3D( b, i, 6, 0, ldb1, ldb2 ) ) = u1 - u3;
    c_im( ARR3D( b, i, 6, 0, ldb1, ldb2 ) ) = v1 - v3;

    u0 = x1 + c81 * (x5 - x7);
    v0 = y1 + c81 * (y5 - y7);
    u1 = x1 - c81 * (x5 - x7);
    v1 = y1 - c81 * (y5 - y7);
    u2 = x3 + c81 * (y5 + y7);
    v2 = y3 - c81 * (x5 + x7);
    u3 = x3 - c81 * (y5 + y7);
    v3 = y3 + c81 * (x5 + x7);

    c_re( ARR3D( b, i, 1, 0, ldb1, ldb2 ) ) = u0 + u2;
    c_im( ARR3D( b, i, 1, 0, ldb1, ldb2 ) ) = v0 + v2;
    c_re( ARR3D( b, i, 5, 0, ldb1, ldb2 ) ) = u1 + u3;
    c_im( ARR3D( b, i, 5, 0, ldb1, ldb2 ) ) = v1 + v3;

    c_re( ARR3D( b, i, 3, 0, ldb1, ldb2 ) ) = u1 - u3;
    c_im( ARR3D( b, i, 3, 0, ldb1, ldb2 ) ) = v1 - v3;
    c_re( ARR3D( b, i, 7, 0, ldb1, ldb2 ) ) = u0 - u2;
    c_im( ARR3D( b, i, 7, 0, ldb1, ldb2 ) ) = v0 - v2;
  }

  for (j = 1; j < l; ++j) {
    wr1 = c_re( w[j] );
    wi1 = c_im( w[j] );
    wr2 = wr1*wr1 - wi1*wi1;
    wi2 = wr1*wi1 + wr1*wi1;
    wr3 = wr1*wr2 - wi1*wi2;
    wi3 = wr1*wi2 + wi1*wr2;
    wr4 = wr2*wr2 - wi2*wi2;
    wi4 = wr2*wi2 + wr2*wi2;
    wr5 = wr2*wr3 - wi2*wi3;
    wi5 = wr2*wi3 + wi2*wr3;
    wr6 = wr3*wr3 - wi3*wi3;
    wi6 = wr3*wi3 + wr3*wi3;
    wr7 = wr3*wr4 - wi3*wi4;
    wi7 = wr3*wi4 + wi3*wr4;

    for (i = 0; i < m; ++i) {
      x0 = c_re( ARR3D( a, i, j, 0, lda1, lda2 ) ) + c_re( ARR3D( a, i, j, 4, lda1, lda2 ) );
      y0 = c_im( ARR3D( a, i, j, 0, lda1, lda2 ) ) + c_im( ARR3D( a, i, j, 4, lda1, lda2 ) );
      x1 = c_re( ARR3D( a, i, j, 0, lda1, lda2 ) ) - c_re( ARR3D( a, i, j, 4, lda1, lda2 ) );
      y1 = c_im( ARR3D( a, i, j, 0, lda1, lda2 ) ) - c_im( ARR3D( a, i, j, 4, lda1, lda2 ) );

      x2 = c_re( ARR3D( a, i, j, 2, lda1, lda2 ) ) + c_re( ARR3D( a, i, j, 6, lda1, lda2 ) );
      y2 = c_im( ARR3D( a, i, j, 2, lda1, lda2 ) ) + c_im( ARR3D( a, i, j, 6, lda1, lda2 ) );
      x3 = c_im( ARR3D( a, i, j, 2, lda1, lda2 ) ) - c_im( ARR3D( a, i, j, 6, lda1, lda2 ) );
      y3 = c_re( ARR3D( a, i, j, 6, lda1, lda2 ) ) - c_re( ARR3D( a, i, j, 2, lda1, lda2 ) );

      u0 = x0 + x2;
      v0 = y0 + y2;
      u1 = x0 - x2;
      v1 = y0 - y2;

      x4 = c_re( ARR3D( a, i, j, 1, lda1, lda2 ) ) + c_re( ARR3D( a, i, j, 5, lda1, lda2 ) );
      y4 = c_im( ARR3D( a, i, j, 1, lda1, lda2 ) ) + c_im( ARR3D( a, i, j, 5, lda1, lda2 ) );
      x5 = c_re( ARR3D( a, i, j, 1, lda1, lda2 ) ) - c_re( ARR3D( a, i, j, 5, lda1, lda2 ) );
      y5 = c_im( ARR3D( a, i, j, 1, lda1, lda2 ) ) - c_im( ARR3D( a, i, j, 5, lda1, lda2 ) );

      x6 = c_re( ARR3D( a, i, j, 3, lda1, lda2 ) ) + c_re( ARR3D( a, i, j, 7, lda1, lda2 ) );
      y6 = c_im( ARR3D( a, i, j, 3, lda1, lda2 ) ) + c_im( ARR3D( a, i, j, 7, lda1, lda2 ) );
      x7 = c_re( ARR3D( a, i, j, 3, lda1, lda2 ) ) - c_re( ARR3D( a, i, j, 7, lda1, lda2 ) );
      y7 = c_im( ARR3D( a, i, j, 3, lda1, lda2 ) ) - c_im( ARR3D( a, i, j, 7, lda1, lda2 ) );

      u2 = x4 + x6;
      v2 = y4 + y6;
      u3 = y4 - y6;
      v3 = x6 - x4;

      c_re( ARR3D( b, i, 0, j, ldb1, ldb2 ) ) = u0 + u2;
      c_im( ARR3D( b, i, 0, j, ldb1, ldb2 ) ) = v0 + v2;
      c_re( ARR3D( b, i, 4, j, ldb1, ldb2 ) ) = wr4 * (u0-u2) - wi4 * (v0-v2);
      c_im( ARR3D( b, i, 4, j, ldb1, ldb2 ) ) = wr4 * (v0-v2) + wi4 * (u0-u2);
      c_re( ARR3D( b, i, 2, j, ldb1, ldb2 ) ) = wr2 * (u1+u3) - wi2 * (v1+v3);
      c_im( ARR3D( b, i, 2, j, ldb1, ldb2 ) ) = wr2 * (v1+v3) + wi2 * (u1+u3);
      c_re( ARR3D( b, i, 6, j, ldb1, ldb2 ) ) = wr6 * (u1-u3) - wi6 * (v1-v3);
      c_im( ARR3D( b, i, 6, j, ldb1, ldb2 ) ) = wr6 * (v1-v3) + wi6 * (u1-u3);

      u0 = x1 + c81 * (x5 - x7);
      v0 = y1 + c81 * (y5 - y7);
      u1 = x1 - c81 * (x5 - x7);
      v1 = y1 - c81 * (y5 - y7);
      u2 = x3 + c81 * (y5 + y7);
      v2 = y3 - c81 * (x5 + x7);
      u3 = x3 - c81 * (y5 + y7);
      v3 = y3 + c81 * (x5 + x7);

      c_re( ARR3D( b, i, 1, j, ldb1, ldb2 ) ) = wr1 * (u0+u2) - wi1 * (v0+v2);
      c_im( ARR3D( b, i, 1, j, ldb1, ldb2 ) ) = wr1 * (v0+v2) + wi1 * (u0+u2);
      c_re( ARR3D( b, i, 5, j, ldb1, ldb2 ) ) = wr5 * (u1+u3) - wi5 * (v1+v3);
      c_im( ARR3D( b, i, 5, j, ldb1, ldb2 ) ) = wr5 * (v1+v3) + wi5 * (u1+u3);
      c_re( ARR3D( b, i, 3, j, ldb1, ldb2 ) ) = wr3 * (u1-u3) - wi3 * (v1-v3);
      c_im( ARR3D( b, i, 3, j, ldb1, ldb2 ) ) = wr3 * (v1-v3) + wi3 * (u1-u3);
      c_re( ARR3D( b, i, 7, j, ldb1, ldb2 ) ) = wr7 * (u0-u2) - wi7 * (v0-v2);
      c_im( ARR3D( b, i, 7, j, ldb1, ldb2 ) ) = wr7 * (v0-v2) + wi7 * (u0-u2);
    }
  }
}

static void
fft3a(fftw_complex *a, fftw_complex *b, fftw_complex *w, int l) {
  int j;
  double x0, x1, x2;
  double y0, y1, y2;
  double wr1, wr2;
  double wi1, wi2;
  double c31 = 0.86602540378443865, c32 = 0.5;

  for (j = 0; j < l; ++j) {
    wr1 = c_re( w[j] );
    wi1 = c_im( w[j] );
    wr2=wr1*wr1-wi1*wi1;
    wi2=wr1*wi1+wr1*wi1;
    x0 = c_re( ARR2D( a, j, 1, l ) ) + c_re( ARR2D( a, j, 2, l ) );
    y0 = c_im( ARR2D( a, j, 1, l ) ) + c_im( ARR2D( a, j, 2, l ) );
    x1 = c_re( ARR2D( a, j, 0, l ) ) - c32 * x0;
    y1 = c_im( ARR2D( a, j, 0, l ) ) - c32 * y0;
    x2 = c31 * ( c_im( ARR2D( a, j, 1, l ) ) - c_im( ARR2D( a, j, 2, l ) ));
    y2 = c31 * ( c_re( ARR2D( a, j, 2, l ) ) - c_re( ARR2D( a, j, 1, l ) ));
    c_re( ARR2D( b, 0, j, 3 ) ) = c_re( ARR2D( a, j, 0, l ) ) + x0;
    c_im( ARR2D( b, 0, j, 3 ) ) = c_im( ARR2D( a, j, 0, l ) ) + y0;
    c_re( ARR2D( b, 1, j, 3 ) ) = wr1*(x1+x2)-wi1*(y1+y2);
    c_im( ARR2D( b, 1, j, 3 ) ) = wr1*(y1+y2)+wi1*(x1+x2);
    c_re( ARR2D( b, 2, j, 3 ) ) = wr2*(x1-x2)-wi2*(y1-y2);
    c_im( ARR2D( b, 2, j, 3 ) ) = wr2*(y1-y2)+wi2*(x1-x2);
  }
}

static void
fft3b(fftw_complex *a, fftw_complex *b, fftw_complex *w, int m, int l) {
  int i, j;
  double x0, x1, x2;
  double y0, y1, y2;
  double wr1, wr2;
  double wi1, wi2;
  double c31 = 0.86602540378443865, c32 = 0.5;

  for (i = 0; i < m; ++i) {
    x0 = c_re( ARR3D( a, i, 0, 1, m, l ) ) + c_re( ARR3D( a, i, 0, 2, m, l ) );
    y0 = c_im( ARR3D( a, i, 0, 1, m, l ) ) + c_im( ARR3D( a, i, 0, 2, m, l ) );
    x1 = c_re( ARR3D( a, i, 0, 0, m, l ) ) - c32 * x0;
    y1 = c_im( ARR3D( a, i, 0, 0, m, l ) ) - c32 * y0;
    x2 = c31 * (c_im( ARR3D( a, i, 0, 1, m, l ) ) - c_im( ARR3D( a, i, 0, 2, m, l ) ));
    y2 = c31 * (c_re( ARR3D( a, i, 0, 2, m, l ) ) - c_re( ARR3D( a, i, 0, 1, m, l ) ));
    c_re( ARR3D( b, i, 0, 0, m, 3 ) ) = c_re( ARR3D( a, i, 0, 0, m, l ) ) + x0;
    c_im( ARR3D( b, i, 0, 0, m, 3 ) ) = c_im( ARR3D( a, i, 0, 0, m, l ) ) + y0;
    c_re( ARR3D( b, i, 1, 0, m, 3 ) ) = x1 + x2;
    c_im( ARR3D( b, i, 1, 0, m, 3 ) ) = y1 + y2;
    c_re( ARR3D( b, i, 2, 0, m, 3 ) ) = x1 - x2;
    c_im( ARR3D( b, i, 2, 0, m, 3 ) ) = y1 - y2;
  }

  for (j = 1; j < l; ++j) {
    wr1 = c_re( w[j] );
    wi1 = c_im( w[j] );
    wr2=wr1*wr1-wi1*wi1;
    wi2=wr1*wi1+wr1*wi1;
    for (i = 0; i < m; ++i) {
      x0 = c_re( ARR3D( a, i, j, 1, m, l ) ) + c_re( ARR3D( a, i, j, 2, m, l ) );
      y0 = c_im( ARR3D( a, i, j, 1, m, l ) ) + c_im( ARR3D( a, i, j, 2, m, l ) );
      x1 = c_re( ARR3D( a, i, j, 0, m, l ) ) - c32 * x0;
      y1 = c_im( ARR3D( a, i, j, 0, m, l ) ) - c32 * y0;
      x2 = c31 * (c_im( ARR3D( a, i, j, 1, m, l ) ) - c_im( ARR3D( a, i, j, 2, m, l ) ));
      y2 = c31 * (c_re( ARR3D( a, i, j, 2, m, l ) ) - c_re( ARR3D( a, i, j, 1, m, l ) ));
      c_re( ARR3D( b, i, 0, j, m, 3 ) ) = c_re( ARR3D( a, i, j, 0, m, l ) ) + x0;
      c_im( ARR3D( b, i, 0, j, m, 3 ) ) = c_im( ARR3D( a, i, j, 0, m, l ) ) + y0;
      c_re( ARR3D( b, i, 1, j, m, 3 ) ) = wr1*(x1+x2)-wi1*(y1+y2);
      c_im( ARR3D( b, i, 1, j, m, 3 ) ) = wr1*(y1+y2)+wi1*(x1+x2);
      c_re( ARR3D( b, i, 2, j, m, 3 ) ) = wr2*(x1-x2)-wi2*(y1-y2);
      c_im( ARR3D( b, i, 2, j, m, 3 ) ) = wr2*(y1-y2)+wi2*(x1-x2);
    }
  }
}

static void
fft5a(fftw_complex *a, fftw_complex *b, fftw_complex *w, int l) {
  int j;
  double wr1, wr2, wr3, wr4;
  double wi1, wi2, wi3, wi4;
  double x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10;
  double y0, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10;
  double c51 = 0.95105651629515357, c52 = 0.61803398874989485;
  double c53 = 0.55901699437494742, c54 = 0.25;

  for (j = 0; j < l; ++j) {
    wr1 = c_re( w[j] );
    wi1 = c_im( w[j] );
    wr2=wr1*wr1-wi1*wi1;
    wi2=wr1*wi1+wr1*wi1;
    wr3=wr1*wr2-wi1*wi2;
    wi3=wr1*wi2+wi1*wr2;
    wr4=wr2*wr2-wi2*wi2;
    wi4=wr2*wi2+wr2*wi2;
    x0 = c_re( ARR2D( a, j, 1, l ) ) + c_re( ARR2D( a, j, 4, l ) );
    y0 = c_im( ARR2D( a, j, 1, l ) ) + c_im( ARR2D( a, j, 4, l ) );
    x1 = c_re( ARR2D( a, j, 2, l ) ) + c_re( ARR2D( a, j, 3, l ) );
    y1 = c_im( ARR2D( a, j, 2, l ) ) + c_im( ARR2D( a, j, 3, l ) );
    x2 = c51 * (c_re( ARR2D( a, j, 1, l ) ) - c_re( ARR2D( a, j, 4, l ) ));
    y2 = c51 * (c_im( ARR2D( a, j, 1, l ) ) - c_im( ARR2D( a, j, 4, l ) ));
    x3 = c51 * (c_re( ARR2D( a, j, 2, l ) ) - c_re( ARR2D( a, j, 3, l ) ));
    y3 = c51 * (c_im( ARR2D( a, j, 2, l ) ) - c_im( ARR2D( a, j, 3, l ) ));
    x4 = x0 + x1;
    y4 = y0 + y1;
    x5 = c53 * (x0-x1);
    y5 = c53 * (y0-y1);
    x6 = c_re( ARR2D( a, j, 0, l ) ) - c54 * x4;
    y6 = c_im( ARR2D( a, j, 0, l ) ) - c54 * y4;
    x7 = x6 + x5;
    y7 = y6 + y5;
    x8 = x6 - x5;
    y8 = y6 - y5;
    x9 = y2 + c52*y3;
    y9 = -x2 - c52*x3;
    x10 = c52*y2 - y3;
    y10 = x3 - c52*x2;
    c_re( ARR2D( b, 0, j, 5 ) ) = c_re( ARR2D( a, j, 0, l ) ) + x4;
    c_im( ARR2D( b, 0, j, 5 ) ) = c_im( ARR2D( a, j, 0, l ) ) + y4;
    c_re( ARR2D( b, 1, j, 5 ) ) = wr1 * (x7+x9) - wi1 * (y7+y9);
    c_im( ARR2D( b, 1, j, 5 ) ) = wr1 * (y7+y9) + wi1 * (x7+x9);
    c_re( ARR2D( b, 2, j, 5 ) ) = wr2 * (x8+x10) - wi2 * (y8+y10);
    c_im( ARR2D( b, 2, j, 5 ) ) = wr2 * (y8+y10) + wi2 * (x8+x10);
    c_re( ARR2D( b, 3, j, 5 ) ) = wr3 * (x8-x10) - wi3 * (y8-y10);
    c_im( ARR2D( b, 3, j, 5 ) ) = wr3 * (y8-y10) + wi3 * (x8-x10);
    c_re( ARR2D( b, 4, j, 5 ) ) = wr4 * (x7-x9) - wi4 * (y7-y9);
    c_im( ARR2D( b, 4, j, 5 ) ) = wr4 * (y7-y9) + wi4 * (x7-x9);
  }
}

static void
fft5b(fftw_complex *a, fftw_complex *b, fftw_complex *w, int m, int l) {
  int i, j;
  double x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10;
  double y0, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10;
  double wr1, wr2, wr3, wr4;
  double wi1, wi2, wi3, wi4;
  double c51 = 0.95105651629515357, c52 = 0.61803398874989485;
  double c53 = 0.55901699437494742, c54 = 0.25;

  for (i = 0; i < m; ++i) {
    x0 = c_re( ARR3D( a, i, 0, 1, m, l ) ) + c_re( ARR3D( a, i, 0, 4, m, l ) );
    y0 = c_im( ARR3D( a, i, 0, 1, m, l ) ) + c_im( ARR3D( a, i, 0, 4, m, l ) );
    x1 = c_re( ARR3D( a, i, 0, 2, m, l ) ) + c_re( ARR3D( a, i, 0, 3, m, l ) );
    y1 = c_im( ARR3D( a, i, 0, 2, m, l ) ) + c_im( ARR3D( a, i, 0, 3, m, l ) );
    x2 = c51 * (c_re( ARR3D( a, i, 0, 1, m, l ) ) - c_re( ARR3D( a, i, 0, 4, m, l ) ));
    y2 = c51 * (c_im( ARR3D( a, i, 0, 1, m, l ) ) - c_im( ARR3D( a, i, 0, 4, m, l ) ));
    x3 = c51 * (c_re( ARR3D( a, i, 0, 2, m, l ) ) - c_re( ARR3D( a, i, 0, 3, m, l ) ));
    y3 = c51 * (c_im( ARR3D( a, i, 0, 2, m, l ) ) - c_im( ARR3D( a, i, 0, 3, m, l ) ));
    x4 = x0 + x1;
    y4 = y0 + y1;
    x5 = c53 * (x0-x1);
    y5 = c53 * (y0-y1);
    x6 = c_re( ARR3D( a, i, 0, 0, m, l ) ) - c54 * x4;
    y6 = c_im( ARR3D( a, i, 0, 0, m, l ) ) - c54 * y4;
    x7 = x6 + x5;
    y7 = y6 + y5;
    x8 = x6 - x5;
    y8 = y6 - y5;
    x9 = y2 + c52 * y3;
    y9 = -x2 - c52 * x3;
    x10 = c52 * y2 - y3;
    y10 = x3 - c52 * x2;
    c_re( ARR3D( b, i, 0, 0, m, 5 ) ) = c_re( ARR3D( a, i, 0, 0, m, l ) ) + x4;
    c_im( ARR3D( b, i, 0, 0, m, 5 ) ) = c_im( ARR3D( a, i, 0, 0, m, l ) ) + y4;
    c_re( ARR3D( b, i, 1, 0, m, 5 ) ) = x7 + x9;
    c_im( ARR3D( b, i, 1, 0, m, 5 ) ) = y7 + y9;
    c_re( ARR3D( b, i, 2, 0, m, 5 ) ) = x8 + x10;
    c_im( ARR3D( b, i, 2, 0, m, 5 ) ) = y8 + y10;
    c_re( ARR3D( b, i, 3, 0, m, 5 ) ) = x8 - x10;
    c_im( ARR3D( b, i, 3, 0, m, 5 ) ) = y8 - y10;
    c_re( ARR3D( b, i, 4, 0, m, 5 ) ) = x7 - x9;
    c_im( ARR3D( b, i, 4, 0, m, 5 ) ) = y7 - y9;
  }

  for (j = 1; j < l; ++j) {
    wr1 = c_re( w[j] );
    wi1 = c_im( w[j] );
    wr2 = wr1 * wr1 - wi1*wi1;
    wi2 = wr1 * wi1 + wr1*wi1;
    wr3 = wr1 * wr2 - wi1*wi2;
    wi3 = wr1 * wi2 + wi1*wr2;
    wr4 = wr2 * wr2 - wi2*wi2;
    wi4 = wr2 * wi2 + wr2*wi2;
    for (i = 0; i < m; ++i) {
      x0 = c_re( ARR3D( a, i, j, 1, m, l ) ) + c_re( ARR3D( a, i, j, 4, m, l ) );
      y0 = c_im( ARR3D( a, i, j, 1, m, l ) ) + c_im( ARR3D( a, i, j, 4, m, l ) );
      x1 = c_re( ARR3D( a, i, j, 2, m, l ) ) + c_re( ARR3D( a, i, j, 3, m, l ) );
      y1 = c_im( ARR3D( a, i, j, 2, m, l ) ) + c_im( ARR3D( a, i, j, 3, m, l ) );
      x2 = c51 * (c_re( ARR3D( a, i, j, 1, m, l ) ) - c_re( ARR3D( a, i, j, 4, m, l ) ));
      y2 = c51 * (c_im( ARR3D( a, i, j, 1, m, l ) ) - c_im( ARR3D( a, i, j, 4, m, l ) ));
      x3 = c51 * (c_re( ARR3D( a, i, j, 2, m, l ) ) - c_re( ARR3D( a, i, j, 3, m, l ) ));
      y3 = c51 * (c_im( ARR3D( a, i, j, 2, m, l ) ) - c_im( ARR3D( a, i, j, 3, m, l ) ));
      x4 = x0 + x1;
      y4 = y0 + y1;
      x5 = c53 * (x0-x1);
      y5 = c53 * (y0-y1);
      x6 = c_re( ARR3D( a, i, j, 0, m, l ) ) - c54*x4;
      y6 = c_im( ARR3D( a, i, j, 0, m, l ) ) - c54*y4;
      x7 = x6 + x5;
      y7 = y6 + y5;
      x8 = x6 - x5;
      y8 = y6 - y5;
      x9 = y2 + c52 * y3;
      y9 = -x2 - c52 * x3;
      x10 = c52*y2 - y3;
      y10 = x3 - c52*x2;
      c_re( ARR3D( b, i, 0, j, m, 5 ) ) = c_re( ARR3D( a, i, j, 0, m, l ) ) + x4;
      c_im( ARR3D( b, i, 0, j, m, 5 ) ) = c_im( ARR3D( a, i, j, 0, m, l ) ) + y4;
      c_re( ARR3D( b, i, 1, j, m, 5 ) ) = wr1*(x7+x9) - wi1*(y7+y9);
      c_im( ARR3D( b, i, 1, j, m, 5 ) ) = wr1*(y7+y9) + wi1*(x7+x9);
      c_re( ARR3D( b, i, 2, j, m, 5 ) ) = wr2*(x8+x10) - wi2*(y8+y10);
      c_im( ARR3D( b, i, 2, j, m, 5 ) ) = wr2*(y8+y10) + wi2*(x8+x10);
      c_re( ARR3D( b, i, 3, j, m, 5 ) ) = wr3*(x8-x10) - wi3*(y8-y10);
      c_im( ARR3D( b, i, 3, j, m, 5 ) ) = wr3*(y8-y10) + wi3*(x8-x10);
      c_re( ARR3D( b, i, 4, j, m, 5 ) ) = wr4*(x7-x9) - wi4*(y7-y9);
      c_im( ARR3D( b, i, 4, j, m, 5 ) ) = wr4*(y7-y9) + wi4*(x7-x9);
    }
  }
}

static void
fft3(fftw_complex *a, fftw_complex *b, fftw_complex *w, int m, int l) {
  if (1 == m)
    fft3a( a, b, w, l );
  else
    fft3b( a, b, w, m, l );
}

static void
fft4(fftw_complex *a, fftw_complex *b, fftw_complex *w, int m, int l) {
  if (1 == m)
    fft4a( a, b, w, l );
  else
    fft4b( a, b, w, m, l );
}

static void
fft5(fftw_complex *a, fftw_complex *b, fftw_complex *w, int m, int l) {
  if (1 == m)
    fft5a( a, b, w, l );
  else
    fft5b( a, b, w, m, l );
}

static void
fft8(fftw_complex *a, fftw_complex *b, fftw_complex *w, int m, int l) {
  if (1 == m)
    fft8a( a, b, w, l );
  else
    fft8b( a, b, w, m, l );
}

int
HPCC_fft235(fftw_complex *a, fftw_complex *b, fftw_complex *w, int n, const int *ip) {
  int j, k, l, m, key, kp4, kp8;

  if (ip[0] != 1) {
    kp4 = 2 - (ip[0] + 2) % 3;
    kp8 = (ip[0]-kp4) / 3;
  } else {
    kp4 = 0;
    kp8 = 0;
  }

  key = 1;
  j = 0;
  l = n;
  m = 1;

  for (k = 0; k < kp8; ++k) {
    l >>= 3; /* divide by 8 */

    if (l >= 2) {
      if (key > 0)
        fft8( a, b, w + j, m, l );
      else
        fft8( b, a, w + j, m, l );

      key = -key;
    } else {
      if (key > 0)
        fft8( a, a, w + j, m, l );
      else
        fft8( b, a, w + j, m, l );
    }
    m <<= 3; /* multiply by 8 */
    j += l;
  }

  for (k = 0; k < ip[2]; ++k) {
    l /= 5;

    if (l >= 2) {
      if (key > 0)
        fft5( a, b, w+j, m, l );
      else
        fft5( b, a, w+j, m, l );

      key = -key;
    } else {
      if (key > 0)
        fft5( a, a, w+j, m, l );
      else
        fft5( b, a, w+j, m, l );
    }

    m *= 5;
    j += l;
  }

  for (k = 0; k < kp4; ++k) {
    l >>= 2; /* divide by 4 */

    if (l >= 2) {
      if (key > 0)
        fft4( a, b, w + j, m, l );
      else
        fft4( b, a, w + j, m, l );

      key = -key;
    } else {
      if (key > 0)
        fft4( a, a, w + j, m, l );
      else
        fft4( b, a, w + j, m, l );
    }
    m <<= 2; /* multiply by 4 */
    j += l;
  }

  for (k = 0; k < ip[1]; ++k) {
    l /= 3;

    if (l >= 2) {
      if (key > 0)
        fft3( a, b, w+j, m, l );
      else
        fft3( b, a, w+j, m, l );

      key = -key;
    } else {
      if (key > 0)
        fft3( a, a, w+j, m, l );
      else
        fft3( b, a, w+j, m, l );
    }

    m *= 3;
    j += l;
  }

  if (ip[0] == 1) {
    if (key > 0)
      fft2( a, a, m );
    else
      fft2( b, a, m );
  }

  return 0;
}

static int
settbl0(fftw_complex *w, int m, int l) {
  int i;
  double pi2, px;

  pi2 = 8.0 * atan(1.0);
  px = -pi2 / m / l;

  for (i = 0; i < l; ++i) {
    c_re(w[i]) = cos(px * i);
    c_im(w[i]) = sin(px * i);
  }

  return 0;
}

int
HPCC_settbl(fftw_complex *w, int n) {
  int j, k, l, kp4, kp8;
  int ip[3];

  HPCC_factor235( n, ip );

  if (1 != ip[0]) {
    kp4 = 2 - (ip[0] + 2) % 3;
    kp8 = (ip[0]-kp4) / 3;
  } else {
    kp4 = 0;
    kp8 = 0;
  }

  j = 0;
  l = n;

  for (k = 0; k < kp8; ++k) {
    l >>= 3; /* divide by 8 */
    settbl0( w + j, 8, l );
    j += l;
  }

  for (k = 0; k < ip[2]; ++k) {
    l /= 5;
    settbl0( w + j, 5, l );
    j += l;
  }

  for (k = 0; k < kp4; ++k) {
    l >>= 2; /* divide by 4 */
    settbl0( w + j, 4, l );
    j += l;
  }

  for (k = 0; k < ip[1]; ++k) {
    l /= 3;
    settbl0( w + j, 3, l );
    j += l;
  }

  return 0;
}	/* settbl */

int
HPCC_factor235(int n, int *ip) {
  ip[0] = ip[1] = ip[2] = 0;

  if (n % 2 != 0 && n % 3 != 0 && n % 5 != 0)
    return 1;

  if (n <= 1)
    return 1;

  /* count all 2 factors */
  for (; n > 1 && ! (n & 1); n >>= 1)
    ip[0]++;

  /* count all 3 factors */
  for (; n > 1 && ! (n % 3); n /= 3)
    ip[1]++;

  /* count all 5 factors */
  for (; n > 1 && ! (n % 5); n /= 5)
    ip[2]++;

  if (n != 1)
    return 1;

  return 0;
}

int
HPCC_factor235_8(s64Int_t n, int *ip) {
  ip[0] = ip[1] = ip[2] = 0;

  if (n % 2 != 0 && n % 3 != 0 && n % 5 != 0)
    return 1;

  if (n <= 1)
    return 1;

  /* count all 2 factors */
  for (; n > 1 && ! (n & 1); n >>= 1)
    ip[0]++;

  /* count all 3 factors */
  for (; n > 1 && ! (n % 3); n /= 3)
    ip[1]++;

  /* count all 5 factors */
  for (; n > 1 && ! (n % 5); n /= 5)
    ip[2]++;

  if (n != 1)
    return 1;

  return 0;
}
