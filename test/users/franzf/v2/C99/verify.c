/*  This file is based on the file ridft-verify.c
    of FFTW 3.0 by Matteo Frigo and Steven Johnson.

    Modified by Yevgen Voronenko and Franz Franchetti.
*/


/*
 * Copyright (c) 2000 Matteo Frigo
 * Copyright (c) 2000 Steven G. Johnson
 *
 * This program is _mm_free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the _mm_free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the _mm_free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* $Id$ */

/*************************************************
 * correctness test for multi-dimensional DFTs
 *************************************************/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#include "include/libfft.h"
#include "include/omega64c.h"

#define ROUNDS 5
#define SIGN (+1)
#define TOLERANCE 1E-5
#undef EXIT_ON_FAIL


#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795L
#endif

double double_rand() { return ((double)rand())/(RAND_MAX); }

typedef void (fftd_func)(double *,double *);
typedef struct {double re; double im;} COMPLEX;
typedef double REAL;

#define RE(a) (a).re
#define IM(a) (a).im

/***************************************************************************/

static double compute_error(COMPLEX *A, COMPLEX *B, int n)
{
     /* compute the relative error */
     double error = 0.0;
     double tol = TOLERANCE;
     int i;

     for (i = 0; i < n; ++i) {
	  double a;
	  double mag;
	  a = hypot(RE(A[i]) - RE(B[i]), IM(A[i]) - IM(B[i]));
	  mag = 0.5 * (hypot(RE(A[i]), IM(A[i])) +
		       hypot(RE(B[i]), IM(B[i]))) + tol;

	  a /= mag;
	  if (a > error)
	       error = a;

#ifdef HAVE_ISNAN
	  FFTW_ASSERT(!isnan(a));
#endif
     }
     return error;
}

static void fill_random(COMPLEX *a, int n)
{
     int i;

     /* generate random inputs */
     for (i = 0; i < n; ++i) {
	  RE(a[i]) = double_rand();
	  IM(a[i]) = double_rand();
     }
}

/* C = A + B */
static void aadd(COMPLEX *C, COMPLEX *A, COMPLEX *B, int n)
{
     int i;

     for (i = 0; i < n; ++i) {
	  RE(C[i]) = RE(A[i]) + RE(B[i]);
	  IM(C[i]) = IM(A[i]) + IM(B[i]);
     }
}

/* C = A - B */
static void asub(COMPLEX *C, COMPLEX *A, COMPLEX *B, int n)
{
     int i;

     for (i = 0; i < n; ++i) {
	  RE(C[i]) = RE(A[i]) - RE(B[i]);
	  IM(C[i]) = IM(A[i]) - IM(B[i]);
     }
}

/* B = rotate left A (complex) */
static void arol(COMPLEX *B, COMPLEX *A, int n, int stride)
{
     int i;

     for (i = 0; i < n - 1; ++i)
	  B[i * stride] = A[(i + 1) * stride];
     B[(n - 1) * stride] = A[0];
}

/* A = alpha * A  (complex, in place) */
static void ascale(COMPLEX *A, COMPLEX alpha, int n)
{
     int i;

     for (i = 0; i < n; ++i) {
	  COMPLEX a = A[i];
	  RE(A[i]) = RE(a) * RE(alpha) - IM(a) * IM(alpha);
	  IM(A[i]) = RE(a) * IM(alpha) + IM(a) * RE(alpha);
     }
}

static double acmp(COMPLEX *A, COMPLEX *B, int n) {
    double tol = TOLERANCE;
    double d = compute_error(A, B, n);
#ifdef EXIT_ON_FAIL
    if (d > tol) {
	printf("\n\nfailure in Ergun's verification procedure. Error: %e\n\n", d);
	exit(1);
    }
#endif
    return d;
}

/***************************************************************************/
/*
 * Implementation of the FFT tester described in
 *
 * Funda Ergün. Testing multivariate linear functions: Overcoming the
 * generator bottleneck. In Proceedings of the Twenty-Seventh Annual
 * ACM Symposium on the Theory of Computing, pages 407-416, Las Vegas,
 * Nevada, 29 May--1 June 1995.
 */

static double verify_linear(fftd_func *dft, 
			  int N,
			  COMPLEX *inA,
			  COMPLEX *inB,
			  COMPLEX *inC,
			  COMPLEX *outA,
			  COMPLEX *outB,
			  COMPLEX *outC,
			  COMPLEX *tmp,
			  int rounds)
{
    int i; double maxerr = 0.0; double e;

    /* test 1: check linearity */
    for (i = 0; i < rounds; ++i) {
	COMPLEX alpha, beta;
	RE(alpha) = double_rand();
	IM(alpha) = double_rand();
	RE(beta) = double_rand();
	IM(beta) = double_rand();
	fill_random(inA, N);
	fill_random(inB, N);
	dft((double *)outA, (double *)inA);
	dft((double *)outB, (double *)inB);

	ascale(outA, alpha, N);
	ascale(outB, beta, N);
	aadd(tmp, outA, outB, N);
	ascale(inA, alpha, N);
	ascale(inB, beta, N);
	aadd(inC, inA, inB, N);
	dft((double *)outC, (double *)inC);

	e = acmp(outC, tmp, N);
	if(e > maxerr) maxerr = e;
    }
    return maxerr;
}

static double verify_impulse(fftd_func *dft,
			    int n, int veclen,
			   COMPLEX *inA,
			   COMPLEX *inB,
			   COMPLEX *inC,
			   COMPLEX *outA,
			   COMPLEX *outB,
			   COMPLEX *outC,
			   COMPLEX *tmp,
			   int rounds)
{
     int N = n * veclen;
     COMPLEX impulse;
     int i;
     double e, maxerr = 0.0;

     /* test 2: check that the unit impulse is transformed properly */
     RE(impulse) = 1.0;
     IM(impulse) = 0.0;
     
     for (i = 0; i < N; ++i) {
	  /* impulse */
	  RE(inA[i]) = 0.0;
	  IM(inA[i]) = 0.0;
	  
	  /* transform of the impulse */
	  outA[i] = impulse;
     }
     for (i = 0; i < veclen; ++i)
	  inA[i * n] = impulse;

     /* a simple test first, to help with debugging: */
     dft((double *)outB, (double *)inA);
     e = acmp(outB, outA, N);
     if(e > maxerr) maxerr = e;

     for (i = 0; i < rounds; ++i) {
	  fill_random(inB, N);
	  asub(inC, inA, inB, N);
   	  dft((double *)outB, (double *)inB);
   	  dft((double *)outC, (double *)inC);
	  aadd(tmp, outB, outC, N);
	  e = acmp(tmp, outA, N);
	  if(e > maxerr) maxerr = e;
     }
     return maxerr;
}

static double verify_shift(fftd_func *dft,
			 int N, 
			 COMPLEX *inA,
			 COMPLEX *inB,
			 COMPLEX *outA,
			 COMPLEX *outB,
			 COMPLEX *tmp,
			 int rounds)
{
    const double twopin = 2 * M_PI / (double) N;
    int i, j;
    double e, maxerr = 0.0;

    /* test 3: check the time-shift property */
    /* the paper performs more tests, but this code should be fine too */

    for (i = 0; i < rounds; ++i) {
	  
	fill_random(inA, N);
	arol(inB, inA, N, 1);

	dft((double *)outA, (double *)inA);
	dft((double *)outB, (double *)inB);

	for (j = 0; j < N; ++j) {
	    double s = SIGN * sin(j * twopin);
	    double c = cos(j * twopin);
	    int index = j;
	    RE(tmp[index]) = RE(outB[index]) * c - IM(outB[index]) * s;
	    IM(tmp[index]) = RE(outB[index]) * s + IM(outB[index]) * c;
	}
	e = acmp(tmp, outA, N);
	if(e > maxerr) maxerr = e;
    }
    return maxerr;
}

/***************************************************************************/

void verify_fft(int N, fftd_func *dft)
{
     COMPLEX *inA, *inB, *inC, *outA, *outB, *outC;
     COMPLEX *tmp;
     int i, dim, n_before, n_after;
     int n, veclen, rounds = ROUNDS;
     
     inA = (COMPLEX *) malloc(N * sizeof(COMPLEX));
     inB = (COMPLEX *) malloc(N * sizeof(COMPLEX));
     inC = (COMPLEX *) malloc(N * sizeof(COMPLEX));
     outA = (COMPLEX *) malloc(N * sizeof(COMPLEX));
     outB = (COMPLEX *) malloc(N * sizeof(COMPLEX));
     outC = (COMPLEX *) malloc(N * sizeof(COMPLEX));
     tmp = (COMPLEX *) malloc(N * sizeof(COMPLEX));

	 printf("    linear: %.3e\n",
	    verify_linear(dft, N, inA, inB, inC, outA, outB, outC, tmp, rounds));
     printf("    impulse : %.3e,\n",
	    verify_impulse(dft, N, 1, inA, inB, inC, outA, outB, outC, tmp, rounds));
     printf("    shift : %.3e\n", 
	    verify_shift(dft, N, inA, inB, outA, outB, tmp, rounds));

     free(tmp);
     free(outC);
     free(outB);
     free(outA);
     free(inC);
     free(inB);
     free(inA);
}

