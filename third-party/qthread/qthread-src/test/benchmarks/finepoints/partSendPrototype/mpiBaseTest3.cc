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

static void worker( unsigned long tid, unsigned long stop, void *arg) {
  arg_t *a = (arg_t*)arg;
	int rc;
	int iteration;
  //int rank = a->rank;
  int numIterations = a->numIterations;
  char* sendBuf = a->sendBuf;
  char* recvBuf = a->recvBuf;
  unsigned long numThreads = a->numThreads;
  size_t threadPart = a->threadPart;
  int other = a->other;
  MPI_Request *requests = a->requests;
  idx = tid;
	if( tid == 0 ) {
		assert( numThreads == stop );
		start = MPI_Wtime();
	}

	for ( iteration = 0; iteration < numIterations; iteration++ ) {

		//printf("rank=%d thread=%d iteration=%d myPart=%lu\n",rank,tid,iteration,threadPart);
    qt_barrier_enter(wait_on_me);
    if( tid == 0 ) {
      rc = MPI_Isend( (char*) sendBuf + tid * threadPart, threadPart, MPI_CHAR, other, tid,
                      MPI_COMM_WORLD, &requests[ tid * 2 ] );
      assert( rc == MPI_SUCCESS );
    }
		rc = MPI_Irecv( (char*) recvBuf + tid * threadPart, threadPart, MPI_CHAR, other, tid, 
                    MPI_COMM_WORLD, &requests[ tid * 2 + 1] );
		assert( rc == MPI_SUCCESS );

		rc = MPI_Waitall( 2, &requests[tid*2] , MPI_STATUS_IGNORE );
		assert( rc == MPI_SUCCESS );
	}
}

double doTest3( int rank, int numIterations, char* sendBuf, char* recvBuf, int numThreads, size_t threadPart )
{
  arg_t arg;
	int other = (rank + 1) % 2;
	MPI_Request requests[2*numThreads];

  arg.rank = rank;
  arg.numIterations = numIterations;
  arg.sendBuf = sendBuf;
  arg.recvBuf = recvBuf;
  arg.numThreads = numThreads;
  arg.threadPart = threadPart;
  arg.other = other;
  arg.requests = requests;

  qt_loop(0, numThreads, worker, &arg);

	return MPI_Wtime() - start;
}
