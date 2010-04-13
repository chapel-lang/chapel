#include "args.h"
#include "ra.h"
#include "rand.h"
#include "timer.h"
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int n = 28, N_U_log2 = 19;
u64 N_U, m, idxMask;
const double errorTolerance = 1e-2;

int rank, nprocs;
int doneCount = 0;

MPI_Request recvReq;
u64 recvBuf[2];

static int idxToLocale(u64 ind);
static int ind2localIdx(u64 ind);
static void tryRecv(void);
static void doSend(int dest, u64 val, u64 finish);

u64* A;

/* Given a global index, return the process rank it is local on */
static int idxToLocale(u64 ind) {
  return nprocs*ind / m;
}

/* Given an index that is known to be on this process,
 * convert it to a local index.
 */
static int ind2localIdx(u64 ind) {
  u64 blockSize = m / nprocs;
  return ind - blockSize*rank;
}

/* See if the currently pending Irecv has completed. If yes, then do the
 * local update and start a new Irecv. If recvBuf[1] is non-zero, it means
 * no more messages will be sent from that process, and recvBuf[0] should
 * be ignored. 
 */
static void tryRecv() {
  int recvFlag;
  MPI_Status recvStat;

  if (doneCount == nprocs)
    return;

  MPI_Test(&recvReq, &recvFlag, &recvStat);

  if (recvFlag) {
    if (recvBuf[1] == 1) {
      doneCount++;
    } else {
      A[ind2localIdx(recvBuf[0] & idxMask)] ^= recvBuf[0];
    }
    if (doneCount != nprocs) {
      MPI_Irecv(recvBuf, 2, MPI_UNSIGNED_LONG_LONG, MPI_ANY_SOURCE,
                1, MPI_COMM_WORLD, &recvReq);
    }
  }
}

/* Send a message to dest containing the two u64 values.
 */
static void doSend(int dest, u64 val, u64 finish) {
  MPI_Request sendReq;
  MPI_Status sendStat;
  int sendFlag;
  u64 buf[2];

  buf[0] = val;
  buf[1] = finish;
  MPI_Send(buf, 2, MPI_UNSIGNED_LONG_LONG, dest, 1, MPI_COMM_WORLD);
  tryRecv();
}

int main(int argc, char* argv[]) {
  u64 i, locN_U, locStart, locEnd, r, base;
  int iter, errorCnt = 0, totalErrors;
  double starttime, finishtime, exectime;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  parseArgs(argc, argv);

  if (rank == 0) {
    printf("Number of nodes = %d\n", nprocs); fflush(stdout);
    printf("Problem size = %lld (2**%d)\n", m, n); fflush(stdout);
    printf("Number of updates = %lld (2**%d)\n", N_U, N_U_log2); fflush(stdout);
  }

  // initialize the random number generator
  randInit();

  A = malloc(sizeof(u64)*m/nprocs);
  if (A == NULL) {
    fprintf(stderr, "malloc failed for main array.\n"); fflush(stderr);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  // Initialize the array so each element is its own global index.
  base = rank * (m / nprocs);
  for (i=0; i < m / nprocs; i++) {
    A[i] = i + base;
  }

  MPI_Barrier(MPI_COMM_WORLD);

  // The first iteration does the benchmark, the second iteration
  // reverses it for validation.
  for (iter=0; iter < 2; iter++) {
    doneCount = 0;

    if (iter == 0) {
      starttime = now_time();
    }

    MPI_Irecv(recvBuf, 2, MPI_UNSIGNED_LONG_LONG, MPI_ANY_SOURCE,
              1, MPI_COMM_WORLD, &recvReq);

    locN_U = N_U / nprocs;
    locStart = locN_U * rank;
    locEnd = locStart + locN_U;
    r = getNthRandom(locStart);

    for (i=locStart; i < locEnd; i++) {
      int loc = idxToLocale(r & idxMask);
      tryRecv();
      if (loc == rank) {
        A[ind2localIdx(r & idxMask)] ^= r;
      } else {
        doSend(loc, r, 0);
      }
      tryRecv();
      getNextRandom(&r);
    }
    // Send a message to each process to indicate that this process won't
    // do any more sends.
    for (i = 0; i < nprocs; i++) {
      doSend(i, 0, 1);
    }

    while (doneCount != nprocs) {
      tryRecv();
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (iter == 0) {
      finishtime = now_time();
      exectime = (finishtime - starttime) / 1.0e+6;
    }
  }

  for (i=0; i < m/nprocs; i++) {
    if (A[i] != i + base) {
      errorCnt++;
    }
  }

  MPI_Reduce(&errorCnt, &totalErrors, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    printf("Number of errors: %d\n", totalErrors); fflush(stdout);
    if (totalErrors <= errorTolerance * N_U) {
      printf("Validation: SUCCESS\n"); fflush(stdout);
    } else {
      printf("Validation: FAILURE\n"); fflush(stdout);
    }
    printf("Execution time = %lf\n", exectime); fflush(stdout);
    printf("Performance (GUPS) = %lf\n", (N_U / exectime) * 1.0e-9); fflush(stdout);
  }

  MPI_Finalize();

  return 0;
}

