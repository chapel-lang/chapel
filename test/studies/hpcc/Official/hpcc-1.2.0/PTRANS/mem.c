/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; fill-column: 79; coding: iso-latin-1-unix -*- */

#include <hpcc.h>

static int
CheckNode(int imrow, int imcol, int nmat, int *mval, int *nval, int nbmat, int *mbval, int *nbval,
          int myrow, int mycol, int nprow, int npcol, long *maxMem) {
  int i__, ii, m, n, mb, nb, ierr[1];
  int lcm, np0, nq0, mp0, mq0, mg, ng, np, nq, mp, mq;
  long isw, ipw, ipiw, ipa, ipc;

  *maxMem = 0;
  for (i__ = 0; i__ < nmat; ++i__) {
    m = mval[i__];
    n = nval[i__];

/*           Make sure matrix information is correct */

      ierr[0] = 0;
      if (m < 1) {
        ierr[0] = 1;
      } else if (n < 1) {
        ierr[0] = 1;
      }

      if (ierr[0] > 0) {
        continue;
      }

      for (ii = 0; ii < nbmat; ++ii) { /* Loop over different block sizes */

        mb = mbval[ii];
        nb = nbval[ii];

/*              Make sure blocking sizes are legal */
        ierr[0] = 0;
        if (mb < 1) {
          ierr[0] = 1;
        } else if (nb < 1) {
          ierr[0] = 1;
        }

/*              Make sure no one had error */

        if (ierr[0] > 0) {
          continue;
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

        if (*maxMem < isw) *maxMem = isw;
      }
  }
  return 0;
}

int
MaxMem(int nprocs, int imrow, int imcol, int nmat, int *mval, int *nval, int nbmat, int *mbval,
       int *nbval, int ngrids, int *npval, int *nqval, long *maxMem) {
  int nprow, npcol, myrow, mycol;
  int j, ierr[1];
  long curMem;

  *maxMem = 0;
  for (j = 0; j < ngrids; ++j) {
    nprow = npval[j];
    npcol = nqval[j];

/*        Make sure grid information is correct */

    ierr[0] = 0;
    if (nprow < 1) {
     ierr[0] = 1;
    } else if (npcol < 1) {
      ierr[0] = 1;
    } else if (nprow * npcol > nprocs) {
      ierr[0] = 1;
    }

    if (ierr[0] > 0) {
      continue;
    }
    for (myrow = 0; myrow < nprow; myrow++)
      for (mycol = 0; mycol < npcol; mycol++) {
        CheckNode( imrow, imcol, nmat, mval, nval, nbmat, mbval, nbval, myrow, mycol, nprow,
                   npcol, &curMem );
        if (*maxMem < curMem) *maxMem = curMem;
      }
  }
  return 0;
}

#ifdef HPCC_MEMMAIN
#include <stdio.h>
int iceil_(int *n,int *d) {return *n>0 ? (*n+*d-1)/ *d : *n/ *d;}
int numroc_(int *n, int *nb, int *iproc, int *isrcproc, int *nprocs) {
  int ret_val, extrablks, mydist, nblocks;
  mydist = (*nprocs + *iproc - *isrcproc) % *nprocs;
  nblocks = *n / *nb;
  ret_val = nblocks / *nprocs * *nb;
  extrablks = nblocks % *nprocs;
  if (mydist < extrablks) {
    ret_val += *nb;
  } else if (mydist == extrablks) {
    ret_val += *n % *nb;
  }
  return ret_val;
}
int ilcm_(int *m, int *n) {
  int ret_val;
  int ia, iq, ir;
  if (*m >= *n) {
    ia = *m;
    ret_val = *n;
  } else {
    ia = *n;
    ret_val = *m;
  }
  for (;;) {
    iq = ia / ret_val;
    ir = ia - iq * ret_val;
    if (ir == 0) {
      ret_val = *m * *n / ret_val;
      return ret_val;
    }
    ia = ret_val;
    ret_val = ir;
  }
}
int
main(int argc, char *argv[]) {
  int n, nb, nprow, npcol, ng, lcm;
  int nval[1], nbval[1];
  long maxMem;

  if (argc <= 1) {
    printf( "Usage:\n%s n nb nprow npcol\n", argv[0] );
  }

  if (argc <= 1 || sscanf( argv[1], "%d", &n  ) != 1 || n < 1)  n = 50000;
  if (argc <= 2 || sscanf( argv[2], "%d", &nb ) != 1 || nb < 1) nb = 80;
  if (argc <= 3 || sscanf( argv[3], "%d", &nprow ) != 1 || nprow < 1) nprow = 8;
  if (argc <= 4 || sscanf( argv[4], "%d", &npcol ) != 1 || npcol < 1) npcol = nprow;

  nval[0] = n;
  nbval[0] = nb;

  CheckNode( 0, 0, 1, nval, nval, 1, nbval, nbval, 0, 0, nprow, npcol, &maxMem );

  printf( "n=%d nb=%d nprow=%d npcol=%d lcm(nprow,npcol)=%d\n%ld\n", n, nb, nprow, npcol,
          ilcm_(&nprow, &npcol), maxMem );

  ng = iceil_(&n, &nb);
  lcm = ilcm_(&nprow, &npcol);
  printf( "%d %d %d\n", ng, lcm, (iceil_(&ng, &lcm) << 1) * nb * iceil_(&ng, &lcm) * nb );
  printf( "%d %d\n", (iceil_(&ng, &lcm) << 1), iceil_(&ng, &lcm) );

  return 0;
}
#endif
