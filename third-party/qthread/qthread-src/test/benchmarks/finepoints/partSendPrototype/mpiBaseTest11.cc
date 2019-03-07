//#include <omp.h>
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

static void worker(unsigned long tid, unsigned long, void *arg) {
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
  MPI_Request *requests = a->requests;
  idx = tid;
  if ( 0 == rank ) {
    rc = MPI_Send_init( (char*) sendBuf + tid * threadPart, threadPart, MPI_CHAR, other, tid, MPI_COMM_WORLD, &requests[1] );
    assert( rc == MPI_SUCCESS );
  } else {
    rc = MPI_Recv_init( (char*) recvBuf + tid * threadPart, threadPart, MPI_CHAR, other, tid, MPI_COMM_WORLD, &requests[0] );
    assert( rc == MPI_SUCCESS );
  }

  if( tid == 0)
    start = MPI_Wtime();

  for ( iteration = 0; iteration < numIterations; iteration++ ) {

    if( tid == 0 )
      MPI_Barrier(MPI_COMM_WORLD);

    qt_barrier_enter(wait_on_me);

    if ( 0 == rank ) {
      rc = MPI_Start(&requests[1]);
      assert( rc == MPI_SUCCESS );
      rc = MPI_Wait( &requests[1], MPI_STATUS_IGNORE);
      assert( rc == MPI_SUCCESS );
    } else {
      rc = MPI_Start(&requests[0]);
      assert( rc == MPI_SUCCESS );
      rc = MPI_Wait( &requests[0], MPI_STATUS_IGNORE);
      assert( rc == MPI_SUCCESS );
    }
  }

  if ( 0 == rank ) {
    MPI_Request_free( &requests[1] );
  } else {
    MPI_Request_free( &requests[0] );
  }
}

double doTest11( int rank, int numIterations, char* sendBuf, char* recvBuf, int numThreads, size_t threadPart )
{
  // each thread
  arg_t arg;
  //int rc;
  //int iteration;
  //int tid;
  MPI_Request requests[2];
  int other = (rank + 1) % 2;

  double duration;
  arg.rank = rank;
  arg.numIterations = numIterations;
  arg.sendBuf = sendBuf;
  arg.recvBuf = recvBuf;
  arg.numThreads = numThreads;
  arg.threadPart = threadPart;
  arg.other = other;
  arg.requests = requests;

  qt_loop(0, numThreads, worker, &arg);
  duration = MPI_Wtime() - start;

  return duration;
}
