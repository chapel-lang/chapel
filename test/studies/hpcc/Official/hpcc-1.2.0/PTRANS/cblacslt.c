/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; -*- */
/*

cblacslt.c

-- V0.0 CBLACS Lite routines --
University of Tennessee, October, 2003
Written by Piotr Luszczek.

*/

#include <hpcc.h>

#include <mpi.h>

#include "cblacslt.h"

#define DPRN(i,v) do{printf(__FILE__ "(%d)@%d:" #v "=%g\n",__LINE__,i,(double)(v));fflush(stdout);}while(0)

/* ---------------------------------------------------------------------- */

/*FIXME: what about parameter checking: context, etc? have a macro too? */
#define CBLACS_INIT if (! CblacsInitialized) CblacsInit(); else if (CblacsFinalized) do{CblacsWarn();return;}while(0)
#define CBLACS_INIT1(v) if (! CblacsInitialized) CblacsInit();else if (CblacsFinalized)do{CblacsWarn();return(v);}while(0)
#define CblacsWarn() CblacsWarnImp( __FILE__, __LINE__ )

static int CblacsInitialized = 0, CblacsFinalized;

double
dcputime00(void) {return HPL_ptimer_cputime();}
double
dwalltime00(void) {return MPI_Wtime();}

static void
CblacsWarnImp(char *file, int line) {
  int rank;
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  printf( "%s(%d)@%d: CBLACS warning.\n", file, line, rank );
  fflush(stdout);
}

static struct {MPI_Comm comm, rowComm, colComm; unsigned int taken;} CblacsComms[10];
static int CblacsNComms;

#define CBLACS_CHK_CTXT(v) if (ctxt < 1 || ctxt > CblacsNComms) return v
static MPI_Comm
CblacsGetComm(int ctxt) {
  CBLACS_CHK_CTXT(MPI_COMM_NULL);
  return CblacsComms[ctxt - 1].comm;
}
static MPI_Comm
CblacsGetRowComm(int ctxt) {
  CBLACS_CHK_CTXT(MPI_COMM_NULL);
  return CblacsComms[ctxt - 1].rowComm;
}
static MPI_Comm
CblacsGetColComm(int ctxt) {
  CBLACS_CHK_CTXT(MPI_COMM_NULL);
  return CblacsComms[ctxt - 1].colComm;
}

static int
CblacsSetComm(int ctxt, MPI_Comm comm) {
  CBLACS_CHK_CTXT(-1);
  CblacsComms[ctxt - 1].comm = comm;
  return 0;
}
static int
CblacsSetRowComm(int ctxt, MPI_Comm comm) {
  CBLACS_CHK_CTXT(-1);
  CblacsComms[ctxt - 1].rowComm = comm;
  return 0;
}
static int
CblacsSetColComm(int ctxt, MPI_Comm comm) {
  CBLACS_CHK_CTXT(-1);
  CblacsComms[ctxt - 1].colComm = comm;
  return 0;
}

static int
CblacsNewCtxt() {
  int i;

  for (i = 1; i < CblacsNComms; i++)
    if (! CblacsComms[i].taken) {
      CblacsComms[i].taken = 1;
      return i + 1;
    }

  return 0;
}

static int
CblacsDeleteCtxt(int *ctxtP) {
  int idx = *ctxtP - 1;

  if (idx < 1 || idx >= CblacsNComms) {
    CblacsWarn();
    return -1;
  }
  if (0 == CblacsComms[idx].taken) {
    CblacsWarn();
    return -1;
  }

  if (MPI_COMM_NULL != CblacsComms[idx].colComm) MPI_Comm_free( &(CblacsComms[idx].colComm) );
  if (MPI_COMM_NULL != CblacsComms[idx].rowComm) MPI_Comm_free( &(CblacsComms[idx].rowComm) );
  if (MPI_COMM_NULL != CblacsComms[idx].comm)    MPI_Comm_free( &(CblacsComms[idx].comm) );

  CblacsComms[idx].taken = 0;

  *ctxtP = 0; /* deleted contexts are 0 */

  return 0;
}

/*
static void *
CblacsNewBuf(int count, int esize) {
  return malloc( count * esize );
}
*/
#define CblacsNewBuf(c,s) malloc((c)*(s))
#define CblacsDeleteBuf(b) free(b)

static int
CblacsInit() {
  int i, flag;

  if (MPI_SUCCESS != MPI_Initialized( &flag ) || ! flag) {CblacsWarn();return 1;}

  CblacsInitialized = 1;
  CblacsFinalized = 0;

  CblacsNComms = 10;
  for (i = 0; i < CblacsNComms; i++) {
    CblacsComms[i].comm    = MPI_COMM_NULL;
    CblacsComms[i].rowComm = MPI_COMM_NULL;
    CblacsComms[i].colComm = MPI_COMM_NULL;
    CblacsComms[i].taken   = 0;
  }
  /* FIXME: setup system context to be a cartesian grid with row and column comm's*/
  CblacsComms[0].comm    = MPI_COMM_WORLD;
  CblacsComms[0].rowComm = MPI_COMM_NULL;
  CblacsComms[0].colComm = MPI_COMM_NULL;
  CblacsComms[0].taken   = 1;

  return 0;
}

void
Cblacs_pinfo(int *mypnum, int *nprocs) {
  CBLACS_INIT;
  MPI_Comm_rank( MPI_COMM_WORLD, mypnum );
  MPI_Comm_size( MPI_COMM_WORLD, nprocs );
}

void
Cblacs_exit(int NotDone) {
  CBLACS_INIT;
  CblacsFinalized = 0;
  if (! NotDone) MPI_Finalize();
}

void
Cblacs_abort(int ConTxt, int ErrNo) {
  int nprow, npcol, myrow, mycol, rank;

  CBLACS_INIT;

  MPI_Comm_rank( MPI_COMM_WORLD, &rank );

  Cblacs_gridinfo(ConTxt, &nprow, &npcol, &myrow, &mycol);
  fprintf(stderr, "{%d,%d}, pnum=%d, Contxt=%d, killed other procs, exiting with error #%d.\n\n",
          myrow, mycol, rank, ConTxt, ErrNo);

  fflush(stderr);
  fflush(stdout);
  MPI_Abort( MPI_COMM_WORLD, ErrNo );
}

void
Cblacs_get(int ConTxt, int what, int *val) {
  CBLACS_INIT;

  switch (what) {
    case SGET_SYSCONTXT:
      *val = 1;
      break;
    default:
      *val = -1;
      CblacsWarn();
      break;
  }
}

static int
CblacsGridNew(int nprow, int npcol, int *ConTxt, MPI_Comm *comm) {
  int size;

  CBLACS_INIT1(-1);

  *comm = CblacsGetComm(*ConTxt);
  if (MPI_COMM_NULL == *comm) return -1;

  MPI_Comm_size( *comm, &size );
  if (nprow < 1 || nprow > size) return -1;
  if (npcol < 1 || npcol > size) return -1;
  if (nprow * npcol > size) return -1;

  *ConTxt = CblacsNewCtxt();

  return 0;
}

void
Cblacs_gridmap(int *ConTxt, int *umap, int ldumap, int nprow, int npcol) {
  int i, j, np_me, npall, npwho, myrow, mycol, color, key, rv;
  MPI_Comm comm, newComm, rowComm, colComm;

  if (CblacsGridNew( nprow, npcol, ConTxt, &comm )) {
    CblacsWarn();
    goto gmapErr;
  }

  Cblacs_pinfo( &np_me, &npall );

  myrow = mycol = -1;
  color = MPI_UNDEFINED;
  key = 0;
  for (i = 0; i < nprow; ++i)
    for (j = 0; j < npcol; ++j) {
      npwho = umap[j + i * ldumap];
      if (np_me == npwho) {
        color = 0;
        key = j + i * npcol;
        myrow = i;
        mycol = j;
        goto gmapFound;
      }
    }

  gmapFound:

  /* communicator of all grid processes */
  rv = MPI_Comm_split( comm, color, key, &newComm );
  if (MPI_SUCCESS != rv) {
    /* make contexts for non-participating processes a 0 value so gridinfo() works correctly */
    CblacsDeleteCtxt( ConTxt );
    goto gmapErr;
  }
  CblacsSetComm( *ConTxt, newComm );
  if (MPI_COMM_NULL == newComm) { /* this process does not participate in this grid */
    CblacsDeleteCtxt( ConTxt );
    return;
  }

  /* row communicator */
  rv = MPI_Comm_split( newComm, myrow, mycol, &rowComm );
  if (MPI_SUCCESS != rv) {
    CblacsDeleteCtxt( ConTxt );
    goto gmapErr;
  }
  CblacsSetRowComm( *ConTxt, rowComm );

  /* column communicator */
  rv = MPI_Comm_split( newComm, mycol, myrow, &colComm );
  if (MPI_SUCCESS != rv) {
    CblacsDeleteCtxt( ConTxt );
    goto gmapErr;
  }
  CblacsSetColComm( *ConTxt, colComm );

  return;

  gmapErr:

  *ConTxt = 0;
  CblacsWarn();
  return;
}

void
Cblacs_gridexit(int ConTxt) {
  CBLACS_INIT;
  CblacsDeleteCtxt( &ConTxt );
}

void
Cblacs_gridinfo(int ConTxt, int *nprow, int *npcol, int *myrow, int *mycol) {
  MPI_Comm comm;

  CBLACS_INIT;

  comm = CblacsGetComm( ConTxt );

  /* deleted contexts (or the contexts for non-participating processes) are 0 */
  if (MPI_COMM_NULL == comm) {
    *nprow = *npcol = *myrow = *mycol = -1;
  } else {
    MPI_Comm_size( CblacsGetRowComm(ConTxt), npcol );
    MPI_Comm_rank( CblacsGetRowComm(ConTxt), mycol );
    MPI_Comm_size( CblacsGetColComm(ConTxt), nprow );
    MPI_Comm_rank( CblacsGetColComm(ConTxt), myrow );
  }
}

/* ---------------------------------------------------------------------- */
/* Communication routines */

void
Cblacs_barrier(int ConTxt, char *scope) {
  MPI_Comm comm;
  CBLACS_INIT;

  switch (*scope) {
    case 'A': case 'a': comm = CblacsGetComm( ConTxt );    break;
    case 'C': case 'c': comm = CblacsGetColComm( ConTxt ); break;
    case 'R': case 'r': comm = CblacsGetRowComm( ConTxt ); break;
    default: comm = MPI_COMM_NULL; CblacsWarn(); break;
  }
  if (MPI_COMM_NULL == comm) {
    CblacsWarn();
    return;
  }

  MPI_Barrier( comm );
}

static void
Cvgred2d(int ConTxt, char *scope, int m, int n, void *A, int lda, int rowRank,
  int colRank, MPI_Datatype dtype, int dsize, MPI_Op op) {
  int j, rank, root, count, coords[2], dest_rank, npcol;
  void *sbuf, *rbuf;
  MPI_Comm comm;

  /* if the answer should be left on all processes */
  if (-1 == rowRank || -1 == colRank) root = 0;
  else root = 1;

  switch (*scope) {
    case 'A': case 'a':
      comm = CblacsGetComm( ConTxt );
      coords[0] = rowRank;
      coords[1] = colRank;
      MPI_Comm_size( CblacsGetRowComm( ConTxt ), &npcol );
      dest_rank = colRank + rowRank * npcol;
      break;
    case 'C': case 'c':
      comm = CblacsGetColComm( ConTxt );
      coords[0] = rowRank;
      dest_rank = rowRank;
      break;
    case 'R': case 'r':
      comm = CblacsGetRowComm( ConTxt );
      coords[0] = colRank;
      dest_rank = colRank;
      break;
    default: comm = MPI_COMM_NULL; CblacsWarn(); break;
  }
  if (MPI_COMM_NULL == comm) {
    CblacsWarn();
    return;
  }
  /* if not leave-on-all then get rank of the destination */
  if (root) root = dest_rank; /* MPI_Cart_rank( comm, coords, &root ); */
  else root = MPI_PROC_NULL;

  /* FIXME: what if contiguous buffer cannot be allocated */
  count = m * n;
  if (m == lda || n == 1) sbuf = A; /* A is contiguous, reuse it */
  else {
    /* a new data type could be created to reflect layout of `A' but then the
     * receiving buffer would have to be the same, and if `lda' is large in
     * comparison to `m' then it might be unfeasible */
    sbuf = CblacsNewBuf( count, dsize );
    for (j = 0; j < n; j++)
      memcpy( (char *)sbuf + j * m * dsize, (char *)A + j * lda * dsize, m * dsize );
  }
  rbuf = CblacsNewBuf( count, dsize );

  if (MPI_PROC_NULL == root) {
    MPI_Allreduce( sbuf, rbuf, count, dtype, op, comm );
  } else {
    MPI_Reduce( sbuf, rbuf, count, dtype, op, root, comm );
    MPI_Comm_rank( comm, &rank );
  }
  if (MPI_PROC_NULL == root || root == rank) {
    if (A == sbuf) memcpy( A, rbuf, count * dsize ); /* A is contiguous */
    else {
      for (j = 0; j < n; j++)
        memcpy( (char *)A + j * lda * dsize, (char *)rbuf + j * m * dsize, m * dsize );
    }
  }

  CblacsDeleteBuf( rbuf );
  if (sbuf != A) CblacsDeleteBuf( sbuf );
}

/*
 *  Purpose
 *
 *  Combine sum operation for double precision rectangular matrices.
 *
 *  Arguments
 *
 *  ConTxt  (input) int
 *          Index into MyConTxts00 (my contexts array).
 *
 *  scope   (input) Ptr to char
 *          Limit the scope of the operation.
 *          = 'R' :   Operation is performed by a process row
 *          = 'C' :   Operation is performed by a process column.
 *          = 'A' :   Operation is performed by all processes in grid.
 * If both `rdest' and `cdest' are not -1 then for 'R' scope `rdest' is ignored and for `C' - `cdest'
 * is ignored (row or column of the scope are used, respectively).
 *
 *  top     (input) Ptr to char
 *          Controls fashion in which messages flow within the operation.
 *
 *  m       (input) int
 *          The number of rows of the matrix A.  m >= 0.
 *
 *  n       (input) int
 *          The number of columns of the matrix A.  n >= 0.
 *
 *  A       (output) Ptr to double precision two dimensional array
 *          The m by n matrix A.  Fortran 77 (column-major) storage
 *          assumed.
 *
 *  lda     (input) int
 *          The leading dimension of the array A.  lda >= m.
 *
 *  rdest   (input) int
 *          The process row of the destination of the sum.
 *          If rdest == -1, then result is left on all processes in scope.
 *
 *  cdest   (input) int
 *          The process column of the destination of the sum.
 *          If cdest == -1, then result is left on all processes in scope.
 */
void
Cdgsum2d(int ConTxt, char *scope, char *top, int m, int n, double *A, int lda, int rdest, int cdest){
  CBLACS_INIT;
  top = top; /* user `top'ology is ignored */
  Cvgred2d( ConTxt, scope, m, n, A, lda, rdest, cdest, MPI_DOUBLE, sizeof(double), MPI_SUM );
}
void
Cigsum2d(int ConTxt, char *scope, char *top, int m, int n, int *A, int lda, int rdest, int cdest){
  CBLACS_INIT;
  top = top; /* user `top'ology is ignored */
  Cvgred2d( ConTxt, scope, m, n, A, lda, rdest, cdest, MPI_INT, sizeof(int), MPI_SUM );
}

void
CblacsAbsMax(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype) {
  int i, n = *len; double *dinvec, *dinoutvec;
  if (MPI_DOUBLE == *datatype) {
    dinvec = (double *)invec;
    dinoutvec = (double *)inoutvec;
    for (i = n; i; i--, dinvec++, dinoutvec++)
      if (fabs(*dinvec) > fabs(*dinoutvec)) *dinoutvec = *dinvec;
  } else
    CblacsWarn();
}
void
CblacsAbsMin(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype) {
  int i, n = *len; double *dinvec, *dinoutvec;
  if (MPI_DOUBLE == *datatype) {
    dinvec = (double *)invec;
    dinoutvec = (double *)inoutvec;
    for (i = n; i; i--, dinvec++, dinoutvec++)
      if (fabs(*dinvec) < fabs(*dinoutvec)) *dinoutvec = *dinvec;
  } else
    CblacsWarn();
}

/*
 *  Purpose
 *
 *  Combine amx operation for double precision rectangular matrices.
 *
 *  Arguments
 *
 *  ConTxt  (input) Ptr to int
 *          Index into MyConTxts00 (my contexts array).
 *
 *  SCOPE   (input) Ptr to char
 *          Limit the scope of the operation.
 *          = 'R' :   Operation is performed by a process row.
 *          = 'C' :   Operation is performed by a process column.
 *          = 'A' :   Operation is performed by all processes in grid.
 *
 *  TOP     (input) Ptr to char
 *          Controls fashion in which messages flow within the operation.
 *
 *  M       (input) Ptr to int
 *          The number of rows of the matrix A.  M >= 0.
 *
 *  N       (input) Ptr to int
 *          The number of columns of the matrix A.  N >= 0.
 *
 *  A       (output) Ptr to double precision two dimensional array
 *          The m by n matrix A.  Fortran77 (column-major) storage
 *          assumed.
 *
 *  LDA     (input) Ptr to int
 *          The leading dimension of the array A.  LDA >= M.
 *
 *  RA      (output) Integer Array, dimension (LDIA, N)
 *          Contains process row that the amx of each element
 *          of A was found on: i.e., rA(1,2) contains the process
 *          row that the amx of A(1,2) was found on.
 *          Values are left on process {rdest, cdest} only, others
 *          may be modified, but not left with interesting data.
 *          If rdest == -1, then result is left on all processes in scope.
 *          If LDIA == -1, this array is not accessed, and need not exist.
 *
 *  CA      (output) Integer Array, dimension (LDIA, N)
 *          Contains process column that the amx of each element
 *          of A was found on: i.e., cA(1,2) contains the process
 *          column that the max/min of A(1,2) was found on.
 *          Values are left on process {rdest, cdest} only, others
 *          may be modified, but not left with interesting data.
 *          If rdest == -1, then result is left on all processes in scope.
 *          If LDIA == -1, this array is not accessed, and need not exist.
 *
 *  LDIA    (input) Ptr to int
 *          If (LDIA == -1), then the arrays RA and CA are not accessed.
 *          ELSE leading dimension of the arrays RA and CA.  LDIA >= M.
 *
 *  RDEST   (input) Ptr to int
 *          The process row of the destination of the amx.
 *          If rdest == -1, then result is left on all processes in scope.
 *
 *  CDEST   (input) Ptr to int
 *          The process column of the destination of the amx.
 *          If rdest == -1, then CDEST ignored.
 */
void
Cdgamx2d(int ConTxt, char *scope, char *top, int m, int n, double *A, int lda, int *rA, int *cA,
  int ldia, int rdest, int cdest) {
  MPI_Op op;
  CBLACS_INIT;
  if (ldia > 0) {CblacsWarn(); rA = cA; return;} /* no AMAX_LOC yet */
  MPI_Op_create( CblacsAbsMax, 1, &op );
  top = top; /* user `top'ology is ignored */
  Cvgred2d( ConTxt, scope, m, n, A, lda, rdest, cdest, MPI_DOUBLE, sizeof(double), op );
  MPI_Op_free( &op );
}
void
Cdgamn2d(int ConTxt, char *scope, char *top, int m, int n, double *A, int lda, int *rA, int *cA,
  int ldia, int rdest, int cdest) {
  MPI_Op op;
  CBLACS_INIT;
  if (ldia > 0) {CblacsWarn(); rA = cA; return;} /* no AMAX_LOC yet */
  MPI_Op_create( CblacsAbsMin, 1, &op );
  top = top; /* user `top'ology is ignored */
  Cvgred2d( ConTxt, scope, m, n, A, lda, rdest, cdest, MPI_DOUBLE, sizeof(double), op );
  MPI_Op_free( &op );
}

void
Cblacs_dSendrecv(int ctxt, int mSrc, int nSrc, double *Asrc, int ldaSrc, int rdest, int cdest,
  int mDest, int nDest, double *Adest, int ldaDest, int rsrc, int csrc) {
  MPI_Comm comm, rowComm;
  MPI_Datatype typeSrc, typeDest;
  MPI_Status stat;
  int src, dest, dataIsContiguousSrc, dataIsContiguousDest, countSrc, countDest, npcol;

  CBLACS_INIT;

  comm = CblacsGetComm( ctxt );
  if (MPI_COMM_NULL == comm) {CblacsWarn(); return;}

  if (mSrc == ldaSrc || 1 == nSrc) {
    dataIsContiguousSrc = 1;
    countSrc = mSrc * nSrc;
    typeSrc = MPI_DOUBLE;
  } else {
    dataIsContiguousSrc = 0;
    countSrc = 1;
    MPI_Type_vector( nSrc, mSrc, ldaSrc, MPI_DOUBLE, &typeSrc );
    MPI_Type_commit( &typeSrc );
  }
  if (mDest == ldaDest || 1 == nDest) {
    dataIsContiguousDest = 1;
    countDest = mDest * nDest;
    typeDest = MPI_DOUBLE;
  } else {
    dataIsContiguousDest = 0;
    countDest = 1;
    MPI_Type_vector( nDest, mDest, ldaDest, MPI_DOUBLE, &typeDest );
    MPI_Type_commit( &typeDest );
  }

  rowComm = CblacsGetRowComm( ctxt );
  MPI_Comm_size( rowComm, &npcol );
  dest = cdest + rdest * npcol;
  src  = csrc + rsrc * npcol;

  MPI_Sendrecv( Asrc, countSrc, typeSrc, dest, 0, Adest, countDest, typeDest, src, 0, comm,
                &stat ); /* IBM's (old ?) MPI doesn't have: MPI_STATUS_IGNORE */

  if (! dataIsContiguousSrc) MPI_Type_free( &typeSrc );
  if (! dataIsContiguousDest) MPI_Type_free( &typeDest );
}

static void
CblacsBcast(int ConTxt, char *scope, int m, int n, void *A, int lda, int rowRank, int colRank,
  MPI_Datatype baseType){
  MPI_Comm comm;
  MPI_Datatype type;
  int root, coords[2], dest_rank, npcol;

  /* if this process is the root of broadcast */
  if (-1 == rowRank || -1 == colRank) root = 0;
  else root = 1;

  switch (*scope) {
    case 'A': case 'a':
      comm = CblacsGetComm( ConTxt );
      coords[0] = rowRank;
      coords[1] = colRank;
      MPI_Comm_size( CblacsGetRowComm( ConTxt ), &npcol );
      dest_rank = colRank + rowRank * npcol;
      break;
    case 'C': case 'c':
      comm = CblacsGetColComm( ConTxt );
      coords[0] = rowRank;
      dest_rank = rowRank;
      break;
    case 'R': case 'r':
      comm = CblacsGetRowComm( ConTxt );
      coords[0] = colRank;
      dest_rank = colRank;
      break;
    default: comm = MPI_COMM_NULL; CblacsWarn(); break;
  }
  if (MPI_COMM_NULL == comm) {
    CblacsWarn();
    return;
  }
  if (MPI_COMM_NULL == comm) {
    CblacsWarn();
    return;
  }

  /* if broadcast/receive */
  if (root) root = dest_rank; /* MPI_Cart_rank( comm, coords, &root ); */
  else MPI_Comm_rank( comm, &root ); /* else broadcast/send - I'm the root */

  MPI_Type_vector( n, m, lda, baseType, &type );
  MPI_Type_commit( &type );

  MPI_Bcast( A, 1, type, root, comm );

  MPI_Type_free( &type );
}

/*
 *  Purpose
 *
 *  Broadcast/send for general double precision arrays.
 *
 *  Arguments
 *
 *  ConTxt  (input) Ptr to int
 *          Index into MyConTxts00 (my contexts array).
 *
 *  SCOPE   (input) Ptr to char
 *          Limit the scope of the operation.
 *          = 'R' :   Operation is performed by a process row.
 *          = 'C' :   Operation is performed by a process column.
 *          = 'A' :   Operation is performed by all processes in grid.
 *
 *  TOP     (input) Ptr to char
 *          Controls fashion in which messages flow within the operation.
 *
 *  M       (input) Ptr to int
 *          The number of rows of the matrix A.  M >= 0.
 *
 *  N       (input) Ptr to int
 *          The number of columns of the matrix A.  N >= 0.
 *
 *  A       (input) Ptr to double precision two dimensional array
 *          The m by n matrix A.  Fortran77 (column-major) storage
 *          assumed.
 *
 *  LDA     (input) Ptr to int
 *          The leading dimension of the array A.  LDA >= M.
 */
void
Cdgebs2d(int ConTxt, char *scope, char *top, int m, int n, double *A, int lda) {
  CBLACS_INIT;
  top = top; /* user `top'ology is ignored */
  CblacsBcast( ConTxt, scope, m, n, A, lda, -1, -1, MPI_DOUBLE );
}

/*
 *  Purpose
 *
 *  Broadcast/receive for general double precision arrays.
 *
 *  Arguments
 *
 *  ConTxt  (input) Ptr to int
 *          Index into MyConTxts00 (my contexts array).
 *
 *  SCOPE   (input) Ptr to char
 *          Limit the scope of the operation.
 *          = 'R' :   Operation is performed by a process row.
 *          = 'C' :   Operation is performed by a process column.
 *          = 'A' :   Operation is performed by all processes in grid.
 *
 *  TOP     (input) Ptr to char
 *          Controls fashion in which messages flow within the operation.
 *
 *  M       (input) Ptr to int
 *          The number of rows of the matrix A.  M >= 0.
 *
 *  N       (input) Ptr to int
 *          The number of columns of the matrix A.  N >= 0.
 *
 *  A       (output) Ptr to double precision two dimensional array
 *          The m by n matrix A.  Fortran77 (column-major) storage
 *          assumed.
 *
 *  LDA     (input) Ptr to int
 *          The leading dimension of the array A.  LDA >= M.
 *
 *
 *  RSRC    (input) Ptr to int
 *          The process row of the source of the matrix.
 *
 *  CSRC    (input) Ptr to int
 *          The process column of the source of the matrix.
 */
void
Cdgebr2d(int ConTxt, char *scope, char *top, int m, int n, double *A, int lda, int rsrc, int csrc) {
  CBLACS_INIT;
  top = top; /* user `top'ology is ignored */
  CblacsBcast( ConTxt, scope, m, n, A, lda, rsrc, csrc, MPI_DOUBLE );
}
void
Cigebs2d(int ConTxt, char *scope, char *top, int m, int n, int *A, int lda) {
  CBLACS_INIT;
  top = top; /* user `top'ology is ignored */
  CblacsBcast( ConTxt, scope, m, n, A, lda, -1, -1, MPI_INT );
}
void
Cigebr2d(int ConTxt, char *scope, char *top, int m, int n, int *A, int lda, int rsrc, int csrc) {
  CBLACS_INIT;
  top = top; /* user `top'ology is ignored */
  CblacsBcast( ConTxt, scope, m, n, A, lda, rsrc, csrc, MPI_INT );
}
