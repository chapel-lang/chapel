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
  for (i = 0; i < n; i += incx) {
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
      fprintf( outputFile, "Error in line %d of the input file.\n", line );
      END_IO( myRank, outputFile );
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
      BEGIN_IO( myRank, params->outFname, outputFile );
      fprintf( outputFile, "Error in line %d of the input file.\n", line );
      END_IO( myRank, outputFile );
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
  int i, nMax, nbMax, procCur, procMax, procMin, errCode;
  double totalMem;
  char inFname[12] = "hpccinf.txt", outFname[13] = "hpccoutf.txt";
  FILE *outputFile;
  MPI_Comm comm = MPI_COMM_WORLD;
  time_t currentTime;
  char hostname[MPI_MAX_PROCESSOR_NAME + 1]; int hostnameLen;
  size_t hpl_mem, ptrans_mem;
  long dMemSize;

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

  BEGIN_IO( myRank, params->outFname, outputFile );
  fprintf( outputFile,
            "########################################################################\n" );
  fprintf( outputFile,
            "This is the DARPA/DOE HPC Challenge Benchmark version %d.%d.%d October 2003\n",
            HPCC_VERSION_MAJOR, HPCC_VERSION_MINOR, HPCC_VERSION_MICRO );
  fprintf( outputFile, "Produced by Jack Dongarra and Piotr Luszczek\n" );
  fprintf( outputFile, "Innovative Computing Laboratory\n" );
  fprintf( outputFile, "University of Tennessee Knoxville and Oak Ridge National Laboratory\n\n" );
  fprintf( outputFile, "See the source files for authors of specific codes.\n" );
  fprintf( outputFile, "Compiled on %s at %s\n", __DATE__ , __TIME__ );
  fprintf( outputFile, "Current time (%ld) is %s\n",(long)currentTime,ctime(&currentTime));
  fprintf( outputFile, "Hostname: '%s'\n", hostname );
  fprintf( outputFile,
            "########################################################################\n" );
  END_IO( myRank, outputFile );

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
  params->StreamVectorSize =
  params->MPIRandomAccess_Algorithm =
  params->MPIFFT_Procs = -1;

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

  i = iiamax( params->PTRANSnbs, params->PTRANSnbval, 1 );
  nbMax = params->PTRANSnbval[i];

#ifdef HPCC_MEMALLCTR
  MaxMem( commSize, 0, 0, params->PTRANSns, params->PTRANSnval, params->PTRANSnval, params->PTRANSnbs, params->PTRANSnbval, params->PTRANSnbval, params->PTRANSnpqs, params->PTRANSpval, params->PTRANSqval, &dMemSize );
  ptrans_mem = dMemSize * sizeof(double) + 3 * commSize * sizeof(int);
  hpl_mem = params->HPLMaxProcMem + (nMax + nbMax) * sizeof(double) * nbMax;
  HPCC_alloc_init( Mmax( ptrans_mem, hpl_mem ) );
#endif

  return 0;
}

int
HPCC_Finalize(HPCC_Params *params) {
  int myRank, commSize;
  int i;
  FILE *outputFile;
  MPI_Comm comm = MPI_COMM_WORLD;
  time_t currentTime;

#ifdef HPCC_MEMALLCTR
  HPCC_alloc_finalize();
#endif

  time( &currentTime );

  MPI_Comm_rank( comm, &myRank );
  MPI_Comm_size( comm, &commSize );

  BEGIN_IO(myRank, params->outFname, outputFile);
  fprintf( outputFile, "Begin of Summary section.\n" );
  fprintf( outputFile, "VersionMajor=%d\n", HPCC_VERSION_MAJOR );
  fprintf( outputFile, "VersionMinor=%d\n", HPCC_VERSION_MINOR );
  fprintf( outputFile, "VersionMicro=%d\n", HPCC_VERSION_MICRO );
  fprintf( outputFile, "VersionRelease=%c\n", HPCC_VERSION_RELEASE );
  fprintf( outputFile, "LANG=%s\n", "C" );
  fprintf( outputFile, "Success=%d\n", params->Failure ? 0 : 1 );
  fprintf( outputFile, "sizeof_char=%d\n", (int)sizeof(char) );
  fprintf( outputFile, "sizeof_short=%d\n", (int)sizeof(short) );
  fprintf( outputFile, "sizeof_int=%d\n", (int)sizeof(int) );
  fprintf( outputFile, "sizeof_long=%d\n", (int)sizeof(long) );
  fprintf( outputFile, "sizeof_void_ptr=%d\n", (int)sizeof(void*) );
  fprintf( outputFile, "sizeof_size_t=%d\n", (int)sizeof(size_t) );
  fprintf( outputFile, "sizeof_float=%d\n", (int)sizeof(float) );
  fprintf( outputFile, "sizeof_double=%d\n", (int)sizeof(double) );
  fprintf( outputFile, "sizeof_s64Int=%d\n", (int)sizeof(s64Int) );
  fprintf( outputFile, "sizeof_u64Int=%d\n", (int)sizeof(u64Int) );
  fprintf( outputFile, "sizeof_struct_double_double=%d\n", (int)sizeof(struct{double HPCC_r,HPCC_i;}) );
  fprintf( outputFile, "CommWorldProcs=%d\n", commSize );
  fprintf( outputFile, "MPI_Wtick=%e\n", MPI_Wtick() );
  fprintf( outputFile, "HPL_Tflops=%g\n", params->HPLrdata.Gflops * 1e-3 );
  fprintf( outputFile, "HPL_time=%g\n", params->HPLrdata.time );
  fprintf( outputFile, "HPL_eps=%g\n", params->HPLrdata.eps );
  fprintf( outputFile, "HPL_RnormI=%g\n", params->HPLrdata.RnormI );
  fprintf( outputFile, "HPL_Anorm1=%g\n", params->HPLrdata.Anorm1 );
  fprintf( outputFile, "HPL_AnormI=%g\n", params->HPLrdata.AnormI );
  fprintf( outputFile, "HPL_Xnorm1=%g\n", params->HPLrdata.Xnorm1 );
  fprintf( outputFile, "HPL_XnormI=%g\n", params->HPLrdata.XnormI );
  fprintf( outputFile, "HPL_N=%d\n", params->HPLrdata.N );
  fprintf( outputFile, "HPL_NB=%d\n", params->HPLrdata.NB );
  fprintf( outputFile, "HPL_nprow=%d\n", params->HPLrdata.nprow );
  fprintf( outputFile, "HPL_npcol=%d\n", params->HPLrdata.npcol );
  fprintf( outputFile, "HPL_depth=%d\n", params->HPLrdata.depth );
  fprintf( outputFile, "HPL_nbdiv=%d\n", params->HPLrdata.nbdiv );
  fprintf( outputFile, "HPL_nbmin=%d\n", params->HPLrdata.nbmin );
  fprintf( outputFile, "HPL_cpfact=%c\n", params->HPLrdata.cpfact );
  fprintf( outputFile, "HPL_crfact=%c\n", params->HPLrdata.crfact );
  fprintf( outputFile, "HPL_ctop=%c\n", params->HPLrdata.ctop );
  fprintf( outputFile, "HPL_order=%c\n", params->HPLrdata.order );
  fprintf( outputFile, "HPL_dMACH_EPS=%e\n",  HPL_dlamch( HPL_MACH_EPS ) );
  fprintf( outputFile, "HPL_dMACH_SFMIN=%e\n",HPL_dlamch( HPL_MACH_SFMIN ) );
  fprintf( outputFile, "HPL_dMACH_BASE=%e\n", HPL_dlamch( HPL_MACH_BASE ) );
  fprintf( outputFile, "HPL_dMACH_PREC=%e\n", HPL_dlamch( HPL_MACH_PREC ) );
  fprintf( outputFile, "HPL_dMACH_MLEN=%e\n", HPL_dlamch( HPL_MACH_MLEN ) );
  fprintf( outputFile, "HPL_dMACH_RND=%e\n",  HPL_dlamch( HPL_MACH_RND ) );
  fprintf( outputFile, "HPL_dMACH_EMIN=%e\n", HPL_dlamch( HPL_MACH_EMIN ) );
  fprintf( outputFile, "HPL_dMACH_RMIN=%e\n", HPL_dlamch( HPL_MACH_RMIN ) );
  fprintf( outputFile, "HPL_dMACH_EMAX=%e\n", HPL_dlamch( HPL_MACH_EMAX ) );
  fprintf( outputFile, "HPL_dMACH_RMAX=%e\n", HPL_dlamch( HPL_MACH_RMAX ) );
  fprintf( outputFile, "HPL_sMACH_EPS=%e\n",  (double)HPL_slamch( HPL_MACH_EPS ) );
  fprintf( outputFile, "HPL_sMACH_SFMIN=%e\n",(double)HPL_slamch( HPL_MACH_SFMIN ) );
  fprintf( outputFile, "HPL_sMACH_BASE=%e\n", (double)HPL_slamch( HPL_MACH_BASE ) );
  fprintf( outputFile, "HPL_sMACH_PREC=%e\n", (double)HPL_slamch( HPL_MACH_PREC ) );
  fprintf( outputFile, "HPL_sMACH_MLEN=%e\n", (double)HPL_slamch( HPL_MACH_MLEN ) );
  fprintf( outputFile, "HPL_sMACH_RND=%e\n",  (double)HPL_slamch( HPL_MACH_RND ) );
  fprintf( outputFile, "HPL_sMACH_EMIN=%e\n", (double)HPL_slamch( HPL_MACH_EMIN ) );
  fprintf( outputFile, "HPL_sMACH_RMIN=%e\n", (double)HPL_slamch( HPL_MACH_RMIN ) );
  fprintf( outputFile, "HPL_sMACH_EMAX=%e\n", (double)HPL_slamch( HPL_MACH_EMAX ) );
  fprintf( outputFile, "HPL_sMACH_RMAX=%e\n", (double)HPL_slamch( HPL_MACH_RMAX ) );
  fprintf( outputFile, "dweps=%e\n", HPCC_dweps() );
  fprintf( outputFile, "sweps=%e\n", (double)HPCC_sweps() );
  fprintf( outputFile, "HPLMaxProcs=%d\n", params->HPLMaxProc );
  fprintf( outputFile, "HPLMinProcs=%d\n", params->HPLMinProc );
  fprintf( outputFile, "DGEMM_N=%d\n", params->DGEMM_N );
  fprintf( outputFile, "StarDGEMM_Gflops=%g\n",   params->StarDGEMMGflops );
  fprintf( outputFile, "SingleDGEMM_Gflops=%g\n", params->SingleDGEMMGflops );
  fprintf( outputFile, "PTRANS_GBs=%g\n", params->PTRANSrdata.GBs );
  fprintf( outputFile, "PTRANS_time=%g\n", params->PTRANSrdata.time );
  fprintf( outputFile, "PTRANS_residual=%g\n", params->PTRANSrdata.residual );
  fprintf( outputFile, "PTRANS_n=%d\n", params->PTRANSrdata.n );
  fprintf( outputFile, "PTRANS_nb=%d\n", params->PTRANSrdata.nb );
  fprintf( outputFile, "PTRANS_nprow=%d\n", params->PTRANSrdata.nprow );
  fprintf( outputFile, "PTRANS_npcol=%d\n", params->PTRANSrdata.npcol );
  fprintf( outputFile, "MPIRandomAccess_N=" FSTR64 "\n", params->MPIRandomAccess_N );
  fprintf( outputFile, "MPIRandomAccess_time=%g\n", params->MPIRandomAccess_time );
  fprintf( outputFile, "MPIRandomAccess_CheckTime=%g\n", params->MPIRandomAccess_CheckTime );
  fprintf( outputFile, "MPIRandomAccess_Errors=" FSTR64 "\n", params->MPIRandomAccess_Errors );
  fprintf( outputFile, "MPIRandomAccess_ErrorsFraction=%g\n", params->MPIRandomAccess_ErrorsFraction );
  fprintf( outputFile, "MPIRandomAccess_ExeUpdates=" FSTR64 "\n", params->MPIRandomAccess_ExeUpdates );
  fprintf( outputFile, "MPIRandomAccess_GUPs=%g\n", params->MPIGUPs );
  fprintf( outputFile, "MPIRandomAccess_TimeBound=%g\n", params->MPIRandomAccess_TimeBound );
  fprintf( outputFile, "MPIRandomAccess_Algorithm=%d\n", params->MPIRandomAccess_Algorithm );
  fprintf( outputFile, "RandomAccess_N=" FSTR64 "\n", params->RandomAccess_N );
  fprintf( outputFile, "StarRandomAccess_GUPs=%g\n", params->StarGUPs );
  fprintf( outputFile, "SingleRandomAccess_GUPs=%g\n", params->SingleGUPs );
  fprintf( outputFile, "STREAM_VectorSize=%d\n", params->StreamVectorSize );
  fprintf( outputFile, "STREAM_Threads=%d\n", params->StreamThreads );
  fprintf( outputFile, "StarSTREAM_Copy=%g\n", params->StarStreamCopyGBs );
  fprintf( outputFile, "StarSTREAM_Scale=%g\n", params->StarStreamScaleGBs );
  fprintf( outputFile, "StarSTREAM_Add=%g\n", params->StarStreamAddGBs );
  fprintf( outputFile, "StarSTREAM_Triad=%g\n", params->StarStreamTriadGBs );
  fprintf( outputFile, "SingleSTREAM_Copy=%g\n", params->SingleStreamCopyGBs );
  fprintf( outputFile, "SingleSTREAM_Scale=%g\n", params->SingleStreamScaleGBs );
  fprintf( outputFile, "SingleSTREAM_Add=%g\n", params->SingleStreamAddGBs );
  fprintf( outputFile, "SingleSTREAM_Triad=%g\n", params->SingleStreamTriadGBs );
  fprintf( outputFile, "FFT_N=%d\n", params->FFT_N );
  fprintf( outputFile, "StarFFT_Gflops=%g\n",   params->StarFFTGflops );
  fprintf( outputFile, "SingleFFT_Gflops=%g\n", params->SingleFFTGflops );
  fprintf( outputFile, "MPIFFT_N=" FSTR64 "\n", params->MPIFFT_N );
  fprintf( outputFile, "MPIFFT_Gflops=%g\n", params->MPIFFTGflops );
  fprintf( outputFile, "MPIFFT_maxErr=%g\n", params->MPIFFT_maxErr );
  fprintf( outputFile, "MPIFFT_Procs=%d\n", params->MPIFFT_Procs );
  fprintf( outputFile, "MaxPingPongLatency_usec=%g\n", params->MaxPingPongLatency );
  fprintf( outputFile, "RandomlyOrderedRingLatency_usec=%g\n", params->RandomlyOrderedRingLatency );
  fprintf( outputFile, "MinPingPongBandwidth_GBytes=%g\n", params->MinPingPongBandwidth );
  fprintf( outputFile, "NaturallyOrderedRingBandwidth_GBytes=%g\n", params->NaturallyOrderedRingBandwidth );
  fprintf( outputFile, "RandomlyOrderedRingBandwidth_GBytes=%g\n", params->RandomlyOrderedRingBandwidth );
  fprintf( outputFile, "MinPingPongLatency_usec=%g\n", params->MinPingPongLatency );
  fprintf( outputFile, "AvgPingPongLatency_usec=%g\n", params->AvgPingPongLatency );
  fprintf( outputFile, "MaxPingPongBandwidth_GBytes=%g\n", params->MaxPingPongBandwidth );
  fprintf( outputFile, "AvgPingPongBandwidth_GBytes=%g\n", params->AvgPingPongBandwidth );
  fprintf( outputFile, "NaturallyOrderedRingLatency_usec=%g\n", params->NaturallyOrderedRingLatency );
  fprintf( outputFile, "FFTEnblk=%d\n", params->FFTEnblk );
  fprintf( outputFile, "FFTEnp=%d\n", params->FFTEnp );
  fprintf( outputFile, "FFTEl2size=%d\n", params->FFTEl2size );

#ifdef _OPENMP
  fprintf( outputFile, "M_OPENMP=%ld\n", (long)(_OPENMP) );
#pragma omp parallel
  {
#pragma omp single nowait
    {

      fprintf( outputFile, "omp_get_num_threads=%d\n", omp_get_num_threads() );
      fprintf( outputFile, "omp_get_max_threads=%d\n", omp_get_max_threads() );
      fprintf( outputFile, "omp_get_num_procs=%d\n",   omp_get_num_procs() );
    }
  }
#else
  fprintf( outputFile, "M_OPENMP=%ld\n", -1L );
  fprintf( outputFile, "omp_get_num_threads=%d\n", 0 );
  fprintf( outputFile, "omp_get_max_threads=%d\n", 0 );
  fprintf( outputFile, "omp_get_num_procs=%d\n",   0 );
#endif

  fprintf( outputFile, "MemProc=%g\n", HPCC_MemProc );
  fprintf( outputFile, "MemSpec=%d\n", HPCC_MemSpec );
  fprintf( outputFile, "MemVal=%g\n", HPCC_MemVal );

  for (i = 0; i < MPIFFT_TIMING_COUNT - 1; i++)
    fprintf( outputFile, "MPIFFT_time%d=%g\n", i, params->MPIFFTtimingsForward[i+1] - params->MPIFFTtimingsForward[i] );

  /* CPS: C Preprocessor Symbols */

  i = 0;
#ifdef HPCC_FFT_235
  i = 1;
#endif
  fprintf( outputFile, "CPS_HPCC_FFT_235=%d\n", i );

  i = 0;
#ifdef HPCC_FFTW_ESTIMATE
  i = 1;
#endif
  fprintf( outputFile, "CPS_HPCC_FFTW_ESTIMATE=%d\n", i );

  i = 0;
#ifdef HPCC_MEMALLCTR
  i = 1;
#endif
  fprintf( outputFile, "CPS_HPCC_MEMALLCTR=%d\n", i );

  i = 0;
#ifdef HPL_USE_GETPROCESSTIMES
  i = 1;
#endif
  fprintf( outputFile, "CPS_HPL_USE_GETPROCESSTIMES=%d\n", i );


  i = 0;
#ifdef RA_SANDIA_NOPT
  i = 1;
#endif
  fprintf( outputFile, "CPS_RA_SANDIA_NOPT=%d\n", i );

  i = 0;
#ifdef RA_SANDIA_OPT2
  i = 1;
#endif
  fprintf( outputFile, "CPS_RA_SANDIA_OPT2=%d\n", i );

  i = 0;
#ifdef USING_FFTW
  i = 1;
#endif
  fprintf( outputFile, "CPS_USING_FFTW=%d\n", i );

  fprintf( outputFile, "End of Summary section.%s\n", "" );
  fprintf( outputFile,
            "########################################################################\n" );
  fprintf( outputFile, "End of HPC Challenge tests.\n" );
  fprintf( outputFile, "Current time (%ld) is %s\n",(long)currentTime,ctime(&currentTime));
  fprintf( outputFile,
            "########################################################################\n" );
  END_IO( myRank, outputFile );

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
#ifdef Add__
#define F77xerbla xerbla__
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

#ifdef HPCC_MEMALLCTR
#define MEM_MAXCNT 7

typedef double Mem_t;

static Mem_t *Mem_base;
static size_t Mem_dsize;

/*
  Each entry can be in one of three states:
  1. Full (holds a block of allocated memory) if:
     ptr != NULL; size > 0; free == 0
  2. Free (holds block of unallocated memory) if:
     ptr != NULL; free = 1
  3  Empty (doesn't hold a block of memory) if:
     ptr == NULL; free = 1
 */
typedef struct {
  Mem_t *Mem_ptr;
  size_t Mem_size;
  int Mem_free;
} Mem_entry_t;

static Mem_entry_t Mem_blocks[MEM_MAXCNT];

static void
HPCC_alloc_set_empty(int idx) {
  int i, n0, n;

  if (MEM_MAXCNT == idx) {
    n0 = 0;
    n = idx;
  } else {
    n0 = idx;
    n = idx + 1;
  }

  /* initialize all blocks to empty */
  for (i = n0; i < n; ++i) {
    Mem_blocks[i].Mem_ptr = (Mem_t *)(NULL);
    Mem_blocks[i].Mem_size = 0;
    Mem_blocks[i].Mem_free = 1;
  }
}

static void
HPCC_alloc_set_free(int idx, Mem_t *dptr, size_t size) {
  Mem_blocks[idx].Mem_ptr = dptr;
  Mem_blocks[idx].Mem_size = size;
  Mem_blocks[idx].Mem_free = 1;
}

int
HPCC_alloc_init(size_t total_size) {
  size_t dsize;

  Mem_dsize = dsize = Mceil( total_size, sizeof(Mem_t) );

  Mem_base = (Mem_t *)malloc( dsize * sizeof(Mem_t) );

  HPCC_alloc_set_empty( MEM_MAXCNT );

  if (Mem_base) {
    HPCC_alloc_set_free( 0, Mem_base, dsize );
    return 0;
  }

  return -1;
}

int
HPCC_alloc_finalize() {
  free( Mem_base );
  HPCC_alloc_set_empty( MEM_MAXCNT );
  return 0;
}

void *
HPCC_malloc(size_t size) {
  size_t dsize, diff_size, cur_diff_size;
  int i, cur_best, cur_free;

  dsize = Mceil( size, sizeof(Mem_t) );

  cur_diff_size = Mem_dsize + 1;
  cur_free = cur_best = MEM_MAXCNT;

  for (i = 0; i < MEM_MAXCNT; ++i) {
    /* skip full spots */
    if (! Mem_blocks[i].Mem_free)
      continue;

    /* find empty spot */
    if (! Mem_blocks[i].Mem_ptr) {
      cur_free = i;
      continue;
    }

    diff_size = Mem_blocks[i].Mem_size - dsize;

    if (Mem_blocks[i].Mem_size >= dsize && diff_size < cur_diff_size) {
      /* a match that's the best (so far) was found */
      cur_diff_size = diff_size;
      cur_best = i;
    }
  }

  /* found a match */
  if (cur_best < MEM_MAXCNT) {
    if (cur_free < MEM_MAXCNT && cur_diff_size > 0) {
      /* create a new free block */
      HPCC_alloc_set_free( cur_free, Mem_blocks[cur_best].Mem_ptr + dsize,
                           cur_diff_size );

      Mem_blocks[cur_best].Mem_size = dsize; /* shrink the best match */
    }

    Mem_blocks[cur_best].Mem_free = 0;

    return (void *)(Mem_blocks[cur_best].Mem_ptr);
  }

  return NULL;
}

void
HPCC_free(void *ptr) {
  Mem_t *dptr = (Mem_t *)ptr;
  int cur_blk = MEM_MAXCNT, made_changes, i, j;

  /* look for the block being freed */
  for (i = 0; i < MEM_MAXCNT; ++i) {
    if (Mem_blocks[i].Mem_free)
      continue;

    if (Mem_blocks[i].Mem_ptr == dptr) {
      cur_blk = i;
      break;
    }
  }

  /* not finding the pointer (including NULL) causes abort */
  if (MEM_MAXCNT == cur_blk) {
    HPL_pabort( __LINE__, "HPCC_free", "Unknown pointer in HPCC_free()." );
  }

  /* double-free causes abort */
  if (1 == Mem_blocks[cur_blk].Mem_free) {
    HPL_pabort( __LINE__, "HPCC_free", "Second call to HPCC_free() with the same pointer." );
  }

  Mem_blocks[cur_blk].Mem_free = 1;

  /* merge as many blocks as possible */
  for (made_changes = 1; made_changes;) {
    made_changes = 0;

    for (i = 0; i < MEM_MAXCNT; ++i) {

      /* empty or full blocks can't be merged */
      if (! Mem_blocks[i].Mem_free || ! Mem_blocks[i].Mem_ptr)
        continue;

      for (j = 0; j < MEM_MAXCNT; ++j) {

        /* empty or occupied blocks can't be merged */
        if (! Mem_blocks[j].Mem_free || ! Mem_blocks[j].Mem_ptr)
          continue;

        if (Mem_blocks[i].Mem_ptr + Mem_blocks[i].Mem_size ==
            Mem_blocks[j].Mem_ptr) {
          Mem_blocks[i].Mem_size += Mem_blocks[j].Mem_size;

          HPCC_alloc_set_empty( j );

          made_changes = 1;
        }
      }
    }
  }
}
#endif
