/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; fill-column: 79; coding: iso-latin-1-unix -*- */
/*
  hpcc.c
*/

#include <hpcc.h>

#include <ctype.h>
#include <string.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

static double HPCC_MemProc = -1.0, HPCC_MemVal = -1.0;
static int HPCC_MemSpec = -1;

static int
ReadInts(char *buf, int n, int *val) {
  int i, j;

  for (j = i = 0; i < n; i++) {
    if (sscanf( buf + j, "%d", val + i ) != 1) {
      i--;
      break;
    }
    for (; buf[j] && isdigit(buf[j]); j++)
      ; /* EMPTY */
    for (; buf[j] && ! isdigit(buf[j]); j++)
      ; /* EMPTY */
    if (! buf[j]) {
      i--;
      break;
    }
  }

  return i + 1;
}

static int
HPCC_InitHPL(HPCC_Params *p) {
  HPL_pdinfo( &p->test, &p->ns, p->nval, &p->nbs, p->nbval, &p->porder, &p->npqs, p->pval,
              p->qval, &p->npfs, p->pfaval, &p->nbms, p->nbmval, &p->ndvs, p->ndvval, &p->nrfs,
              p->rfaval, &p->ntps, p->topval, &p->ndhs, p->ndhval, &p->fswap, &p->tswap,
              &p->L1notran, &p->Unotran, &p->equil, &p->align );

  if (p->test.thrsh <= 0.0) p->Failure = 1;

  return 0;
}

static int
iiamax(int n, int *x, int incx) {
  int i, v, mx, idx = 0;

  idx = 0;
  mx = (x[0] < 0 ? -x[0] : x[0]);
  for (i = 0; i < n; i++, x += incx) {
    v = (x[i] < 0 ? -x[i] : x[i]);
    if (mx < v) {mx = v; idx = i;}
  }

  return idx;
}

static void
icopy(int n, int *src, int sinc, int *dst, int dinc) {
  int i;

  for (i = n; i; i--) {
    *dst = *src;
    dst += dinc;
    src += sinc;
  }
}

int
HPCC_InputFileInit(HPCC_Params *params) {
  int myRank, commSize;
  int i, j, n, ioErr, lastConfigLine = 32, line, rv, maxHPLn;
  char buf[82]; int nbuf = 82;
  FILE *f, *outputFile;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Comm_size( comm, &commSize );
  MPI_Comm_rank( comm, &myRank );

  if (0 == myRank) {
    f = fopen( params->inFname, "r" );
    if (! f) {
      ioErr = 1;
      goto ioEnd;
    }

    /* skip irrelevant lines in config file */
            for (line = 0; line < lastConfigLine; line++)
              if (! fgets( buf, nbuf, f )) break;

            if (line < lastConfigLine) { /* if didn't read all the required lines */
      ioErr = 1;
      goto ioEnd;
    }

    /* Get values of N for PTRANS */
    line++;
    fgets( buf, nbuf, f );
    rv = sscanf( buf, "%d", &n );
    if (rv != 1 || n < 0) { /* parse error or negative value*/
      n = 0;
      BEGIN_IO(myRank, params->outFname, outputFile);
      FPRINTF( myRank, outputFile, "Error in line %d of the input file.", line );
      END_IO(  myRank, outputFile );
    }
    n = Mmin( n, HPL_MAX_PARAM );

    line++;
    fgets( buf, nbuf, f );
    ReadInts( buf, n, params->PTRANSnval );

    /* find the largest matrix for HPL */
    maxHPLn = params->nval[iiamax( params->ns, params->nval, 1 )];

    for (j = i = 0; i < n; i++) {
      /* if memory for PTRANS is at least 90% of what's used for HPL */
      if (params->PTRANSnval[i] >= 0.9486 * maxHPLn * 0.5) {
        params->PTRANSnval[j] = params->PTRANSnval[i];
        j++;
      }
    }
    n = j; /* only this many entries use enough memory */

    /* copy matrix sizes from HPL, divide by 2 so both PTRANS matrices (plus "work" arrays) occupy
       as much as HPL's one */
    for (i = 0; i < params->ns; i++)
      params->PTRANSnval[i + n] = params->nval[i] / 2;
    params->PTRANSns = n + params->ns;

    /* Get values of block sizes */
    line++;
    fgets( buf, nbuf, f );
    rv = sscanf( buf, "%d", &n );
    if (rv != 1 || n < 0) { /* parse error or negative value*/
      n = 0;
      BEGIN_IO(myRank, params->outFname, outputFile);
      FPRINTF( myRank, outputFile, "Error in line %d of the input file.", line );
      END_IO(  myRank, outputFile );
    }
    n = Mmin( n, HPL_MAX_PARAM );

    line++;
    fgets( buf, nbuf, f );
    ReadInts( buf, n, params->PTRANSnbval );

    icopy( params->nbs, params->nbval, 1, params->PTRANSnbval + n, 1 );
    params->PTRANSnbs = n + params->nbs;

    ioErr = 0;
    ioEnd:
    if (f) fclose( f );
  }

  MPI_Bcast( &ioErr, 1, MPI_INT, 0, comm );
  if (ioErr) {
    /* copy matrix sizes from HPL, divide by 2 so both PTRANS matrices (plus "work" arrays) occupy
       as much as HPL's one */
    for (i = 0; i < params->ns; i++)
      params->PTRANSnval[i] = params->nval[i] / 2;
    params->PTRANSns = params->ns;

    icopy( params->nbs, params->nbval, 1, params->PTRANSnbval, 1 );
    params->PTRANSnbs = params->nbs;
  }

  /* broadcast what's been read on node 0 */
  MPI_Bcast( &params->PTRANSns, 1, MPI_INT, 0, comm );
  if (params->PTRANSns > 0)
    MPI_Bcast( &params->PTRANSnval, params->PTRANSns, MPI_INT, 0, comm );
  MPI_Bcast( &params->PTRANSnbs, 1, MPI_INT, 0, comm );
  if (params->PTRANSnbs > 0)
    MPI_Bcast( &params->PTRANSnbval, params->PTRANSnbs, MPI_INT, 0, comm );

  /* copy what HPL has */
  params->PTRANSnpqs = params->npqs;
  icopy( params->npqs, params->qval, 1, params->PTRANSqval, 1 );
  icopy( params->npqs, params->pval, 1, params->PTRANSpval, 1 );

  return ioErr;
}

static int
ErrorReduce(FILE *f, char *str, int eCode, MPI_Comm comm) {
  int rCode;

  if (eCode) eCode = 1; /* make sure error is indicated with 1 */

  MPI_Allreduce( &eCode, &rCode, 1, MPI_INT, MPI_SUM, comm );

  if (rCode) {
    if (f)
      fprintf( f, str );

    return -1;
  }

  return 0;
}

int
HPCC_Init(HPCC_Params *params) {
  int myRank, commSize;
  int i, nMax, procCur, procMax, procMin, errCode;
  double totalMem;
  char inFname[12] = "hpccinf.txt", outFname[13] = "hpccoutf.txt";
  FILE *outputFile;
  MPI_Comm comm = MPI_COMM_WORLD;
  time_t currentTime;
  char hostname[MPI_MAX_PROCESSOR_NAME + 1]; int hostnameLen;

  outputFile = NULL;

  MPI_Comm_size( comm, &commSize );
  MPI_Comm_rank( comm, &myRank );

  strcpy( params->inFname, inFname );
  strcpy( params->outFname, outFname );

  if (0 == myRank)
    outputFile = fopen( params->outFname, "a" );

  errCode = 0;
  if (sizeof(u64Int) < 8 || sizeof(s64Int) < 8) errCode = 1;
  if (ErrorReduce( outputFile, "No 64-bit integer type available.", errCode, comm ))
    return -1;

  i = MPI_Get_processor_name( hostname, &hostnameLen );
  if (i) hostname[0] = 0;
  else hostname[Mmax(hostnameLen, MPI_MAX_PROCESSOR_NAME)] = 0;
  time( &currentTime );

  BEGIN_IO(myRank, params->outFname, outputFile);
  FPRINTF( myRank, outputFile,
            "########################################################################%s", "" );
  FPRINTF3( myRank, outputFile,
            "This is the DARPA/DOE HPC Challange Benchmark version %d.%d.%d October 2003",
            HPCC_VERSION_MAJOR, HPCC_VERSION_MINOR, HPCC_VERSION_MICRO );
  FPRINTF( myRank, outputFile, "Produced by Jack Dongarra and Piotr Luszczek%s", "" );
  FPRINTF( myRank, outputFile, "Innovative Computing Laboratory%s", "" );
  FPRINTF( myRank, outputFile, "University of Tennessee Knoxville and Oak Ridge National Laboratory%s", "" );
  FPRINTF( myRank, outputFile, "%s", "" );
  FPRINTF( myRank, outputFile, "See the source files for authors of specific codes. %s", "" );
  FPRINTF( myRank, outputFile, "Compiled on " __DATE__ " at %s", __TIME__ );
  FPRINTF2(myRank, outputFile, "Current time (%ld) is %s",(long)currentTime,ctime(&currentTime));
  FPRINTF( myRank, outputFile, "Hostname: '%s'", hostname );
  FPRINTF( myRank, outputFile,
            "########################################################################%s", "" );
  END_IO(  myRank, outputFile );

  params->Failure = 0;

  HPCC_InitHPL( params ); /* HPL calls exit() if there is a problem */
  HPCC_InputFileInit( params );

  params->RunHPL = 0;
  params->RunStarDGEMM = 0;
  params->RunSingleDGEMM = 0;
  params->RunPTRANS = 0;
  params->RunStarStream = 0;
  params->RunSingleStream = 0;
  params->RunMPIRandomAccess = 0;
  params->RunStarRandomAccess = 0;
  params->RunSingleRandomAccess = 0;
  params->RunLatencyBandwidth = 0;
  params->RunMPIFFT = 0;
  params->RunHPL = params->RunStarDGEMM = params->RunSingleDGEMM =
  params->RunPTRANS = params->RunStarStream = params->RunSingleStream =
  params->RunMPIRandomAccess = params->RunStarRandomAccess = params->RunSingleRandomAccess = 
  params->RunMPIFFT = params->RunStarFFT = params->RunSingleFFT = 
  params->RunLatencyBandwidth = 1;

  params->MPIGUPs = params->StarGUPs = params->SingleGUPs =
  params->StarDGEMMGflops = params->SingleDGEMMGflops = -1.0;
  params->StarStreamCopyGBs = params->StarStreamScaleGBs = params->StarStreamAddGBs =
  params->StarStreamTriadGBs = params->SingleStreamCopyGBs = params->SingleStreamScaleGBs =
  params->SingleStreamAddGBs = params->SingleStreamTriadGBs =
  params->SingleFFTGflops = params->StarFFTGflops = params->MPIFFTGflops = params->MPIFFT_maxErr =
  params->MaxPingPongLatency = params-> RandomlyOrderedRingLatency = params-> MinPingPongBandwidth =
  params->NaturallyOrderedRingBandwidth = params->RandomlyOrderedRingBandwidth =
  params->MinPingPongLatency = params->AvgPingPongLatency = params->MaxPingPongBandwidth =
  params->AvgPingPongBandwidth = params->NaturallyOrderedRingLatency = -1.0;

  params->HPLrdata.Gflops = -1000.0;
  params->HPLrdata.time = params->HPLrdata.eps = params->HPLrdata.RnormI = params->HPLrdata.Anorm1 = params->HPLrdata.AnormI = params->HPLrdata.Xnorm1 = params->HPLrdata.XnormI = -1.0;
  params->HPLrdata.N = params->HPLrdata.NB = params->HPLrdata.nprow = params->HPLrdata.npcol = params->HPLrdata.depth = params->HPLrdata.nbdiv = params->HPLrdata.nbmin = -1;
  params->HPLrdata.cpfact = params->HPLrdata.crfact = params->HPLrdata.ctop = params->HPLrdata.order = '-';

  params->PTRANSrdata.GBs = params->PTRANSrdata.time = params->PTRANSrdata.residual = -1.0;
  params->PTRANSrdata.n = params->PTRANSrdata.nb = params->PTRANSrdata.nprow =
  params->PTRANSrdata.npcol = -1;

  params->MPIRandomAccess_ErrorsFraction =
  params->MPIRandomAccess_time = params->MPIRandomAccess_CheckTime =
  params->MPIRandomAccess_TimeBound = -1.0;

  params->DGEMM_N =
  params->FFT_N =
  params->StreamVectorSize = -1;

  params->StreamThreads = 1;

  params->FFTEnblk = params->FFTEnp = params->FFTEl2size = -1;

  params->MPIFFT_N =
  params->RandomAccess_N =
  params->MPIRandomAccess_N =
  params->MPIRandomAccess_Errors =
  params->MPIRandomAccess_ExeUpdates = (s64Int)(-1);

  procMax = procMin = params->pval[0] * params->qval[0];
  for (i = 1; i < params->npqs; ++i) {
    procCur = params->pval[i] * params->qval[i];
    if (procMax < procCur) procMax = procCur;
    if (procMin > procCur) procMin = procCur;
  }
  params->HPLMaxProc = procMax;
  params->HPLMinProc = procMin;

  nMax = params->nval[iiamax( params->ns, params->nval, 1 )];

  /* totalMem = (nMax*nMax) * sizeof(double) */
  totalMem = nMax;
  totalMem *= nMax;
  totalMem *= sizeof(double);
  params->HPLMaxProcMem = totalMem / procMin;

  for (i = 0; i < MPIFFT_TIMING_COUNT; i++)
    params->MPIFFTtimingsForward[i] = 0.0;

  return 0;
}

double
HPCC_dweps() {
  double eps, one, half;

  one = 1.0;
  half = one / 2.0;
  eps = one;

  while (one + eps != one)
    eps *= half;

  return eps;
}

float
HPCC_sweps() {
  float eps, one, half;

  one = 1.0f;
  half = one / 2.0f;
  eps = one;

  while (one + eps != one)
    eps *= half;

  return eps;
}

int
HPCC_Finalize(HPCC_Params *params) {
  int myRank, commSize;
  int i;
  FILE *outputFile;
  MPI_Comm comm = MPI_COMM_WORLD;
  time_t currentTime;

  time( &currentTime );

  MPI_Comm_rank( comm, &myRank );
  MPI_Comm_size( comm, &commSize );

  BEGIN_IO(myRank, params->outFname, outputFile);
  FPRINTF( myRank, outputFile, "Begin of Summary section.%s", "" );
  FPRINTF( myRank, outputFile, "VersionMajor=%d", HPCC_VERSION_MAJOR );
  FPRINTF( myRank, outputFile, "VersionMinor=%d", HPCC_VERSION_MINOR );
  FPRINTF( myRank, outputFile, "VersionMicro=%d", HPCC_VERSION_MICRO );
  FPRINTF( myRank, outputFile, "VersionRelease=%c", HPCC_VERSION_RELEASE );
  FPRINTF( myRank, outputFile, "LANG=%s", "C" );
  FPRINTF( myRank, outputFile, "Success=%d", params->Failure ? 0 : 1 );
  FPRINTF( myRank, outputFile, "sizeof_char=%d", (int)sizeof(char) );
  FPRINTF( myRank, outputFile, "sizeof_short=%d", (int)sizeof(short) );
  FPRINTF( myRank, outputFile, "sizeof_int=%d", (int)sizeof(int) );
  FPRINTF( myRank, outputFile, "sizeof_long=%d", (int)sizeof(long) );
  FPRINTF( myRank, outputFile, "sizeof_void_ptr=%d", (int)sizeof(void*) );
  FPRINTF( myRank, outputFile, "sizeof_size_t=%d", (int)sizeof(size_t) );
  FPRINTF( myRank, outputFile, "sizeof_float=%d", (int)sizeof(float) );
  FPRINTF( myRank, outputFile, "sizeof_double=%d", (int)sizeof(double) );
  FPRINTF( myRank, outputFile, "sizeof_s64Int=%d", (int)sizeof(s64Int) );
  FPRINTF( myRank, outputFile, "sizeof_u64Int=%d", (int)sizeof(u64Int) );
  FPRINTF( myRank, outputFile, "CommWorldProcs=%d", commSize );
  FPRINTF( myRank, outputFile, "MPI_Wtick=%e", MPI_Wtick() );
  FPRINTF( myRank, outputFile, "HPL_Tflops=%g", params->HPLrdata.Gflops * 1e-3 );
  FPRINTF( myRank, outputFile, "HPL_time=%g", params->HPLrdata.time );
  FPRINTF( myRank, outputFile, "HPL_eps=%g", params->HPLrdata.eps );
  FPRINTF( myRank, outputFile, "HPL_RnormI=%g", params->HPLrdata.RnormI );
  FPRINTF( myRank, outputFile, "HPL_Anorm1=%g", params->HPLrdata.Anorm1 );
  FPRINTF( myRank, outputFile, "HPL_AnormI=%g", params->HPLrdata.AnormI );
  FPRINTF( myRank, outputFile, "HPL_Xnorm1=%g", params->HPLrdata.Xnorm1 );
  FPRINTF( myRank, outputFile, "HPL_XnormI=%g", params->HPLrdata.XnormI );
  FPRINTF( myRank, outputFile, "HPL_N=%d", params->HPLrdata.N );
  FPRINTF( myRank, outputFile, "HPL_NB=%d", params->HPLrdata.NB );
  FPRINTF( myRank, outputFile, "HPL_nprow=%d", params->HPLrdata.nprow );
  FPRINTF( myRank, outputFile, "HPL_npcol=%d", params->HPLrdata.npcol );
  FPRINTF( myRank, outputFile, "HPL_depth=%d", params->HPLrdata.depth );
  FPRINTF( myRank, outputFile, "HPL_nbdiv=%d", params->HPLrdata.nbdiv );
  FPRINTF( myRank, outputFile, "HPL_nbmin=%d", params->HPLrdata.nbmin );
  FPRINTF( myRank, outputFile, "HPL_cpfact=%c", params->HPLrdata.cpfact );
  FPRINTF( myRank, outputFile, "HPL_crfact=%c", params->HPLrdata.crfact );
  FPRINTF( myRank, outputFile, "HPL_ctop=%c", params->HPLrdata.ctop );
  FPRINTF( myRank, outputFile, "HPL_order=%c", params->HPLrdata.order );
  FPRINTF( myRank, outputFile, "HPL_dMACH_EPS=%e",  HPL_dlamch( HPL_MACH_EPS ) );
  FPRINTF( myRank, outputFile, "HPL_dMACH_SFMIN=%e",HPL_dlamch( HPL_MACH_SFMIN ) );
  FPRINTF( myRank, outputFile, "HPL_dMACH_BASE=%e", HPL_dlamch( HPL_MACH_BASE ) );
  FPRINTF( myRank, outputFile, "HPL_dMACH_PREC=%e", HPL_dlamch( HPL_MACH_PREC ) );
  FPRINTF( myRank, outputFile, "HPL_dMACH_MLEN=%e", HPL_dlamch( HPL_MACH_MLEN ) );
  FPRINTF( myRank, outputFile, "HPL_dMACH_RND=%e",  HPL_dlamch( HPL_MACH_RND ) );
  FPRINTF( myRank, outputFile, "HPL_dMACH_EMIN=%e", HPL_dlamch( HPL_MACH_EMIN ) );
  FPRINTF( myRank, outputFile, "HPL_dMACH_RMIN=%e", HPL_dlamch( HPL_MACH_RMIN ) );
  FPRINTF( myRank, outputFile, "HPL_dMACH_EMAX=%e", HPL_dlamch( HPL_MACH_EMAX ) );
  FPRINTF( myRank, outputFile, "HPL_dMACH_RMAX=%e", HPL_dlamch( HPL_MACH_RMAX ) );
  FPRINTF( myRank, outputFile, "HPL_sMACH_EPS=%e",  (double)HPL_slamch( HPL_MACH_EPS ) );
  FPRINTF( myRank, outputFile, "HPL_sMACH_SFMIN=%e",(double)HPL_slamch( HPL_MACH_SFMIN ) );
  FPRINTF( myRank, outputFile, "HPL_sMACH_BASE=%e", (double)HPL_slamch( HPL_MACH_BASE ) );
  FPRINTF( myRank, outputFile, "HPL_sMACH_PREC=%e", (double)HPL_slamch( HPL_MACH_PREC ) );
  FPRINTF( myRank, outputFile, "HPL_sMACH_MLEN=%e", (double)HPL_slamch( HPL_MACH_MLEN ) );
  FPRINTF( myRank, outputFile, "HPL_sMACH_RND=%e",  (double)HPL_slamch( HPL_MACH_RND ) );
  FPRINTF( myRank, outputFile, "HPL_sMACH_EMIN=%e", (double)HPL_slamch( HPL_MACH_EMIN ) );
  FPRINTF( myRank, outputFile, "HPL_sMACH_RMIN=%e", (double)HPL_slamch( HPL_MACH_RMIN ) );
  FPRINTF( myRank, outputFile, "HPL_sMACH_EMAX=%e", (double)HPL_slamch( HPL_MACH_EMAX ) );
  FPRINTF( myRank, outputFile, "HPL_sMACH_RMAX=%e", (double)HPL_slamch( HPL_MACH_RMAX ) );
  FPRINTF( myRank, outputFile, "dweps=%e", HPCC_dweps() );
  FPRINTF( myRank, outputFile, "sweps=%e", (double)HPCC_sweps() );
  FPRINTF( myRank, outputFile, "HPLMaxProcs=%d", params->HPLMaxProc );
  FPRINTF( myRank, outputFile, "HPLMinProcs=%d", params->HPLMinProc );
  FPRINTF( myRank, outputFile, "DGEMM_N=%d", params->DGEMM_N );
  FPRINTF( myRank, outputFile, "StarDGEMM_Gflops=%g",   params->StarDGEMMGflops );
  FPRINTF( myRank, outputFile, "SingleDGEMM_Gflops=%g", params->SingleDGEMMGflops );
  FPRINTF( myRank, outputFile, "PTRANS_GBs=%g", params->PTRANSrdata.GBs );
  FPRINTF( myRank, outputFile, "PTRANS_time=%g", params->PTRANSrdata.time );
  FPRINTF( myRank, outputFile, "PTRANS_residual=%g", params->PTRANSrdata.residual );
  FPRINTF( myRank, outputFile, "PTRANS_n=%d", params->PTRANSrdata.n );
  FPRINTF( myRank, outputFile, "PTRANS_nb=%d", params->PTRANSrdata.nb );
  FPRINTF( myRank, outputFile, "PTRANS_nprow=%d", params->PTRANSrdata.nprow );
  FPRINTF( myRank, outputFile, "PTRANS_npcol=%d", params->PTRANSrdata.npcol );
  FPRINTF( myRank, outputFile, "MPIRandomAccess_N=" FSTR64 , params->MPIRandomAccess_N );
  FPRINTF( myRank, outputFile, "MPIRandomAccess_time=%g", params->MPIRandomAccess_time );
  FPRINTF( myRank, outputFile, "MPIRandomAccess_CheckTime=%g", params->MPIRandomAccess_CheckTime );
  FPRINTF( myRank, outputFile, "MPIRandomAccess_Errors=" FSTR64 , params->MPIRandomAccess_Errors );
  FPRINTF( myRank, outputFile, "MPIRandomAccess_ErrorsFraction=%g", params->MPIRandomAccess_ErrorsFraction );
  FPRINTF( myRank, outputFile, "MPIRandomAccess_ExeUpdates=" FSTR64 , params->MPIRandomAccess_ExeUpdates );
  FPRINTF( myRank, outputFile, "MPIRandomAccess_GUPs=%g", params->MPIGUPs );
  FPRINTF( myRank, outputFile, "MPIRandomAccess_TimeBound=%g", params->MPIRandomAccess_TimeBound );
  FPRINTF( myRank, outputFile, "RandomAccess_N=" FSTR64, params->RandomAccess_N );
  FPRINTF( myRank, outputFile, "StarRandomAccess_GUPs=%g", params->StarGUPs );
  FPRINTF( myRank, outputFile, "SingleRandomAccess_GUPs=%g", params->SingleGUPs );
  FPRINTF( myRank, outputFile, "STREAM_VectorSize=%d", params->StreamVectorSize );
  FPRINTF( myRank, outputFile, "STREAM_Threads=%d", params->StreamThreads );
  FPRINTF( myRank, outputFile, "StarSTREAM_Copy=%g", params->StarStreamCopyGBs );
  FPRINTF( myRank, outputFile, "StarSTREAM_Scale=%g", params->StarStreamScaleGBs );
  FPRINTF( myRank, outputFile, "StarSTREAM_Add=%g", params->StarStreamAddGBs );
  FPRINTF( myRank, outputFile, "StarSTREAM_Triad=%g", params->StarStreamTriadGBs );
  FPRINTF( myRank, outputFile, "SingleSTREAM_Copy=%g", params->SingleStreamCopyGBs );
  FPRINTF( myRank, outputFile, "SingleSTREAM_Scale=%g", params->SingleStreamScaleGBs );
  FPRINTF( myRank, outputFile, "SingleSTREAM_Add=%g", params->SingleStreamAddGBs );
  FPRINTF( myRank, outputFile, "SingleSTREAM_Triad=%g", params->SingleStreamTriadGBs );
  FPRINTF( myRank, outputFile, "FFT_N=%d", params->FFT_N );
  FPRINTF( myRank, outputFile, "StarFFT_Gflops=%g",   params->StarFFTGflops );
  FPRINTF( myRank, outputFile, "SingleFFT_Gflops=%g", params->SingleFFTGflops );
  FPRINTF( myRank, outputFile, "MPIFFT_N=" FSTR64, params->MPIFFT_N );
  FPRINTF( myRank, outputFile, "MPIFFT_Gflops=%g", params->MPIFFTGflops );
  FPRINTF( myRank, outputFile, "MPIFFT_maxErr=%g", params->MPIFFT_maxErr );
  FPRINTF( myRank, outputFile, "MaxPingPongLatency_usec=%g", params->MaxPingPongLatency );
  FPRINTF( myRank, outputFile, "RandomlyOrderedRingLatency_usec=%g", params->RandomlyOrderedRingLatency );
  FPRINTF( myRank, outputFile, "MinPingPongBandwidth_GBytes=%g", params->MinPingPongBandwidth );
  FPRINTF( myRank, outputFile, "NaturallyOrderedRingBandwidth_GBytes=%g", params->NaturallyOrderedRingBandwidth );
  FPRINTF( myRank, outputFile, "RandomlyOrderedRingBandwidth_GBytes=%g", params->RandomlyOrderedRingBandwidth );
  FPRINTF( myRank, outputFile, "MinPingPongLatency_usec=%g", params->MinPingPongLatency );
  FPRINTF( myRank, outputFile, "AvgPingPongLatency_usec=%g", params->AvgPingPongLatency );
  FPRINTF( myRank, outputFile, "MaxPingPongBandwidth_GBytes=%g", params->MaxPingPongBandwidth );
  FPRINTF( myRank, outputFile, "AvgPingPongBandwidth_GBytes=%g", params->AvgPingPongBandwidth );
  FPRINTF( myRank, outputFile, "NaturallyOrderedRingLatency_usec=%g", params->NaturallyOrderedRingLatency );
  FPRINTF( myRank, outputFile, "FFTEnblk=%d", params->FFTEnblk );
  FPRINTF( myRank, outputFile, "FFTEnp=%d", params->FFTEnp );
  FPRINTF( myRank, outputFile, "FFTEl2size=%d", params->FFTEl2size );

#ifdef _OPENMP
  FPRINTF( myRank, outputFile, "M_OPENMP=%ld", (long)(_OPENMP) );
#pragma omp parallel
  {
#pragma omp single nowait
    {

      FPRINTF( myRank, outputFile, "omp_get_num_threads=%d", omp_get_num_threads() );
      FPRINTF( myRank, outputFile, "omp_get_max_threads=%d", omp_get_max_threads() );
      FPRINTF( myRank, outputFile, "omp_get_num_procs=%d",   omp_get_num_procs() );
    }
  }
#else
  FPRINTF( myRank, outputFile, "M_OPENMP=%ld", -1L );
  FPRINTF( myRank, outputFile, "omp_get_num_threads=%d", 0 );
  FPRINTF( myRank, outputFile, "omp_get_max_threads=%d", 0 );
  FPRINTF( myRank, outputFile, "omp_get_num_procs=%d",   0 );
#endif

  FPRINTF( myRank, outputFile, "MemProc=%g", HPCC_MemProc );
  FPRINTF( myRank, outputFile, "MemSpec=%d", HPCC_MemSpec );
  FPRINTF( myRank, outputFile, "MemVal=%g", HPCC_MemVal );

  for (i = 0; i < MPIFFT_TIMING_COUNT - 1; i++)
    FPRINTF2(myRank,outputFile, "MPIFFT_time%d=%g", i, params->MPIFFTtimingsForward[i+1] - params->MPIFFTtimingsForward[i] );
  FPRINTF( myRank, outputFile, "End of Summary section.%s", "" );
  FPRINTF( myRank, outputFile,
            "########################################################################%s", "" );
  FPRINTF( myRank, outputFile, "End of HPC Challange tests.%s", "" );
  FPRINTF2(myRank, outputFile, "Current time (%ld) is %s",(long)currentTime,ctime(&currentTime));
  FPRINTF( myRank, outputFile,
            "########################################################################%s", "" );
  END_IO(  myRank, outputFile );

  return 0;
}

int
HPCC_LocalVectorSize(HPCC_Params *params, int vecCnt, size_t size, int pow2) {
  int flg2, maxIntBits2;

  /* this is the maximum power of 2 that that can be held in a signed integer (for a 4-byte
     integer, 2**31-1 is the maximum integer, so the maximum power of 2 is 30) */
  maxIntBits2 = sizeof(int) * 8 - 2;

  /* flg2 = floor(log2(params->HPLMaxProcMem / size / vecCnt)) */
  for (flg2 = 1; params->HPLMaxProcMem / size / vecCnt >> flg2; ++flg2)
    ; /* EMPTY */
  --flg2;

  if (flg2 <= maxIntBits2) {
    if (pow2)
      return 1 << flg2;

    return params->HPLMaxProcMem / size / vecCnt;
  }

  return 1 << maxIntBits2;
}

int
HPCC_ProcessGrid(int *P, int *Q, MPI_Comm comm) {
  int myRank, commSize;
  int i, p, q, nproc;

  MPI_Comm_size( comm, &commSize );
  MPI_Comm_rank( comm, &myRank );

  for (nproc = commSize; ; --nproc) { /* this loop makes at most two iterations */

    for (i = (int)sqrt( nproc ); i > 1; --i) {
      q = nproc / i;
      p = nproc / q;
      if (p * q == nproc) {
        *P = p;
        *Q = q;
        return 0;
      }
    }

    /* if the code gets here `nproc' is small or is a prime */

    if (nproc < 20) { /* do 1D grid for small process counts */
      *P = 1;
      *Q = nproc;
      return 0;
    }
  }

  return 0;
}

size_t
HPCC_Memory(MPI_Comm comm) {
  int myRank, commSize;
  int num_threads;
  char memFile[13] = "hpccmemf.txt";
  char buf[HPL_LINE_MAX]; int nbuf = HPL_LINE_MAX;
  char *sVal;
  FILE *f;
  double mult, mval, procMem;
  size_t rv;

  mult = 1.0;
  num_threads = 1;

  MPI_Comm_size( comm, &commSize );
  MPI_Comm_rank( comm, &myRank );

#ifdef _OPENMP
#pragma omp parallel
  {
#pragma omp single nowait
    {
      num_threads = omp_get_num_threads();
    }
  }
#endif

  if (myRank == 0) {
    procMem = 64;

    f = fopen( memFile, "r" );
    if (f) {

      if (fgets( buf, nbuf, f )) {

        if (strncmp( "Total=", buf, 6 ) == 0) {
          mult = 1.0 / commSize;
          sVal = buf + 6;
          HPCC_MemSpec = 1;
        } else if (strncmp( "Thread=", buf, 7 ) == 0) {
          mult = num_threads;
          sVal = buf + 7;
          HPCC_MemSpec = 2;
        } else if (strncmp( "Process=", buf, 8 ) == 0) {
          mult = 1.0;
          sVal = buf + 8;
          HPCC_MemSpec = 3;
        } else
          sVal = NULL;

        if (sVal && 1 == sscanf( sVal, "%lf", &mval )) {
          procMem = mval * mult;
          HPCC_MemVal = mval;
        }
      }

      fclose( f );
    }
  }

  MPI_Bcast( &procMem, 1, MPI_DOUBLE, 0, comm );

  rv = procMem;
  rv *= 1024; rv *= 1024;

  HPCC_MemProc = procMem;

  return rv;
}

int
HPCC_Defaults(HPL_T_test *TEST, int *NS, int *N,
              int *NBS, int *NB,
              HPL_T_ORDER *PMAPPIN,
              int *NPQS, int *P, int *Q,
              int *NPFS, HPL_T_FACT *PF,
              int *NBMS, int *NBM,
              int *NDVS, int *NDV,
              int *NRFS, HPL_T_FACT *RF,
              int *NTPS, HPL_T_TOP *TP,
              int *NDHS, int *DH,
              HPL_T_SWAP *FSWAP, int *TSWAP, int *L1NOTRAN, int *UNOTRAN, int *EQUIL, int *ALIGN, MPI_Comm comm) {
  int nb = 80;
  double memFactor = 0.8;

  *NS = *NBS = *NPQS = *NPFS = *NBMS = *NDVS = *NRFS = *NTPS = *NDHS = 1;

  TEST->thrsh = 16.0;

  *NB = nb;

  *PMAPPIN = HPL_COLUMN_MAJOR;

  HPCC_ProcessGrid( P, Q, comm );

  *N = (int)sqrt( memFactor * (double)(*P * *Q) * (double)(HPCC_Memory( comm ) / sizeof(double)) ) / (2 * nb);
  *N *= 2*nb; /* make N multiple of 2*nb so both HPL and PTRANS see matrix
                 dimension divisible by nb */

  *PF = HPL_RIGHT_LOOKING;

  *NBM = 4;

  *NDV = 2;

  *RF = HPL_CROUT;

  *TP = HPL_1RING_M;

  *DH = 1;

  *FSWAP = HPL_SW_MIX;

  *TSWAP = 64;

  *L1NOTRAN = 0;

  *UNOTRAN = 0;

  *EQUIL = 1;

  *ALIGN = 8;

  return 0;
}

#ifdef XERBLA_MISSING

#ifdef Add_
#define F77xerbla xerbla_
#endif
#ifdef NoChange
#define F77xerbla xerbla
#endif
#ifdef UpCase
#define F77xerbla XERBLA
#endif
#ifdef f77IsF2C
#define F77xerbla xerbla_
#endif

void
F77xerbla(char *srname, F77_INTEGER *info, long srname_len) {
  /*
  int i; char Cname[7];
  for (i = 0; i < 6; i++) Cname[i] = srname[i];
  Cname[6] = 0;
  printf("xerbla(%d)\n", *info);
  */
  printf("xerbla()\n");
  fflush(stdout);
}
#endif
