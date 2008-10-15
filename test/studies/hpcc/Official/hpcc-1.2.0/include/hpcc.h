/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; fill-column: 79; coding: iso-latin-1-unix -*- */

#ifndef HPCC_H
#define HPCC_H 1

/* HPL includes:
stdio.h
stdlib.h
string.h
stdarg.h
vararg.h (if necessary)
mpi.h
*/
#include <hpl.h>

#include <hpccver.h>

#include <math.h>
#include <time.h>

#define MPIFFT_TIMING_COUNT 8

/* Define 64-bit types and corresponding format strings for printf() and constants */
#ifdef LONG_IS_64BITS
typedef unsigned long u64Int;
typedef long s64Int;
#define FSTR64 "%ld"
#define FSTRU64 "%lu"
#define ZERO64B 0L
#else
typedef unsigned long long u64Int;
typedef long long s64Int;
#define FSTR64 "%lld"
#define FSTRU64 "%llu"
#define ZERO64B 0LL
#endif

typedef struct {
  double GBs, time, residual;
  int n, nb, nprow, npcol;
} PTRANS_RuntimeData;

/* parameters of execution */
typedef struct {
  /* HPL section */
   HPL_T_test                 test;
   int                        nval  [HPL_MAX_PARAM],
                              nbval [HPL_MAX_PARAM],
                              pval  [HPL_MAX_PARAM],
                              qval  [HPL_MAX_PARAM],
                              nbmval[HPL_MAX_PARAM],
                              ndvval[HPL_MAX_PARAM],
                              ndhval[HPL_MAX_PARAM];
   HPL_T_ORDER                porder;
   HPL_T_FACT                 pfaval[HPL_MAX_PARAM],
                              rfaval[HPL_MAX_PARAM];
   HPL_T_TOP                  topval[HPL_MAX_PARAM];
   HPL_T_FACT                 rpfa;
   HPL_T_SWAP                 fswap;
   int ns, nbs, npqs, npfs, nbms, ndvs, nrfs, ntps, ndhs, tswap, L1notran, Unotran, equil, align;

  /* HPCC section */
  char inFname[256 + 1], outFname[256 + 1];
  int PTRANSns, PTRANSnval[2 * HPL_MAX_PARAM];
  int PTRANSnbs, PTRANSnbval[2 * HPL_MAX_PARAM];
  int PTRANSnpqs, PTRANSpval[2 * HPL_MAX_PARAM], PTRANSqval[2 * HPL_MAX_PARAM];
  double MPIGUPs, StarGUPs, SingleGUPs,
    MPIRandomAccess_ErrorsFraction, MPIRandomAccess_time, MPIRandomAccess_CheckTime,
    MPIRandomAccess_TimeBound,
    StarStreamCopyGBs, StarStreamScaleGBs,
    StarStreamAddGBs, StarStreamTriadGBs, SingleStreamCopyGBs, SingleStreamScaleGBs,
    SingleStreamAddGBs, SingleStreamTriadGBs, StarDGEMMGflops, SingleDGEMMGflops;
  double StarFFTGflops, SingleFFTGflops, MPIFFTGflops, MPIFFT_maxErr;
  double MaxPingPongLatency, RandomlyOrderedRingLatency, MinPingPongBandwidth,
    NaturallyOrderedRingBandwidth, RandomlyOrderedRingBandwidth,
    MinPingPongLatency, AvgPingPongLatency, MaxPingPongBandwidth, AvgPingPongBandwidth,
    NaturallyOrderedRingLatency;
  int DGEMM_N;
  int StreamThreads, StreamVectorSize;
  int FFT_N;
  int MPIFFT_Procs;
  int MPIRandomAccess_Algorithm;

  HPL_RuntimeData HPLrdata;
  PTRANS_RuntimeData PTRANSrdata;

  int Failure; /* over all failure of the benchmark */

  double MPIFFTtimingsForward[MPIFFT_TIMING_COUNT], MPIFFTtimingsBackward[MPIFFT_TIMING_COUNT];

  size_t HPLMaxProcMem;
  int HPLMaxProc, HPLMinProc;
  int RunHPL, RunStarDGEMM, RunSingleDGEMM, RunPTRANS, RunStarStream,
    RunSingleStream, RunMPIRandomAccess, RunStarRandomAccess,
    RunSingleRandomAccess, RunLatencyBandwidth, RunStarFFT, RunSingleFFT, RunMPIFFT;

  int FFTEnblk, FFTEnp, FFTEl2size;
  s64Int RandomAccess_N, MPIRandomAccess_ExeUpdates, MPIRandomAccess_N, MPIRandomAccess_Errors, MPIFFT_N;
} HPCC_Params;
/*
This is what needs to be done to add a new benchmark:
-  Add the benchmark code to the directory structure (headers, makefiles)
-  Add benchmark output data to the HPCC_Params structure.
-  Initialize the HPCC_Params structure data in HPCC_Init().
-  Add a call to the benchmark function in main().
-  Make sure that all the processes fill out the structure with the same data.
-  Print the output of the benchmark in HPCC_Finalize().
-  For tests that have "Star" and "Single" variants (DGEMM, RandomAccess, STREAM) the function
that performs the test returns a value (0 or 1) that indicates runtime failure and also returns
benchamark failure (due to wrong optimization that causes numerical error) by setting
params->Failure.
*/

extern int HPCC_Init(HPCC_Params *params);
extern int HPCC_Finalize(HPCC_Params *params);
extern int HPCC_LocalVectorSize(HPCC_Params *params, int vecCnt, size_t size, int pow2);
extern int
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
              HPL_T_SWAP *FSWAP, int *TSWAP, int *L1NOTRAN, int *UNOTRAN, int *EQUIL, int *ALIGN, MPI_Comm comm);

extern int HPL_main(int ARGC, char **ARGV, HPL_RuntimeData *rdata, int *failure);
extern float HPL_slamch (const HPL_T_MACH);
extern double HPCC_dweps();
extern float HPCC_sweps();
extern int HPCC_StarDGEMM(HPCC_Params *params);
extern int HPCC_SingleDGEMM(HPCC_Params *params);
extern int PTRANS(HPCC_Params *params);
extern int HPCC_MPIRandomAccess(HPCC_Params *params);
extern int HPCC_SingleRandomAccess(HPCC_Params *params);
extern int HPCC_StarRandomAccess(HPCC_Params *params);
extern int HPCC_SingleStream(HPCC_Params *params);
extern int HPCC_StarStream(HPCC_Params *params);
extern int HPCC_StarFFT(HPCC_Params *params);
extern int HPCC_SingleFFT(HPCC_Params *params);
extern int HPCC_MPIFFT(HPCC_Params *params);

extern int HPCC_TestFFT(HPCC_Params *params, int doIO, double *UGflops, int *Un, int *Ufailure);
extern int HPCC_TestDGEMM(HPCC_Params *params, int doIO, double *UGflops, int *Un, int *Ufailure);
extern int MaxMem(int nprocs, int imrow, int imcol, int nmat, int *mval, int *nval, int nbmat,
  int *mbval, int *nbval, int ngrids, int *npval, int *nqval, long *maxMem);
extern int HPCC_RandomAccess(HPCC_Params *params, int doIO, double *GUPs, int *failure);
extern int HPCC_Stream(HPCC_Params *params, int doIO, double *copyGBs, double *scaleGBs,
  double *addGBs, double *triadGBs, int *failure);
extern void main_bench_lat_bw(HPCC_Params *params);

extern int pdtrans(char *trans, int *m, int *n, int * mb, int *nb, double *a, int *lda,
  double *beta, double *c__, int *ldc, int *imrow, int *imcol, double *work, int *iwork);
extern FILE* pdtransinfo(int *nmat, int *mval, int *nval, int *ldval,
  int *nbmat, int *mbval, int *nbval, int *ldnbval, int *ngrids, int *npval, int *nqval,
  int *ldpqval, int *iaseed, int *imrow, int *imcol, float *thresh, int *iam, int *nprocs,
  double *eps, char *infname, int *fcl, char *outfname);
extern int pdmatgen(int *ictxt, char *aform, char *diag, int *m, int *n, int *mb, int *nb, double*a,
  int *lda, int *iarow, int *iacol, int *iseed, int *iroff, int *irnum, int *icoff, int *icnum,
  int * myrow, int *mycol, int *nprow, int *npcol, double alpha);
extern int pdmatcmp(int *ictxt, int *m_, int *n_, double *a, int *lda_, double *aCopy, int *ldc_,
  double *error);
extern int pxerbla(int *ictxt, char *srname, int *info);
extern int slcombine_(int *ictxt, char *scope, char *op, char * timetype, int *n, int *ibeg,
  double *times);
extern int icopy_(int *n, int *sx, int *incx, int * sy, int *incy);
extern int numroc_(int *, int *, int *, int *, int *);
extern int slboot_(void);
extern int sltimer_(int *i__);
extern int ilcm_(int *, int *);
extern int iceil_(int *, int *);
extern double pdrand();
extern int setran_(int *, int *, int *);
extern int jumpit_(int *, int *, int *, int *);
extern int xjumpm_(int *, int *, int *, int *, int *, int *, int *);
/* ---------------------------------------------------------------------- */

#define DPRN(i,v) do{printf(__FILE__ "(%d)@%d:" #v "=%g\n",__LINE__,i,(double)(v));fflush(stdout);}while(0)

#define BEGIN_IO(r,fn,f) if(0==r){f=fopen(fn,"a");if(!f)fprintf(f=stderr,"Problem with appending to file '%s'\n",fn)
#define END_IO(r,f) fflush(f); if (f!=stdout && f!=stderr) fclose(f);} f=(FILE*)(NULL)

#include <hpccmema.h>

#define XMALLOC(t,s) ((t*)malloc(sizeof(t)*(s)))
#define XCALLOC(t,s) ((t*)calloc((s),sizeof(t)))

#endif
