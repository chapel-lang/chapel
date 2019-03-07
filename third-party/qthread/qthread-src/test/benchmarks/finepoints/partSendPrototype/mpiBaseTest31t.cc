#include <vector>
#include <mpi.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#ifdef __APPLE__
#include "osxbarrier.h"
#endif

//#define PRINTF(args,...)  printf( args, ##__VA_ARGS__ )
#define PRINTF(args,...)

struct Info {
	int numIterations;
	int tid;
	int rank;
	int other;
	pthread_barrier_t* barrier;
	size_t threadPart;
	char* sendBuf;
	char* recvBuf;
};

void *work( void * info );
double doTest31t( int rank, int numIterations, char* sendBuf, char* recvBuf, int numThreads, size_t threadPart )
{
	double start;
	std::vector<pthread_t> threads(numThreads);
	int rc;
	std::vector<Info> infoV(numThreads);

	Info info;
	info.numIterations = numIterations;
	info.rank = rank;
	info.other = (rank + 1) % 2;
	info.sendBuf = sendBuf;
	info.recvBuf = recvBuf;
	info.threadPart = threadPart;

	pthread_barrier_t barrier;
	rc = pthread_barrier_init(&barrier,NULL, numThreads );

	info.barrier = &barrier;
	assert(0 == rc);

	for ( unsigned j = 1; j < threads.size(); j++) {

		infoV[j] = info;
		infoV[j].tid = j;
		rc = pthread_create( &threads[j], NULL, work, &infoV[j] );
		assert(0 == rc);
	}

	info.tid = 0;

	start = MPI_Wtime();

	work( &info );

    double val = MPI_Wtime() - start;

	for ( unsigned j = 1; j < threads.size(); j++) {
		pthread_join( threads[j], NULL );
	}

	PRINTF("%s():%d %d threadPart=%lu\n",__func__,__LINE__,rank,threadPart);

	return val;
}

void *work( void * x )
{
	Info* info = (Info*) x; 
	int rc;
	int iteration;
	int tid = info->tid;
	size_t threadPart = info->threadPart;
	int other = info->other;
	char* sendBuf = info->sendBuf;
	char* recvBuf = info->recvBuf;

	PRINTF("%s():%d rank=%d tid=%d enter\n",__func__,__LINE__,info->rank,tid);

 	for ( iteration = 0; iteration < info->numIterations; iteration++ ) {

		pthread_barrier_wait( info->barrier );

		if ( 0 == info->rank ) {
			rc = MPI_Send( (char*) sendBuf + tid * threadPart, threadPart, MPI_CHAR, other, tid, MPI_COMM_WORLD );
			assert( rc == MPI_SUCCESS );
		} else {
			rc = MPI_Recv( (char*) recvBuf + tid * threadPart, threadPart, MPI_CHAR, other, tid, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			assert( rc == MPI_SUCCESS );
		}
	}
	PRINTF("%s():%d rank=%d tid=%d done\n",__func__,__LINE__,info->rank,tid);

	return NULL;
}
