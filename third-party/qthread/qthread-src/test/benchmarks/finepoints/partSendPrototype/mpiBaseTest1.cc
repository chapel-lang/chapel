//#include <omp.h>
#include <mpi.h>
#include <qthread/qthread.h>
#include <qthread/barrier.h>
#include <qthread/qloop.h>
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

static void worker(unsigned long tid, unsigned long, void *arg) {
	arg_t *a = (arg_t*)arg;
	int rc;
	int iteration;
	//int rank = a->rank;
	int numIterations = a->numIterations;
	char* sendBuf = a->sendBuf;
	char* recvBuf = a->recvBuf;
	//int numThreads = a->numThreads;
	size_t threadPart = a->threadPart;
  int other = a->other;
	MPI_Request *requests = a->requests;

	rc = MPI_Recv_init( (char*) recvBuf + tid * threadPart, threadPart, MPI_CHAR, other, tid, MPI_COMM_WORLD, &requests[tid*2 + 0] );
	assert( rc == MPI_SUCCESS );

	rc = MPI_Send_init( (char*) sendBuf + tid * threadPart, threadPart, MPI_CHAR, other, tid, MPI_COMM_WORLD, &requests[tid*2 + 1] );
	assert( rc == MPI_SUCCESS );

	if(tid == 0)
		start = MPI_Wtime();

	for ( iteration = 0; iteration < numIterations; iteration++ ) {
		if(tid == 0) {
			rc = MPI_Barrier(MPI_COMM_WORLD);
			assert( rc == MPI_SUCCESS );
		}
		qt_barrier_enter(wait_on_me);

		rc = MPI_Start(&requests[tid*2 + 0]);
		assert( rc == MPI_SUCCESS );

		rc = MPI_Start(&requests[tid*2 + 1]);
		assert( rc == MPI_SUCCESS );

		rc = MPI_Waitall( 2, &requests[ tid*2 ], MPI_STATUS_IGNORE );
		assert( rc == MPI_SUCCESS );
	}

	MPI_Request_free( &requests[tid*2 + 0] );
	MPI_Request_free( &requests[tid*2 + 1] );

}

double doTest1( int rank, int numIterations, char* sendBuf, char* recvBuf, int numThreads, size_t threadPart )
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
	return  MPI_Wtime() - start;
}
