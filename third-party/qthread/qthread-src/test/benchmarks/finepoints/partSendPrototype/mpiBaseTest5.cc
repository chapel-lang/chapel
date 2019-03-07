#include <time.h>
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

static long sleep1;
static long sleepPlus;

static qt_barrier_t *wait_on_me;

static void worker( unsigned long tid, unsigned long, void *arg ){
  arg_t *a = (arg_t *)arg;
  int rank = a->rank;
	int numIterations = a->numIterations;
  char *sendBuf = a->sendBuf;
  char *recvBuf = a->recvBuf;
	int numThreads = a->numThreads;
	size_t threadPart = a->threadPart;
  int other = (rank + 1) % 2;
  int rc;
  int iteration;


  struct timespec req,rem;
  req.tv_sec = 0;

  if ( numThreads > 1 && tid == numThreads - 1 ) {
    req.tv_nsec = sleepPlus;
  } else {
    req.tv_nsec = sleep1;
  }
  size_t bufSize = numThreads * threadPart;
  for ( iteration = 0; iteration < numIterations; iteration++ ) {

    if ( 0 == rank ) {

      qt_barrier_enter(wait_on_me);

      //      rc = clock_nanosleep(CLOCK_REALTIME,0,&req, &rem);
      if ( 0  !=  rc ) {
        printf("rc=%s rem %li\n",strerror(rc),rem.tv_nsec);
      }


      if ( tid == 0 ) {
        rc = MPI_Send( (char*) sendBuf, bufSize, MPI_CHAR, other, 0xdead, MPI_COMM_WORLD );
        assert( rc == MPI_SUCCESS );
      }
    } else {

      qt_barrier_enter(wait_on_me);
      if ( tid == 0 ){
        rc = MPI_Recv( (char*) recvBuf, bufSize, MPI_CHAR, other, 0xdead, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        assert( rc == MPI_SUCCESS );
      }
    }
  }
}

double doTest5( int rank, int numIterations, char* sendBuf, char* recvBuf, int numThreads, size_t threadPart, double compTime, double noise )
{
  // each thread
  arg_t arg;
  double start,duration;

  start = MPI_Wtime();

  sleep1 = compTime * 1000000000;
  sleepPlus = (compTime + ( compTime * noise)) * 1000000000 ;

  qt_loop(0, numThreads, worker, &arg);

  duration = MPI_Wtime() - start;
  if ( numThreads > 1 ) {
    duration -=  sleepPlus / 1000000000.0 * numIterations;
  } else {
    duration -=  sleep1 / 1000000000.0 * numIterations;
  }

  return duration;
}
