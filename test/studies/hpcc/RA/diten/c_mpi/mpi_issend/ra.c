#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include "args.h"
#include "rand.h"
#include "ra.h"

int n = 28;
int N_U_log2 = 19;

unsigned long long N_U;
unsigned long long m;
unsigned long long idxMask;
const double errorTolerance = 1e-2;

int rank, nprocs;

int doneCount = 0;
int doneWithRecvs = 0;

rndtype* A;

/* Given a global index, return the process rank it is local to */
int idxToLocale(unsigned long long ind) {
  return nprocs*ind / m;
}

/* Given an index that is known to be on this process,
 * convert it to a local index.
 */
int ind2localIdx(unsigned long long ind) {
  unsigned long long blockSize = m / nprocs;
  return ind - blockSize*rank;
}

void tryRecv(MPI_Request* recvReq, rndtype recvBuf[]) {
  int recvFlag;
  MPI_Status recvStat;

  MPI_Test(recvReq, &recvFlag, &recvStat);
  if (recvFlag) {
    if (recvBuf[1] == 1) {
      doneCount++;
      if (doneCount == nprocs) {
        doneWithRecvs = 1;
      }
    } else {
      A[ind2localIdx(recvBuf[0] & idxMask)] ^= recvBuf[0];
    }
    if (!doneWithRecvs) {
      MPI_Irecv(recvBuf, 2*sizeof(rndtype), MPI_BYTE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, recvReq);
    }
  }
}

void dosend(int dest, rndtype val, rndtype finish, MPI_Request* recvReq, rndtype recvBuf[]) {
  MPI_Request sendReq;
  int sendFlag, recvFlag;
  MPI_Status sendStat, recvStat;
  rndtype buf[2];
  buf[0] = val;
  buf[1] = finish;

  MPI_Issend(buf, 2*sizeof(rndtype), MPI_BYTE, dest, 1, MPI_COMM_WORLD, &sendReq);

  do {
    MPI_Test(&sendReq, &sendFlag, &sendStat);
    if (!doneWithRecvs) {
      tryRecv(recvReq, recvBuf);
    }
  } while (!sendFlag);
}

double now_time(void) {
  struct tm * now;
  struct timeval t;
  gettimeofday(&t, NULL);
  now = localtime(&t.tv_sec);
  return (double)(now->tm_hour)*3600.0e+6 +
    (double)(now->tm_min)*60.0e+6 +
    (double)(now->tm_sec)*1.0e+6 +
    (double)(t.tv_usec);
}

int main(int argc, char* argv[]) {
  unsigned long long i, locN_U, locStart, locEnd;
  rndtype r, recvBuf[2];
  MPI_Request recvReq;
  int base, iter, errorCnt = 0, totalErrors;
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
  randInit();

  A = malloc(sizeof(rndtype)*m/nprocs);
  if (A == NULL) {
    printf("malloc failed for main array.\n"); fflush(stdout);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  
  base = rank * (m / nprocs);
  for (i=0; i < m / nprocs; i++) {
    A[i] = i + base;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  // first iteration does the benchmark, second reverses it for validation.
  for (iter=0; iter < 2; iter++) {
    doneCount = 0;
    doneWithRecvs = 0;

    if (iter == 0) {
      starttime = now_time();
    }

    MPI_Irecv(recvBuf, 2*sizeof(rndtype), MPI_BYTE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &recvReq);

    locN_U = N_U / nprocs;
    locStart = locN_U * rank;
    locEnd = locStart+locN_U;
    r = getNthRandom(locStart);
    for (i=locStart; i < locEnd; i++) {
      int loc = idxToLocale(r & idxMask);
      if (loc < 0 || loc > nprocs-1) {
        printf("error: r=%llu, r&mask=%llu, loc=%d\n", r, r&idxMask, loc);
      }
      if (loc == rank) {
        A[ind2localIdx(r & idxMask)] ^= r;
      } else {
        dosend(loc, r, 0, &recvReq, recvBuf);
      }
      getNextRandom(&r);
    }
    for (i = 0; i < nprocs; i++) {
      dosend(i, 0, 1, &recvReq, recvBuf);
    }
    while (!doneWithRecvs) {
      tryRecv(&recvReq, recvBuf);
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

