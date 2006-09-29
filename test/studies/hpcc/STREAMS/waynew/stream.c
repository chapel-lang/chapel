/*-----------------------------------------------------------------------*/
/* Program: Stream                                                       */
/* Revision: $Id$ */
/* Original code developed by John D. McCalpin                           */
/* Programmers: John D. McCalpin                                         */
/*              Joe R. Zagar                                             */
/*                                                                       */
/* This program measures memory transfer rates in GB/s for simple        */
/* computational kernels coded in C.                                     */
/*-----------------------------------------------------------------------*/
/* Copyright 1991-2003: John D. McCalpin                                 */
/*-----------------------------------------------------------------------*/
/* License:                                                              */
/*  1. You are free to use this program and/or to redistribute           */
/*     this program.                                                     */
/*  2. You are free to modify this program for your own use,             */
/*     including commercial use, subject to the publication              */
/*     restrictions in item 3.                                           */
/*  3. You are free to publish results obtained from running this        */
/*     program, or from works that you derive from this program,         */
/*     with the following limitations:                                   */
/*     3a. In order to be referred to as "STREAM benchmark results",     */
/*         published results must be in conformance to the STREAM        */
/*         Run Rules, (briefly reviewed below) published at              */
/*         http://www.cs.virginia.edu/stream/ref.html                    */
/*         and incorporated herein by reference.                         */
/*         As the copyright holder, John McCalpin retains the            */
/*         right to determine conformity with the Run Rules.             */
/*     3b. Results based on modified source code or on runs not in       */
/*         accordance with the STREAM Run Rules must be clearly          */
/*         labelled whenever they are published.  Examples of            */
/*         proper labelling include:                                     */
/*         "tuned STREAM benchmark results"                              */
/*         "based on a variant of the STREAM benchmark code"             */
/*         Other comparable, clear and reasonable labelling is           */
/*         acceptable.                                                   */
/*     3c. Submission of results to the STREAM benchmark web site        */
/*         is encouraged, but not required.                              */
/*  4. Use of this program or creation of derived works based on this    */
/*     program constitutes acceptance of these licensing restrictions.   */
/*  5. Absolutely no warranty is expressed or implied.                   */
/*-----------------------------------------------------------------------*/
#include <hpcc.h>

# include <float.h>
# include <limits.h>

#define TUNED 1
#define VERBOSE 1

/* INSTRUCTIONS:
 *
 *       1) Stream requires a good bit of memory to run.  Adjust the
 *          value of 'N' (below) to give a 'timing calibration' of
 *          at least 20 clock-ticks.  This will provide rate estimates
 *          that should be good to about 5% precision.
 */

static int VectorSize;
# define N 2000000
# define NTIMES 10
# define OFFSET 0

/*
 * 3) Compile the code with full optimization.  Many compilers
 *    generate unreasonably bad code before the optimizer tightens
 *    things up.  If the results are unreasonably good, on the
 *    other hand, the optimizer might be too smart for me!
 *
 *         Try compiling with:
 *               cc -O stream_omp.c -o stream_omp
 *
 *         This is known to work on Cray, SGI, IBM, and Sun machines.
 *
 *
 * 4) Mail the results to mccalpin@cs.virginia.edu
 *    Be sure to include:
 *    a) computer hardware model number and software revision
 *    b) the compiler flags
 *    c) all of the output from the test case.
 * Thanks!
 *
 */

# define HLINE "-------------------------------------------------------------\n"

static double *a, *b, *c;

static double avgtime[4] = {0}, maxtime[4] = {0},
  mintime[4] = {FLT_MAX,FLT_MAX,FLT_MAX,FLT_MAX};

static char *label[4] = {"Copy:      ", "Scale:     ",
    "Add:       ", "Triad:     "};

static double bytes[4] = {
    2 * sizeof(double),
    2 * sizeof(double),
    3 * sizeof(double),
    3 * sizeof(double)
    };

#define mysecond MPI_Wtime
#ifdef TUNED
extern void tuned_STREAM_Copy(void);
extern void tuned_STREAM_Scale(double scalar);
extern void tuned_STREAM_Add(void);
extern void tuned_STREAM_Triad(double scalar);
#endif

static void
checkSTREAMresults (FILE *outFile, int doIO, int *failure)
{
  double aj,bj,cj,scalar;
  double asum,bsum,csum;
  double epsilon;
  int j,k;

    /* reproduce initialization */
  aj = 1.0;
  bj = 2.0;
  cj = 0.0;
    /* a[] is modified during timing check */
  aj = 2.0E0 * aj;
    /* now execute timing loop */
  scalar = 3.0;
  for (k=0; k<NTIMES; k++)
        {
            cj = aj;
            bj = scalar*cj;
            cj = aj+bj;
            aj = bj+scalar*cj;
        }
  aj = aj * (double) VectorSize;
  bj = bj * (double) VectorSize;
  cj = cj * (double) VectorSize;

  asum = 0.0;
  bsum = 0.0;
  csum = 0.0;
  for (j=0; j<VectorSize; j++) {
    asum += a[j];
    bsum += b[j];
    csum += c[j];
  }
#ifdef VERBOSE
        if (doIO) {
          fprintf( outFile, "Results Comparison: \n");
          fprintf( outFile, "        Expected  : %f %f %f \n",aj,bj,cj);
          fprintf( outFile, "        Observed  : %f %f %f \n",asum,bsum,csum);
        }
#endif

        epsilon = 1.e-8;

        *failure = 1;
        if (fabs(aj-asum)/asum > epsilon) {
          if (doIO) {
            fprintf( outFile, "Failed Validation on array a[]\n");
            fprintf( outFile, "        Expected  : %f \n",aj);
            fprintf( outFile, "        Observed  : %f \n",asum);
          }
        }
        else if (fabs(bj-bsum)/bsum > epsilon) {
          if (doIO) {
            fprintf( outFile, "Failed Validation on array b[]\n");
            fprintf( outFile, "        Expected  : %f \n",bj);
            fprintf( outFile, "        Observed  : %f \n",bsum);
          }
        }
        else if (fabs(cj-csum)/csum > epsilon) {
          if (doIO) {
            fprintf( outFile, "Failed Validation on array c[]\n");
            fprintf( outFile, "        Expected  : %f \n",cj);
            fprintf( outFile, "        Observed  : %f \n",csum);
          }
        }
        else {
          *failure = 0;
          if (doIO) fprintf( outFile, "Solution Validates\n");
 }
}

# define M 20

static int
checktick()
    {
    int  i, minDelta, Delta;
    double t1, t2, timesfound[M];

/*  Collect a sequence of M unique time values from the system. */

    for (i = 0; i < M; i++) {
      t1 = mysecond();
      while( ((t2=mysecond()) - t1) < 1.0E-6 )
        ;
      timesfound[i] = t1 = t2;
    }

/*
 * Determine the minimum difference between these M values.
 * This result will be our estimate (in microseconds) for the
 * clock granularity.
 */

    minDelta = 1000000;
    for (i = 1; i < M; i++) {
      Delta = (int)( 1.0E6 * (timesfound[i]-timesfound[i-1]));
      minDelta = Mmin(minDelta, Mmax(Delta,0));
    }

   return(minDelta);
    }
#undef M



int
HPCC_Stream(HPCC_Params *params, int doIO, double *copyGBs, double *scaleGBs, double *addGBs,
  double *triadGBs, int *failure) {
    int   quantum;
    int   BytesPerWord;
    register int j, k;
    double  scalar, t, times[4][NTIMES];
    FILE *outFile;
    double GBs = 1073741824.0, curGBs;

    if (doIO) {
      outFile = fopen( params->outFname, "a" );
      if (! outFile) {
        outFile = stderr;
        fprintf( outFile, "Cannot open output file.\n" );
        return 1;
      }
    }

    VectorSize = HPCC_LocalVectorSize( params, 3, sizeof(double), 0 ); /* Need 3 vectors */
    params->StreamVectorSize = VectorSize;

    a = XMALLOC( double, VectorSize );
    b = XMALLOC( double, VectorSize );
    c = XMALLOC( double, VectorSize );

    if (!a || !b || !c) {
      if (c) free(c);
      if (b) free(b);
      if (a) free(a);
      if (doIO) {
        fprintf( outFile, "Failed to allocate memory (%d).\n", VectorSize );
        fflush( outFile );
        fclose( outFile );
      }
      return 1;
    }

    /* --- SETUP --- determine precision and check timing --- */

    if (doIO) {
    fprintf( outFile, HLINE);
    BytesPerWord = sizeof(double);
    fprintf( outFile, "This system uses %d bytes per DOUBLE PRECISION word.\n",
             BytesPerWord);

    fprintf( outFile, HLINE);
    fprintf( outFile, "Array size = %d, Offset = %d\n" , VectorSize, OFFSET);
    fprintf( outFile, "Total memory required = %.4f GB.\n",
             (3.0 * BytesPerWord) * ( (double) VectorSize / GBs));
    fprintf( outFile, "Each test is run %d times, but only\n", NTIMES);
    fprintf( outFile, "the *best* time for each is used.\n");
    }

#ifdef _OPENMP
    if (doIO) fprintf( outFile, HLINE);
#pragma omp parallel private(k)
    {
#pragma omp single nowait
      {
        k = omp_get_num_threads();
        if (doIO) fprintf( outFile, "Number of Threads requested = %i\n",k);
        params->StreamThreads = k;
      }
    }
#endif

    /* Get initial value for system clock. */
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (j=0; j<VectorSize; j++) {
      a[j] = 1.0;
      b[j] = 2.0;
      c[j] = 0.0;
    }

    if (doIO) fprintf( outFile, HLINE);

    if  ( (quantum = checktick()) >= 1) {
      if (doIO) fprintf( outFile, "Your clock granularity/precision appears to be "
                         "%d microseconds.\n", quantum);
    } else {
      if (doIO) fprintf( outFile, "Your clock granularity appears to be "
                         "less than one microsecond.\n");
    }

    t = mysecond();
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (j = 0; j < VectorSize; j++)
      a[j] = 2.0E0 * a[j];
    t = 1.0E6 * (mysecond() - t);

    if (doIO) {
    fprintf( outFile, "Each test below will take on the order"
             " of %d microseconds.\n", (int) t  );
    fprintf( outFile, "   (= %d clock ticks)\n", (int) (t/quantum) );
    fprintf( outFile, "Increase the size of the arrays if this shows that\n");
    fprintf( outFile, "you are not getting at least 20 clock ticks per test.\n");

    fprintf( outFile, HLINE);

    fprintf( outFile, "WARNING -- The above is only a rough guideline.\n");
    fprintf( outFile, "For best results, please be sure you know the\n");
    fprintf( outFile, "precision of your system timer.\n");
    fprintf( outFile, HLINE);
    }

    /* --- MAIN LOOP --- repeat test cases NTIMES times --- */

    scalar = 3.0;
    for (k=0; k<NTIMES; k++)
    {
      times[0][k] = mysecond();
#ifdef TUNED
        tuned_STREAM_Copy();
#else
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (j=0; j<VectorSize; j++)
          c[j] = a[j];
#endif
        times[0][k] = mysecond() - times[0][k];

        times[1][k] = mysecond();
#ifdef TUNED
        tuned_STREAM_Scale(scalar);
#else
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (j=0; j<VectorSize; j++)
          b[j] = scalar*c[j];
#endif
        times[1][k] = mysecond() - times[1][k];

        times[2][k] = mysecond();
#ifdef TUNED
        tuned_STREAM_Add();
#else
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (j=0; j<VectorSize; j++)
          c[j] = a[j]+b[j];
#endif
        times[2][k] = mysecond() - times[2][k];

        times[3][k] = mysecond();
#ifdef TUNED
        tuned_STREAM_Triad(scalar);
#else
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (j=0; j<VectorSize; j++)
          a[j] = b[j]+scalar*c[j];
#endif
        times[3][k] = mysecond() - times[3][k];
    }

    /* --- SUMMARY --- */

    for (k=1; k<NTIMES; k++) /* note -- skip first iteration */
    {
      for (j=0; j<4; j++)
      {
        avgtime[j] = avgtime[j] + times[j][k];
        mintime[j] = Mmin(mintime[j], times[j][k]);
        maxtime[j] = Mmax(maxtime[j], times[j][k]);
      }
    }
 
    if (doIO)
    fprintf( outFile, "Function      Rate (GB/s)   Avg time     Min time     Max time\n");
    for (j=0; j<4; j++) {
      avgtime[j] /= (double)(NTIMES - 1); /* note -- skip first iteration */

      /* make sure no division by zero */
      curGBs = (mintime[j] > 0.0 ? 1.0 / mintime[j] : -1.0);
      curGBs *= 1e-9 * bytes[j] * VectorSize;
        if (doIO)
          fprintf( outFile, "%s%11.4f  %11.4f  %11.4f  %11.4f\n", label[j],
                   curGBs,
                   avgtime[j],
                   mintime[j],
                   maxtime[j]);
        switch (j) {
          case 0: *copyGBs = curGBs; break;
          case 1: *scaleGBs = curGBs; break;
          case 2: *addGBs = curGBs; break;
          case 3: *triadGBs = curGBs; break;
        }
    }
    if (doIO) fprintf( outFile, HLINE);

    /* --- Check Results --- */
    checkSTREAMresults( outFile, doIO, failure );
    if (doIO) fprintf( outFile, HLINE);

    free(c);
    free(b);
    free(a);

    if (doIO) {
      fflush( outFile );
      fclose( outFile );
    }

    return 0;
}

void tuned_STREAM_Copy()
{
  int j;
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (j=0; j<VectorSize; j++)
            c[j] = a[j];
}

void tuned_STREAM_Scale(double scalar)
{
  int j;
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (j=0; j<VectorSize; j++)
    b[j] = scalar*c[j];
}

void tuned_STREAM_Add()
{
  int j;
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (j=0; j<VectorSize; j++)
    c[j] = a[j]+b[j];
}

void tuned_STREAM_Triad(double scalar)
{
  int j;
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (j=0; j<VectorSize; j++)
    a[j] = b[j]+scalar*c[j];
}
