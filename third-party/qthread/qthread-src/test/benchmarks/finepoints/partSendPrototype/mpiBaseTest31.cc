#include <qthread/qthread.h>
#include <qthread/barrier.h>
#include <qthread/qloop.h>
#include <mpi.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct args {
  int rank;
  int numIterations;
  char* sendBuf;
  char* recvBuf;
  int numThreads;
  size_t threadPart;
  int other;
  MPI_Request *requests;
} arg_t;

static qt_barrier_t *wait_on_me;
static double start;
extern int idx;

static void worker( unsigned long tid, unsigned long, void *arg) {
  arg_t *a = (arg_t*)arg;
  int rc;
  int iteration;
  int rank = a->rank;
  int numIterations = a->numIterations;
  char* sendBuf = a->sendBuf;
  char* recvBuf = a->recvBuf;
  //int numThreads = a->numThreads;
  size_t threadPart = a->threadPart;
  int other = a->other;
  idx = tid;

  if( tid == 0 )
    start = MPI_Wtime();

  for ( iteration = 0; iteration < numIterations; iteration++ ) {
    qt_barrier_enter(wait_on_me);
    if ( 0 == rank ) {
      rc = MPI_Send( (char*) sendBuf + tid * threadPart, threadPart, MPI_CHAR, other, tid, MPI_COMM_WORLD );
      assert( rc == MPI_SUCCESS );
    } else {
      rc = MPI_Recv( (char*) recvBuf + tid * threadPart, threadPart, MPI_CHAR, other, tid, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      assert( rc == MPI_SUCCESS );
    }
  }
}

double doTest31( int rank, int numIterations, char* sendBuf, char* recvBuf, int numThreads, size_t threadPart )
{
  arg_t arg;
  int other = (rank + 1) % 2;

  arg.rank = rank;
  arg.numIterations = numIterations;
  arg.sendBuf = sendBuf;
  arg.recvBuf = recvBuf;
  arg.numThreads = numThreads;
  arg.threadPart = threadPart;
  arg.other = other;
  //arg.requests = requests;

  qt_loop(0, numThreads, worker, &arg);

  return MPI_Wtime() - start;
}
