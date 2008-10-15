/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; fill-column: 79; coding: iso-latin-1-unix -*- */
/* mpifft.c
 */

#include <hpcc.h>

#include "hpccfft.h"
#include "wrapmpifftw.h"

double *HPCC_fft_timings_forward, *HPCC_fft_timings_backward;

static void
MPIFFT0(HPCC_Params *params, int doIO, FILE *outFile, MPI_Comm comm, int locN,
        double *UGflops, s64Int_t *Un, double *UmaxErr, int *Ufailure) {
  int commRank, commSize, failure, flags;
  s64Int_t i, n;
  s64Int_t locn, loc0, alocn, aloc0, tls;
  double maxErr, tmp1, tmp2, tmp3, t0, t1, t2, t3, Gflops;
  double deps;
  fftw_complex *inout, *work;
  fftw_mpi_plan p;
  hpcc_fftw_mpi_plan ip;

  failure = 1;
  Gflops = -1.0;
  deps = HPL_dlamch( HPL_MACH_EPS );

  MPI_Comm_size( comm, &commSize );
  MPI_Comm_rank( comm, &commRank );

  n = locN;

  /* number of processes have been factored out - need to put it back in */
  n *= commSize;

  n *= commSize; /* global vector size */

#ifdef HPCC_FFTW_ESTIMATE
  flags = FFTW_ESTIMATE;
#else
  flags = FFTW_MEASURE;
#endif

  t1 = -MPI_Wtime();
  p = fftw_mpi_create_plan( comm, n, FFTW_FORWARD, flags );
  t1 += MPI_Wtime();

  if (! p) goto no_plan;

  fftw_mpi_local_sizes( p, &locn, &loc0, &alocn, &aloc0, &tls );

  inout = (fftw_complex *)HPCC_fftw_malloc( tls * (sizeof *inout) );
  work  = (fftw_complex *)HPCC_fftw_malloc( tls * (sizeof *work) );

  if (! inout || ! work) {
    fftw_mpi_destroy_plan( p );
    goto comp_end;
  }

  /* Make sure that `inout' and `work' are initialized in parallel if using
     Open MP: this will ensure better placement of pages if first-touch policy
     is used by a distrubuted shared memory machine. */
#ifdef _OPENMP
#pragma omp parallel for
  for (i = 0; i < tls; ++i) {
    c_re( inout[i] ) = c_re( work[i] ) = 0.0;
    c_re( inout[i] ) = c_im( work[i] ) = 0.0;
  }
#endif

  t0 = -MPI_Wtime();
  HPCC_bcnrand( 2 * tls, 53 * commRank * 2 * tls, inout );
  t0 += MPI_Wtime();

  t2 = -MPI_Wtime();
  fftw_mpi( p, 1, inout, work );
  t2 += MPI_Wtime();

  fftw_mpi_destroy_plan( p );

  ip = HPCC_fftw_mpi_create_plan( comm, n, FFTW_BACKWARD, FFTW_ESTIMATE );

  if (ip) {
    t3 = -MPI_Wtime();
    HPCC_fftw_mpi( ip, 1, inout, work );
    t3 += MPI_Wtime();

    HPCC_fftw_mpi_destroy_plan( ip );
  }

  HPCC_bcnrand( 2 * tls, 53 * commRank * 2 * tls, work ); /* regenerate data */

  maxErr = 0.0;
  for (i = 0; i < tls; ++i) {
    tmp1 = c_re( inout[i] ) - c_re( work[i] );
    tmp2 = c_im( inout[i] ) - c_im( work[i] );
    tmp3 = sqrt( tmp1*tmp1 + tmp2*tmp2 );
    maxErr = maxErr >= tmp3 ? maxErr : tmp3;
  }
  if (maxErr / log(n) / deps < params->test.thrsh) failure = 0;

  if (t2 > 0.0) Gflops = 1e-9 * (5.0 * n * log(n) / log(2.0)) / t2;

  if (doIO) {
    fprintf( outFile, "Number of nodes: %d\n", commSize );
    fprintf( outFile, "Vector size: %20.0f\n", tmp1 = (double)n );
    fprintf( outFile, "Generation time: %9.3f\n", t0 );
    fprintf( outFile, "Tuning: %9.3f\n", t1 );
    fprintf( outFile, "Computing: %9.3f\n", t2 );
    fprintf( outFile, "Inverse FFT: %9.3f\n", t3 );
    fprintf( outFile, "max(|x-x0|): %9.3e\n", maxErr );
    fprintf( outFile, "Gflop/s: %9.3f\n", Gflops );
  }

  comp_end:

  if (work) HPCC_fftw_free( work );
  if (inout) HPCC_fftw_free( inout );

  no_plan:

  *UGflops = Gflops;
  *Un = n;
  *UmaxErr = maxErr;
  *Ufailure = failure;
}

int
HPCC_MPIFFT(HPCC_Params *params) {
  int commRank, commSize;
  int locN, procCnt, isComputing, doIO, failure;
  s64Int_t n;
  double Gflops = -1.0, maxErr = -1.0;
  MPI_Comm comm;
  FILE *outFile;

  MPI_Comm_size( MPI_COMM_WORLD, &commSize );
  MPI_Comm_rank( MPI_COMM_WORLD, &commRank );

  doIO = commRank == 0 ? 1 : 0;

  if (doIO) {
    outFile = fopen( params->outFname, "a" );
    if (! outFile) outFile = stderr;
  }

  /*
  There are two vectors of size 'n'/'commSize': inout, work,
  and internal work: 2*'n'/'commSize'; it's 4 vectors then.

  FFTE requires that the global vector size 'n' has to be at least
  as big as square of number of processes. The square is calculated
  in each factor independently. In other words, 'n' has to have as
  at least twice as many 2 factors as the process count, twice as many
  3 factors and 5 factors.
  */

#ifdef HPCC_FFT_235
  locN = 0; procCnt = commSize + 1;
  do {
    int f[3];

    procCnt--;

    for ( ; procCnt > 1 && HPCC_factor235( procCnt, f ); procCnt--)
      ; /* EMPTY */

    /* Make sure the local vector size is greater than 0 */
    locN = HPCC_LocalVectorSize( params, 4*procCnt, sizeof(fftw_complex), 0 );
    for ( ; locN >= 1 && HPCC_factor235( locN, f ); locN--)
      ; /* EMPTY */
  } while (locN < 1);
#else
  /* Find power of two that is smaller or equal to number of processes */
  for (procCnt = 1; procCnt <= (commSize >> 1); procCnt <<= 1)
    ; /* EMPTY */

  /* Make sure the local vector size is greater than 0 */
  while (1) {
    locN = HPCC_LocalVectorSize( params, 4*procCnt, sizeof(fftw_complex), 1 );
    if (locN) break;
    procCnt >>= 1;
  }
#endif

  isComputing = commRank < procCnt ? 1 : 0;

  HPCC_fft_timings_forward = params->MPIFFTtimingsForward;
  HPCC_fft_timings_backward = params->MPIFFTtimingsBackward;

  if (commSize == procCnt)
    comm = MPI_COMM_WORLD;
  else
    MPI_Comm_split( MPI_COMM_WORLD, isComputing ? 0 : MPI_UNDEFINED, commRank, &comm );

  if (isComputing)
    MPIFFT0( params, doIO, outFile, comm, locN, &Gflops, &n, &maxErr, &failure );

  if (commSize != procCnt && isComputing && comm != MPI_COMM_NULL)
    MPI_Comm_free( &comm );

  params->MPIFFT_N = n;
  params->MPIFFT_Procs = procCnt;
  params->MPIFFT_maxErr = maxErr;

  MPI_Bcast( &Gflops, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );

  params->MPIFFTGflops = Gflops;

  params->FFTEnblk = FFTE_NBLK;
  params->FFTEnp = FFTE_NP;
  params->FFTEl2size = FFTE_L2SIZE;

  if (doIO) if (outFile != stderr) fclose( outFile );

  return 0;
}
