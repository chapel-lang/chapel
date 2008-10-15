
#include <stdio.h>
#include <stdlib.h>

#include <hpccmema.h>

#include "hpccfft.h"
#include "wrapmpifftw.h"

#define    Mmax3( a_, b_, c_ )      ( (a_) > (b_) ?  ((a_) > (c_) ? (a_) : (c_)) : ((b_) > (c_) ? (b_) : (c_)) )

static int
GetNXYZ(s64Int_t n, int npu) {
  int ip[3], lnx[3], lny[3], lnz[3], lnpu[3];
  int i, nx, ny, nz, nxyz;

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

  nxyz = Mmax3( nx, ny, nz );

  return nxyz;
}

hpcc_fftw_mpi_plan
HPCC_fftw_mpi_create_plan(MPI_Comm comm, s64Int_t n, fftw_direction dir, int flags) {
  hpcc_fftw_mpi_plan p;
  fftw_complex *a = NULL, *b = NULL;
  int nxyz;
  int rank, size;

  MPI_Comm_size( comm, &size );
  MPI_Comm_rank( comm, &rank );

  p = (hpcc_fftw_mpi_plan)fftw_malloc( sizeof *p );
  if (! p) return p;

  nxyz = GetNXYZ( n, size );

  p->wx = (fftw_complex *)HPCC_fftw_malloc( (nxyz/2 + FFTE_NP) * (sizeof *p->wx) );
  p->wy = (fftw_complex *)HPCC_fftw_malloc( (nxyz/2 + FFTE_NP) * (sizeof *p->wy) );
  p->wz = (fftw_complex *)HPCC_fftw_malloc( (nxyz/2 + FFTE_NP) * (sizeof *p->wz) );
  p->work = (fftw_complex *)HPCC_fftw_malloc( n / size * 3 / 2 * (sizeof *p->work) );

  p->c_size = (nxyz+FFTE_NP) * (FFTE_NBLK + 1) + FFTE_NP;
#ifdef _OPENMP
#pragma omp parallel
  {
#pragma omp single
    {
      int i;
      i = omp_get_num_threads();
      p->c = (fftw_complex *)HPCC_fftw_malloc( p->c_size * (sizeof *p->c) * i );
    }
  }
#else
  p->c = (fftw_complex *)HPCC_fftw_malloc( p->c_size * (sizeof *p->c) );
#endif

  if (! p->wx || ! p->wy || ! p->wz || ! p->work || ! p->c) {
    if (p->c) HPCC_fftw_free( p->c );
    if (p->work) HPCC_fftw_free( p->work );
    if (p->wz) HPCC_fftw_free( p->wz );
    if (p->wy) HPCC_fftw_free( p->wy );
    if (p->wx) HPCC_fftw_free( p->wx );
    fftw_free( p );
    return NULL;
  }

  p->n = n;
  p->comm = comm;
  p->dir = dir;
  p->flags = flags;

  MPI_Type_contiguous( 2, MPI_DOUBLE, &p->cmplx );
  MPI_Type_commit( &p->cmplx );

  if (FFTW_FORWARD == p->dir)
    p->timings = HPCC_fft_timings_forward;
  else
    p->timings = HPCC_fft_timings_backward;

  HPCC_pzfft1d( n, a, b, p->work, rank, size, 0, p );

  return p;
}

void
HPCC_fftw_mpi_destroy_plan(hpcc_fftw_mpi_plan p) {
  if (!p) return;

  MPI_Type_free( &p->cmplx );

  HPCC_fftw_free( p->work );
  HPCC_fftw_free( p->c );
  HPCC_fftw_free( p->wz );
  HPCC_fftw_free( p->wy );
  HPCC_fftw_free( p->wx );
  fftw_free( p );
}

void
HPCC_fftw_mpi(hpcc_fftw_mpi_plan p, int n_fields, fftw_complex *local_data, fftw_complex *work){
  int rank, size;
  s64Int_t n;
  int i, ln;

  MPI_Comm_size( p->comm, &size );
  MPI_Comm_rank( p->comm, &rank );

  n = p->n;

  if (FFTW_FORWARD == p->dir)
    HPCC_pzfft1d( n, local_data, work, p->work, rank, size, -1, p );
  else
    HPCC_pzfft1d( n, local_data, work, p->work, rank, size, +1, p );

  ln = n / size;
  for (i = 0; i < ln; ++i) {
    c_assgn( local_data[i], work[i] );
  }
}

void
HPCC_fftw_mpi_local_sizes(hpcc_fftw_mpi_plan p, s64Int_t *local_n, s64Int_t *local_start,
  s64Int_t *local_n_after_transform, s64Int_t *local_start_after_transform, s64Int_t *total_local_size) {
  int rank, size;
  s64Int_t n;
  MPI_Comm_size( p->comm, &size );
  MPI_Comm_rank( p->comm, &rank );
  n = p->n;
  if (local_n) *local_n = n / size;
  if (local_start) *local_start = n / size * rank;
  if (local_n_after_transform) *local_n_after_transform = n / size;
  if (local_start_after_transform) *local_start_after_transform = n / size * rank;
  if (total_local_size) *total_local_size = n / size;
}
