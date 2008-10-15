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
C     PARALLEL 1-D COMPLEX FFT ROUTINE
C
C     FORTRAN77 + MPI SOURCE PROGRAM
C
C     CALL PZFFT1D(A,B,W,N,ICOMM,ME,NPU,IOPT)
C
C     W((N/NPU)*3/2) IS SINE/COSINE TABLE (COMPLEX*16)
C     N IS THE LENGTH OF THE TRANSFORMS (INTEGER*8)
C       -----------------------------------
C         N = (2**IP) * (3**IQ) * (5**IR)
C       -----------------------------------
C     ICOMM IS THE COMMUNICATOR (INTEGER*4)
C     ME IS THE RANK (INTEGER*4)
C     NPU IS THE NUMBER OF PROCESSORS (INTEGER*4)
C     IOPT = 0 FOR INITIALIZING THE COEFFICIENTS (INTEGER*4)
C     IOPT = -1 FOR FORWARD TRANSFORM WHERE
C              A(N/NPU) IS COMPLEX INPUT VECTOR (COMPLEX*16)
C!HPF$ DISTRIBUTE A(BLOCK)
C              B(N/NPU) IS COMPLEX OUTPUT VECTOR (COMPLEX*16)
C!HPF$ DISTRIBUTE B(BLOCK)
C     IOPT = +1 FOR INVERSE TRANSFORM WHERE
C              A(N/NPU) IS COMPLEX INPUT VECTOR (COMPLEX*16)
C!HPF$ DISTRIBUTE A(BLOCK)
C              B(N/NPU) IS COMPLEX OUTPUT VECTOR (COMPLEX*16)
C!HPF$ DISTRIBUTE B(BLOCK)
C     IOPT = -2 FOR FORWARD TRANSFORM WHERE
C              A(N/NPU) IS COMPLEX INPUT VECTOR (COMPLEX*16)
C!HPF$ DISTRIBUTE A(BLOCK)
C              B(N/NPU) IS COMPLEX OUTPUT VECTOR (COMPLEX*16)
C!HPF$ DISTRIBUTE B(CYCLIC)
C     IOPT = +2 FOR INVERSE TRANSFORM WHERE
C              A(N/NPU) IS COMPLEX INPUT VECTOR (COMPLEX*16)
C!HPF$ DISTRIBUTE A(CYCLIC)
C              B(N/NPU) IS COMPLEX OUTPUT VECTOR (COMPLEX*16)
C!HPF$ DISTRIBUTE B(BLOCK)
C
C     WRITTEN BY DAISUKE TAKAHASHI
C
*/

#include "hpccfft.h"
#include "wrapmpifftw.h"

static void
ztrans(fftw_complex *a, fftw_complex *b, int n1, int n2) {
  int ii, jj, i, j;
  int tmin1, tmin2;
  int lda, ldb;

  lda = n1;
  ldb = n2;

#ifdef _OPENMP
#pragma omp parallel for private(i,j,jj,tmin1,tmin2)
#endif
  for (ii = 0; ii < n1; ii += FFTE_NBLK)
    for (jj = 0; jj < n2; jj += FFTE_NBLK) {

      V3MIN( tmin1, ii + FFTE_NBLK, n1 );
      for (i = ii; i < tmin1; ++i) {

        V3MIN( tmin2, jj + FFTE_NBLK, n2 );
        for (j = jj; j < tmin2; ++j) {
          c_assgn( ARR2D( b, j, i, ldb ), ARR2D( a, i, j, lda ) );
        }
      }
    }
}	/* ztrans */

static void
pztrans(fftw_complex *a, fftw_complex *b, s64Int_t nn, hpcc_fftw_mpi_plan p, int npu) {
  int i, nn2;

  nn2 = nn / npu;

  if (1 == npu)
    for (i = 0; i < nn2; ++i) {
      c_assgn( b[i], a[i] );
    }
  else
    MPI_Alltoall( a, nn2, p->cmplx, b, nn2, p->cmplx, p->comm );
}	/* pztrans */

static void
pzfft1d0(fftw_complex *a, fftw_complex *a2, fftw_complex *apxyz, fftw_complex *axyzp, fftw_complex *b,
  fftw_complex *bxyzp, fftw_complex *bzyx, fftw_complex *cy, fftw_complex *cz, fftw_complex *d,
  fftw_complex *wx, fftw_complex *wy, fftw_complex *wz, fftw_complex *ww, fftw_complex *www,
  int nx, int ny, int nz, hpcc_fftw_mpi_plan p, int npu, const int *lnx, const int *lny, const int *lnz) {

  int i, j, k, l, ii, jj, kk;
  int tmin1, tmin2, tmin3;
  int nnx, nnz;
  s64Int_t nn;
  int ldcz, lda2_1, lda2_2, ldaxyzp1, ldaxyzp2, ldaxyzp3, ldbxyzp1, ldbxyzp2, ldbxyzp3, ldww, ldcy;
  int ldwww1, ldwww2, ldwww3, ldapxyz1, ldapxyz2, ldapxyz3, ldbzyx1, ldbzyx2, lda1, lda2;
  fftw_complex ztmp1, ztmp2, ztmp3;

  ldcz = nz + FFTE_NP;
  lda2_1 = nx / npu;
  lda2_2 = ny;
  ldaxyzp1 = nx / npu;
  ldaxyzp2 = ny;
  ldaxyzp3 = nz / npu;
  ldbxyzp1 = nx / npu;
  ldbxyzp2 = ny;
  ldbxyzp3 = nz / npu;
  ldww = ny;
  ldcy = ny + FFTE_NP;
  ldwww1 = npu;
  ldwww2 = nx / npu;
  ldwww3 = ny;
  ldapxyz1 = npu;
  ldapxyz2 = nx / npu;
  ldapxyz3 = ny;
  ldbzyx1 = nz / npu;
  ldbzyx2 = ny;
  lda1 = nx;
  lda2 = ny;

  nnx = nx / npu;
  nnz = nz / npu;
  nn = (s64Int_t)nx * ny * nz / npu;

#ifdef _OPENMP
#pragma omp for private(i,k,l,ii,kk,tmin1,tmin2)
#endif
  for (j = 0; j < ny; ++j) {
    for (ii = 0; ii < nnx; ii += FFTE_NBLK) {
      for (kk = 0; kk < nz; kk += FFTE_NBLK) {

        V3MIN( tmin1, ii + FFTE_NBLK, nnx );
        for (i = ii; i < tmin1; ++i) {

          V3MIN( tmin2, kk + FFTE_NBLK, nz );
          for (k = kk; k < tmin2; ++k) {
            c_assgn( ARR2D( cz, k, i-ii, ldcz ), ARR3D( a2, i, j, k, lda2_1, lda2_2 ) );
          }
        }
      }

      V3MIN( tmin2, ii + FFTE_NBLK, nnx );
      for (i = ii; i < tmin2; ++i)
        HPCC_fft235( PTR2D( cz, 0, i-ii, ldcz ), d, wz, nz, lnz );

      for (l = 0; l < npu; ++l) {
        for (k = 0; k < nnz; ++k) {

          /* reusing tmin2 from above */
          for (i = ii; i < tmin2; ++i) {
            c_assgn( ARR4D( axyzp, i, j, k, l, ldaxyzp1, ldaxyzp2, ldaxyzp3 ),
                     ARR2D( cz, l + k*npu, i-ii, ldcz ) );
          }
        }
      }
    }
  }

#ifdef _OPENMP
#pragma omp single
  {
#endif
  p->timings[3] = MPI_Wtime();

  pztrans( a, b, nn, p, npu );

  p->timings[4] = MPI_Wtime();
#ifdef _OPENMP
  }
#endif

#ifdef _OPENMP
#pragma omp for private(i,j,l,ii,jj,kk,tmin1,tmin2)
#endif
  for (k = 0; k < nnz; ++k) {
    for (l = 0; l < npu; ++l) {
      for (ii = 0; ii < nnx; ii += FFTE_NBLK) {
        for (jj = 0; jj < ny; jj += FFTE_NBLK) {

          V3MIN( tmin1, ii + FFTE_NBLK, nnx );
          for (i = ii; i < tmin1; ++i) {

            V3MIN( tmin2, jj + FFTE_NBLK, ny );
            for (j = jj; j < tmin2; ++j) {
              c_assgn( ztmp1, ARR4D( bxyzp, i, j, k, l, ldbxyzp1, ldbxyzp2, ldbxyzp3 ) );
              c_assgn( ztmp2, ARR2D( ww, j, k, ldww ) );
              c_mul3v(ztmp3, ztmp1, ztmp2);
              c_assgn( ARR2D( cy, j, i-ii, ldcy ), ztmp3 );
            }
          }
        }

        V3MIN( tmin1, ii + FFTE_NBLK, nnx );
        for (i = ii; i < tmin1; ++i)
          HPCC_fft235( PTR2D( cy, 0, i-ii, ldcy ), d, wy, ny, lny );

        for (j = 0; j < ny; ++j) {
        V3MIN( tmin1, ii + FFTE_NBLK, nnx );
          for (i = ii; i < tmin1; ++i) {
            c_assgn( ztmp1, ARR2D( cy, j, i-ii, ldcy ) );
            c_assgn( ztmp2, ARR4D( www, l, i, j, k, ldwww1, ldwww2, ldwww3 ) );
            c_mul3v(ztmp3, ztmp1, ztmp2);
            c_assgn( ARR4D( apxyz, l, i, j, k, ldapxyz1, ldapxyz2, ldapxyz3 ), ztmp3 );
          }
        }
      }
    }

    for (j = 0; j < ny; ++j)
      HPCC_fft235( PTR3D( a, 0, j, k, lda1, lda2 ), d, wx, nx, lnx );
  }

#ifdef _OPENMP
#pragma omp for private(i,j,k,jj,kk,tmin1,tmin2,tmin3)
#endif
  for (ii = 0; ii < nx; ii += FFTE_NBLK) {
    for (jj = 0; jj < ny; jj += FFTE_NBLK) {
      for (kk = 0; kk < nnz; kk += FFTE_NBLK) {

        V3MIN( tmin1, ii + FFTE_NBLK, nx );
        for (i = ii; i < tmin1; ++i) {

          V3MIN( tmin2, jj + FFTE_NBLK, ny );
          for (j = jj; j < tmin2; ++j) {

            V3MIN( tmin3, kk + FFTE_NBLK, nnz );
            for (k = kk; k < tmin3; ++k) {
              c_assgn( ARR3D( bzyx, k, j, i, ldbzyx1, ldbzyx2 ), ARR3D( a, i, j, k, lda1, lda2 ) );
            }
          }
        }
      }
    }
  }
}	/* pzfft1d0 */

static void
psettbl2(fftw_complex *w, int ny, int nz, int me, int npu) {
  int j, k;
  int ldw;
  double pi2, px;
  int tmin1;

  ldw = ny;

  pi2 = 8.0 * atan(1.0);
  px = -pi2 / ((double)ny * nz);

  tmin1 = nz / npu;
#ifdef _OPENMP
#pragma omp parallel for private(j)
#endif
  for (k = 0; k < tmin1; ++k)
    for (j = 0; j < ny; ++j) {
      c_re( ARR2D( w, j, k, ldw ) ) = cos(px * j * (me + (double)k * npu));
      c_im( ARR2D( w, j, k, ldw ) ) = sin(px * j * (me + (double)k * npu));
    }
}	/* psettbl2 */

static void
psettbl3(fftw_complex *w, int nx, int ny, int nz, int me, int npu) {
  int i, j, k;
  int ldw1, ldw2;
  int tmin1;
  double pi2, px;

  ldw1 = nx;
  ldw2 = ny;

  pi2 = 8.0 * atan(1.0);
  px = -pi2 / ((double)nx * ny * nz);

  tmin1 = nz / npu;
#ifdef _OPENMP
#pragma omp parallel for private(i,j)
#endif
  for (k = 0; k < tmin1; ++k)
    for (j = 0; j < ny; ++j)
      for (i = 0; i < nx; ++i) {
        c_re( ARR3D( w, i, j, k, ldw1, ldw2 ) ) = cos( px * i * (me + (double)k * npu + (double)j * nz));
        c_im( ARR3D( w, i, j, k, ldw1, ldw2 ) ) = sin( px * i * (me + (double)k * npu + (double)j * nz));
      }
}	/* psettbl3 */

int
HPCC_pzfft1d(s64Int_t n, fftw_complex *a, fftw_complex *b, fftw_complex *w, int me, int npu, int iopt,
  hpcc_fftw_mpi_plan p) {

  int ip[3], lnx[3], lny[3], lnz[3], lnpu[3];
  s64Int_t nn;
  int i, inn, nn2, nd, nx, ny, nz;
  fftw_complex *wx, *wy, *wz, *c;
  double dn;

  p->timings[0] = MPI_Wtime();

  wx = p->wx;
  wy = p->wy;
  wz = p->wz;
  c = p->c;

  nn = n / npu; inn = (int)nn;
  nn2 = nn / npu;

  HPCC_factor235( npu, lnpu );
  HPCC_factor235_8( n, ip );

  for (i = 0; i < 3; ++i) {
    EMAX( lnz[i], lnpu[i], (ip[i]+1)/3 );
    EMAX( lnx[i], lnpu[i], (ip[i]-lnz[i]+1)/2 );
    lny[i] = ip[i] - lnx[i] - lnz[i];
  }

  nx = HPCC_ipow( 2, lnx[0] ) * HPCC_ipow( 3, lnx[1] ) * HPCC_ipow( 5, lnx[2] );
  ny = HPCC_ipow( 2, lny[0] ) * HPCC_ipow( 3, lny[1] ) * HPCC_ipow( 5, lny[2] );
  nz = HPCC_ipow( 2, lnz[0] ) * HPCC_ipow( 3, lnz[1] ) * HPCC_ipow( 5, lnz[2] );

  if (0 == iopt) {
    HPCC_settbl( wx, nx );
    HPCC_settbl( wy, ny );
    HPCC_settbl( wz, nz );
    psettbl2( w, ny, nz, me, npu );
    psettbl3( w + ny * (nz / npu), nx, ny, nz, me, npu );
    return 0;
  }

  if (1 == iopt || 2 == iopt) {
    for (i = 0; i < inn; ++i) {
      c_im( a[i] ) = -c_im( a[i] );
    }
  }

  p->timings[1] = MPI_Wtime();

  if (-1 == iopt || 1 == iopt || -2 == iopt) {
    ztrans( a, b, npu, nn2 );
    pztrans( b, a, nn, p, npu );
  }

  p->timings[2] = MPI_Wtime();

  nd = ((ny > nz ? ny : nz) + FFTE_NP) * FFTE_NBLK + FFTE_NP;

#ifdef _OPENMP
#pragma omp parallel private(c,i)
   {
    i = omp_get_thread_num();
    c = p->c + i*p->c_size;
#endif

  pzfft1d0( a, a, a, a, b, b, b, c, c, c + nd, wx, wy, wz, w, w + ny*(nz/npu), nx, ny, nz, p, npu, lnx, lny, lnz );

#ifdef _OPENMP
   }
#endif

  p->timings[5] = MPI_Wtime();

  if (-1 == iopt || 1 == iopt || 2 == iopt) {
    pztrans( b, a, nn, p, npu );
    ztrans( a, b, nn2, npu );
  }

  p->timings[6] = MPI_Wtime();

  if (1 == iopt || 2 == iopt) {
    dn = 1.0 / n;
    for (i = 0; i < inn; ++i) {
      c_re( b[i] ) *= dn;
      c_im( b[i] ) *= -dn;
    }
  }

  p->timings[7] = MPI_Wtime();

  return 0;
}	/* HPCC_pzfft1d */
