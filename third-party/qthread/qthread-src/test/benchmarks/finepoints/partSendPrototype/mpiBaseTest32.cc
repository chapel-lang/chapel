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

static void worker( unsigned long tid, unsigned long stop, void *arg ){
  arg_t *a = (arg_t *)arg;
	int rc;
	int iteration;
  char numIterations = a->numIterations;
  char *sendBuf = a->sendBuf;
  char *recvBuf = a->recvBuf;
  size_t threadPart = a->threadPart;
  int other = a->other;
	MPI_Request requests[2];

	for ( iteration = 0; iteration < numIterations; iteration++ ) {

    qt_barrier_enter(wait_on_me);

		rc = MPI_Isend( (char*) sendBuf + tid * threadPart, threadPart, MPI_CHAR, other, tid,
                    MPI_COMM_WORLD, &requests[ 0 ] );
		assert( rc == MPI_SUCCESS );

		rc = MPI_Irecv( (char*) recvBuf + tid * threadPart, threadPart, MPI_CHAR, other, tid,
                    MPI_COMM_WORLD, &requests[ 1 ] );
		assert( rc == MPI_SUCCESS );

		rc = MPI_Waitall( 2, requests , MPI_STATUS_IGNORE );
		assert( rc == MPI_SUCCESS );
	}
}

double doTest32( int rank, int numIterations, char* sendBuf, char* recvBuf, int numThreads, size_t threadPart )
{
  arg_t arg;
	int other = (rank + 1) % 2;
	double start = MPI_Wtime();

  arg.rank = rank;
  arg.numIterations = numIterations;
  arg.sendBuf = sendBuf;
  arg.recvBuf = recvBuf;
  arg.threadPart = threadPart;
  arg.other = other;

  qt_loop(0, numThreads, worker, &arg);

	return MPI_Wtime() - start;
}
