/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; -*- */
/*
  pdtransdriver.c

  -- PUMMA Package routine (version 2.1) --
     Jaeyoung Choi, Oak Ridge National Laboratory.
     Jack Dongarra, Univ. of Tennessee, Oak Ridge National Laboratory.
     David Walker,  Oak Ridge National Laboratory.
     March 26, 1995.

  Purpose: Driver routine for testing the full matrix transpose.
*/

#include <hpcc.h>

#include "cblacslt.h"

/* Common Block Declarations */

struct {
    int ictxt;
} context_;

#define context_1 context_

/* Table of constant values */

static int c__1 = 1;
static int c__0 = 0;

static void
param_dump(FILE *outFile, char *name, int n, int *vals) {
  int j;
  fprintf( outFile, "%s:", name );
  for (j = 0; j < n; ++j)
    fprintf( outFile, " %d", vals[j] );
  fprintf( outFile, "\n" );
}

static void
param_illegal(int iam, FILE *outFile, char *fmt, char *contxt, char *val_name, int x) {
  if (0 != iam) return;

  if (val_name[0])
    fprintf( outFile, fmt, contxt, val_name, x );
  else
    fprintf( outFile, fmt, contxt );
  fprintf( outFile, "\n" );
}

static void
param_allred_sum(int *ierr) {
  int success;
  MPI_Allreduce( ierr, &success, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD );
  ierr[0] = success;
}

static void
grid_map(int np_me, int npall, int nprow, int npcol, int seed, int *umap) {
  int i, j, k, rval[2], rmul[2], radd[2];

  if (seed < 0) seed = -seed;
  rval[1] = (seed >> 16) & 32767; rval[0] = seed & 65535;

  rmul[0] = 20077;
  rmul[1] = 16838;
  radd[0] = 12345;
  radd[1] = 0;
  setran_( rval, rmul, radd );
  pdrand();

  for (i = 0; i < npall; ++i)
    umap[i] = i;

  for (i = 0; i < npall; ++i) {
    j = pdrand() * npall;

    /* swap entries i and j */
    k = umap[j];
    umap[j] = umap[i];
    umap[i] = k;
  }
}

int
PTRANS(HPCC_Params *params) {
  /* calculation of passed/failed/skipped tests assumes that MPI rank 0 is 0x0 in CBLACS */
  int ktests = 0;
  int kpass = 0;
  int kfail = 0;
  int kskip = 0;

  int i__, j, m, n;
  int mb, nb, ii, mg, ng, mp, mq, np, nq;
  int mp0, mq0, np0, nq0, lda, ldc, iam, lcm;
  double eps, *mem;
  int *imem;
  long ipa, ipc, ipw, ipiw, isw;
  int nmat, *mval, ierr[1], *nval;
  int nbmat, *mbval, imcol, *nbval;
  double ctime[2], resid, resid0 = 1.0;
  int npcol, *npval, mycol, *nqval;
  double wtime[2];
  int imrow, nprow, myrow, iaseed = 100, proc_seed;
  char *passed;
  int ngrids;
  double thresh;
  int nprocs;

  FILE *outFile;
  double curGBs, curGBs_0, cpuGBs, *GBs;
  int AllocSuccessful, grid_cnt, r0x0, r0_ingrid;
  int icseed = 200;
  double d_One = 1.0;
  long dMemSize, li;

  GBs = &params->PTRANSrdata.GBs;
  *GBs = curGBs = 0.0;

  Cblacs_pinfo(&iam, &nprocs);

  if (0 == iam) {
    outFile = fopen( params->outFname, "a" );
    if (! outFile) outFile = stderr;
  } else
    outFile = stderr;

  nmat = params->PTRANSns;
  mval = params->PTRANSnval;
  nval = params->PTRANSnval;

  nbmat = params->PTRANSnbs;
  mbval = params->PTRANSnbval;
  nbval = params->PTRANSnbval;

  ngrids = params->PTRANSnpqs;
  npval = params->PTRANSpval;
  nqval = params->PTRANSqval;

  thresh = params->test.thrsh;
  eps = params->test.epsil;

  imrow = imcol = 0;

  /* calculate and allocate memory */
  AllocSuccessful = 0;
  MaxMem( nprocs, imrow, imcol, nmat, mval, nval, nbmat, mbval, nbval, ngrids, npval, nqval, &dMemSize );
  mem = NULL; imem = NULL;
  if (dMemSize > 0) {
    mem = HPCC_XMALLOC( double, dMemSize );
    imem = HPCC_XMALLOC( int, (3 * nprocs) );
    if (mem && imem) AllocSuccessful = 1;
  }

  MPI_Allreduce( &AllocSuccessful, ierr, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD );
  if (ierr[0] < 1) {
    if (imem) HPCC_free(imem);
    if (mem) HPCC_free(mem);
    if (0 == iam) fprintf( outFile, "Failed to allocate %ld doubles\n", dMemSize );
    goto mem_failure;
  }

  /* initialize working arrays; it is necessary because on some systems it will contain NaNs
   * (Not a Number) and NaTs (Not a Thing) and this makes pdmatgen() work incorrectly
   * (0.0 * NaN may cause exception) */
  for (li = 0; li < dMemSize; li++) mem[li] = 0.0;
  for (j = 0; j < 3 * nprocs; j++) imem[j] = 0;

  /* Print headings */
  if (0 == iam) {
    /* matrix sizes */
    param_dump( outFile, "M", nmat, mval );
    param_dump( outFile, "N", nmat, nval );
    /* block sizes */
    param_dump( outFile, "MB", nbmat, mbval );
    param_dump( outFile, "NB", nbmat, nbval );
    /* process grids */
    param_dump( outFile, "P", ngrids, npval );
    param_dump( outFile, "Q", ngrids, nqval );

    fprintf( outFile,
             "TIME   M     N    MB  NB  P   Q     TIME   CHECK   GB/s   RESID\n"
             "---- ----- ----- --- --- --- --- -------- ------ -------- -----\n" );
    fflush( outFile );
  }


  /*
    Loop over different process grids
   */

  for (j = 0; j < ngrids; ++j) {
    nprow = npval[j];
    npcol = nqval[j];

    /*
      Make sure grid information is correct
    */

    ierr[0] = 0;
    if (nprow < 1) {
      param_illegal( iam, outFile, "ILLEGAL %s: %s = %d; It should be at least 1", "GRID", "nprow", nprow );
      ierr[0] = 1;
    } else if (npcol < 1) {
      param_illegal( iam, outFile, "ILLEGAL %s: %s = %d; It should be at least 1", "GRID", "npcol", npcol );
      ierr[0] = 1;
    } else if (nprow * npcol > nprocs) {
      param_illegal( iam, outFile, "ILLEGAL %s: %s = %d. Too many processes requested.", "GRID", "nprow*npcol-nprocs",
                     nprow * npcol - nprocs );
      ierr[0] = 1;
    }

    param_allred_sum( ierr );

    if (ierr[0] > 0) {
      param_illegal( iam, outFile, "Bad %s parameters: going on to next test case.", "grid", "", 0 );
      ++kskip;
      continue;
    }

    for (i__ = 0; i__ < nmat; ++i__) {
      m = mval[i__];
      n = nval[i__];

      /*
        Make sure matrix information is correct
       */

      ierr[0] = 0;
      if (m < 1) {
        param_illegal( iam, outFile, "ILLEGAL %s: %s = %d; It should be at least 1", "MATRIX", "M", m );
        ierr[0] = 1;
      } else if (n < 1) {
        param_illegal( iam, outFile, "ILLEGAL %s: %s = %d; It should be at least 1", "MATRIX", "N", n );
        ierr[0] = 1;
      }

      /*
        Make sure no one had error
       */

      param_allred_sum( ierr );

      if (ierr[0] > 0) {
        param_illegal( iam, outFile, "Bad %s parameters: going on to next test case.", "MATRIX", "", 0 );
        ++kskip;
        continue;
      }

      /*
        Loop over different block sizes
       */

      for (ii = 1; ii <= nbmat; ++ii) {

        mb = mbval[ii - 1];
        nb = nbval[ii - 1];

        /*
          Make sure blocking sizes are legal
         */

        ierr[0] = 0;
        if (mb < 1) {
          ierr[0] = 1;
          param_illegal( iam, outFile, "ILLEGAL %s: %s = %d; It should be at least 1", "MB", "MB", mb );
        } else if (nb < 1) {
          ierr[0] = 1;
          param_illegal( iam, outFile, "ILLEGAL %s: %s = %d; It should be at least 1", "NB", "NB", nb );
        }

        /*
          Make sure no one had error
         */

        param_allred_sum( ierr );

        if (ierr[0] > 0) {
          param_illegal( iam, outFile, "Bad %s parameters: going on to next test case.", "NB", "", 0 );
          ++kskip;
          continue;
        }

        for (grid_cnt = 0; grid_cnt < 5; ++grid_cnt) {

        /*
          Make sure all processes have the same seed
         */
        mp = (int)time(NULL);
        MPI_Allreduce( &mp, &proc_seed, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD );

        /* Define process grid */
        Cblacs_get(-1, 0, &context_1.ictxt);
        grid_map( iam, nprocs, nprow, npcol, proc_seed, imem );
        Cblacs_gridmap( &context_1.ictxt, imem, npcol, nprow, npcol );
        Cblacs_gridinfo(context_1.ictxt, &nprow, &npcol, &myrow, &mycol);

        /*
          Make sure all processes know who's 0x0
         */
        mp = (0 == myrow && 0 == mycol) ? iam : 0;
        MPI_Allreduce( &mp, &r0x0, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD );

        r0_ingrid = 1;

        /* Go to bottom of process grid loop if this case doesn't use my process */
        if (myrow >= nprow || mycol >= npcol) {
          /* nprow and npcol were lost in the call to Cblacs_gridinfo */
          nprow = npval[j];
          npcol = nqval[j];

          /* reporting must be done on process 0 */
          if (0 != iam)
            continue;

          r0_ingrid = 0;
          goto report;
        }

        mp = numroc_(&m, &mb, &myrow, &imrow, &nprow);
        mq = numroc_(&m, &mb, &mycol, &imcol, &npcol);
        np = numroc_(&n, &nb, &myrow, &imrow, &nprow);
        nq = numroc_(&n, &nb, &mycol, &imcol, &npcol);

        mg = iceil_(&m, &mb);
        ng = iceil_(&n, &nb);

        mp0 = iceil_(&mg, &nprow) * mb;
        mq0 = iceil_(&mg, &npcol) * mb;
        np0 = iceil_(&ng, &nprow) * nb;
        nq0 = iceil_(&ng, &npcol) * nb;

        lcm = ilcm_(&nprow, &npcol);
        ipc = 1;
        ipa = ipc + (long)np0 * (long)mq0;
        ipiw = (long)mp0 * (long)nq0 + ipa;
        ipw = ipiw;
        isw = ipw + (long)(iceil_(&mg, &lcm) << 1) * (long)mb * (long)iceil_(&ng, &lcm) * (long)nb;

        /* Make sure have enough memory to handle problem */
        if (isw > dMemSize) {
          param_illegal( iam, outFile, "Unable to perform %s: need %s of at least %d thousand doubles\n",
                         "PTRANS", "memory", (int)((isw + 999)/ 1000) );
          ierr[0] = 1;
        }

        /* Make sure no one had error */
        Cigsum2d(context_1.ictxt,"a","h",1,1,ierr, 1,-1,0);

        if (ierr[0] > 0) {
          param_illegal( iam, outFile, "Bad %s parameters: going on to next test case.", "MEMORY", "", 0 );
          ++kskip;
          continue;
        }

        /*
          Generate matrix A
         */

        lda = Mmax(1,mp);
        /* A = rand(m, n, iaseed) */
        pdmatgen(&context_1.ictxt, "N", "N", &m, &n, &mb, &nb, &mem[ipa - 1], &lda, &imrow, &imcol,
                 &iaseed, &c__0, &mp, &c__0, &nq, &myrow, &mycol, &nprow, &npcol, 0.0);
        /* C = rand(n, m, icseed) */
        pdmatgen(&context_1.ictxt, "T", "N", &n, &m, &nb, &mb, &mem[ipc - 1], &lda, &imrow, &imcol,
                 &icseed, &c__0, &np, &c__0, &mq, &myrow, &mycol, &nprow, &npcol, 0.0);

        slboot_();
        Cblacs_barrier(context_1.ictxt, "All");
        sltimer_(&c__1);

        /*
          Perform the matrix transpose
         */

        ldc = Mmax(1,np);
        /* C := A' + d_One * C */
        pdtrans( "T", &m, &n, &mb, &nb, &mem[ipa - 1], &lda, &d_One, &mem[ipc - 1], &ldc, &imrow,
                 &imcol, &mem[ipw - 1], imem );

        sltimer_(&c__1);

        if (thresh > 0.0) {

          /*
            Regenerate matrix A in transpose form (A')
           */

          lda = Mmax(1,np);
          /* A = rand(n, m, icseed) */
          pdmatgen( &context_1.ictxt, "T", "N", &n, &m, &nb, &mb, &mem[ipa - 1], &lda, &imrow, &imcol, &icseed,
                    &c__0, &np, &c__0, &mq, &myrow, &mycol, &nprow, &npcol, 0.0);
          /* A += rand(m, n, iaseed) */
          pdmatgen( &context_1.ictxt, "T", "N", &m, &n, &mb, &nb, &mem[ipa - 1], &lda, &imrow,
                    &imcol, &iaseed, &c__0, &mp, &c__0, &nq, &myrow, &mycol, &nprow, &npcol, 1.0);

          /*
            Compare A' to C
           */

          pdmatcmp(&context_1.ictxt, &np, &mq, &mem[ipa - 1], &lda, &mem[ipc - 1], &ldc, &resid);
          resid0 = resid;

          resid /= eps * Mmax( m, n );
          if (resid <= thresh && resid - resid == 0.0) { /* if `resid' is small and is not NaN */
            ++kpass;
            passed = "PASSED";
          } else {
            ++kfail;
            passed = "FAILED";
          }
        } else {

          /*
            Don't perform the checking, only the timing operation
           */

          ++kpass;
          resid -= resid;
          passed = "BYPASS";
        }

        /*
          Gather maximum of all CPU and WALL clock timings
         */

        slcombine_(&context_1.ictxt, "All", ">", "W", &c__1, &c__1, wtime);
        slcombine_(&context_1.ictxt, "All", ">", "C", &c__1, &c__1, ctime);

        Cblacs_gridexit(context_1.ictxt);

        report:

        if (0 != r0x0) {
          double dva[3];
          MPI_Status status;

          if (r0x0 == iam) {
            dva[0] = wtime[0];
            dva[1] = ctime[0];
            dva[2] = passed[0];
            MPI_Send( dva, 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD );
          }

          if (0 == iam) {
            MPI_Recv( dva, 3, MPI_DOUBLE, r0x0, 0, MPI_COMM_WORLD, &status );

            if (! r0_ingrid) { /* if 0's process not in grid, timing and pass/fail info is missing */
              wtime[0] = dva[0];
              ctime[0] = dva[1];
              switch ((int)(dva[2])) {
                case 'F': passed = "FAILED"; ++kfail; break;
                case 'B': passed = "BYPASS"; ++kpass; break;
                default:  passed = "PASSED"; ++kpass; break;
              }
            }
          }
        }

        /*
          Print results
         */

        if (0 == iam) {
          /*
            Print WALL time if machine supports it
           */

          if (wtime[0] > 0.0) {
            curGBs_0 = 1e-9 / wtime[0] * m * n * sizeof(double);

            if (0 == grid_cnt)
              curGBs = curGBs_0;

            if (curGBs > curGBs_0) /* take minimum performance */
              curGBs = curGBs_0;

            fprintf( outFile, "WALL %5d %5d %3d %3d %3d %3d %8.2f %s %8.3f %5.2f\n",
                     m, n, mb, nb, nprow, npcol, wtime[0], passed, curGBs, resid );
          }

          /*
            Print CPU time if machine supports it
           */

          if (ctime[0] > 0.0) {
            cpuGBs = 1e-9 / ctime[0] * m * n * sizeof(double);
            fprintf( outFile, "CPU  %5d %5d %3d %3d %3d %3d %8.2f %s %8.3f %5.2f\n",
                     m, n, mb, nb, nprow, npcol, ctime[0], passed, cpuGBs, resid );
          }
        }

        }

        if (0 == iam && curGBs > *GBs) {
          *GBs = curGBs;
          params->PTRANSrdata.time = wtime[0];
          params->PTRANSrdata.residual = resid0;
          params->PTRANSrdata.n = n;
          params->PTRANSrdata.nb = nb;
          params->PTRANSrdata.nprow = nprow;
          params->PTRANSrdata.npcol = npcol;
        }

      }
    }
  }

  if (imem) HPCC_free( imem );
  if (mem) HPCC_free( mem );

  mem_failure:

  /* Print out ending messages and close output file */

  if (0 == iam) {
    ktests = kpass + kfail + kskip;

    fprintf( outFile, "\nFinished %4d tests, with the following results:\n", ktests );

    if (thresh > 0.0) {
      fprintf( outFile, "%5d tests completed and passed residual checks.\n", kpass );
      fprintf( outFile, "%5d tests completed and failed residual checks.\n", kfail );
    } else {
      fprintf( outFile, "%5d tests completed without checking.\n", kpass );
    }
    fprintf( outFile, "%5d tests skipped because of illegal input values.\n", kskip );


    fprintf( outFile, "\nEND OF TESTS.\n" );

    if (outFile != stdout && outFile != stderr) fclose( outFile );
  }

  Cblacs_exit(1);

  /* if at least one test failed or was skipped then it's a total failure */
  MPI_Reduce( &kfail, &ktests, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD );
  if (ktests) params->Failure = 1;
  MPI_Reduce( &kskip, &ktests, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD );
  if (ktests) params->Failure = 1;

  return 0;
}
