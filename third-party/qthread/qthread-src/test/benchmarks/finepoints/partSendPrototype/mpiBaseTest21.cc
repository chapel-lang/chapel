#include <qthread/qthread.h>
#include <qthread/barrier.h>
#include <qthread/qloop.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mpiPart.h"
#include <cstdlib>
#include <random>
#include <ctime>
#include <time.h>

typedef struct args {
  int rank;
  int numIterations;
  char* sendBuf;
  char* recvBuf;
  int numThreads;
  size_t threadPart;
  int other;
  //  MPI_Request *requests;
  MPI_Request sendReq;
  MPI_Request recvReq;
} arg_t;

static long sleep1;
static long sleepPlus;
static qt_barrier_t *wait_on_me;

static void worker( unsigned long tid, unsigned long, void *arg)	{
  arg_t *a = (arg_t*)arg;
  int rc;
  int iteration;
  int rank = a->rank;
  int numIterations = a->numIterations;
  char* sendBuf = a->sendBuf;
  //char* recvBuf = a->recvBuf;
  unsigned long numThreads = a->numThreads;
  size_t threadPart = a->threadPart;
  //int other = a->other;

  MPI_Request sendReq = a->sendReq;
  MPI_Request recvReq = a->recvReq;


  struct timespec req,rem;
  req.tv_sec = 0;
  if ( numThreads > 1 && tid == numThreads - 1 ) {
    req.tv_nsec = sleepPlus;
  } else {
    req.tv_nsec = sleep1;
  }

  for ( iteration = 0; iteration < numIterations; iteration++ ) {

#if 0
    if ( tid == 0 )
      MPI_Barrier( MPI_COMM_WORLD );
#endif

    if ( 0 == rank ) {
      if ( tid == 0 )  {
        rc = MPI_Start_part(&sendReq);
        assert( rc == MPI_SUCCESS );
      }
      qt_barrier_enter(wait_on_me);
      rc = nanosleep(&req, &rem);
      //rc = clock_nanosleep(CLOCK_REALTIME,0,&req, &rem);
      if ( 0  !=  rc ) {
      }

      rc = MPI_Partitioned_Add_to_buffer( &sendReq, sendBuf + (threadPart * tid), threadPart, MPI_CHAR );
      assert( rc  == MPI_SUCCESS );

      qt_barrier_enter(wait_on_me);
    }

    if ( 1 == rank ) {
      if ( tid == 0 ) {
        rc = MPI_Wait_part(&recvReq, MPI_STATUS_IGNORE );
        assert( rc == MPI_SUCCESS );
#ifdef VERIFY
        for ( int i = 0; i < (threadPart * numThreads) / 8; i++ ) {
          assert( ((uint64_t*)recvBuf)[i] == i );
        }
        bzero( recvBuf, threadPart * numThreads );
#endif
      }
    }
  }
}
//#define VERIFY 1
double doTest21( int rank, int numIterations, char* sendBuf, char* recvBuf, int numThreads, size_t threadPart, double compTime, double noise )
{
  arg_t arg;
  MPI_Request sendReq,recvReq;
  int other = (rank + 1) % 2;
  double start;
  int TAG = 0xdead;
  int rc;

  arg.rank = rank;
  arg.numIterations = numIterations;
  arg.sendBuf = sendBuf;
  arg.recvBuf = recvBuf;
  arg.numThreads = numThreads;
  arg.threadPart = threadPart;

#ifdef VERIFY
  for ( int i = 0; i < (threadPart * numThreads) / 8; i++ ) {
    ((uint64_t*)sendBuf)[i] = i;
  }

  bzero( recvBuf, threadPart * numThreads );
#endif
  if ( rank == 0 ) {
    rc = MPI_Partitioned_Send_create( sendBuf, threadPart * numThreads, MPI_CHAR, numThreads, other, TAG,
                                      MPI_COMM_WORLD, &sendReq );
    assert( rc == MPI_SUCCESS );
    arg.sendReq = sendReq;

  } else {
    rc = MPI_Partitioned_Recv_create( recvBuf, threadPart * numThreads, MPI_CHAR, other, TAG,
                                      MPI_COMM_WORLD, &recvReq );
    assert( rc == MPI_SUCCESS );
    arg.recvReq = recvReq;
  }

  start = MPI_Wtime();

  srand(time(NULL));

  sleep1 = compTime * 1000000000;
  sleepPlus = (compTime + ( compTime * noise)) * 1000000000 ;

  qt_loop(0, numThreads, worker, &arg);
  if ( 0 == rank ) {
    MPI_Partitioned_free( &sendReq );
  } else {
    MPI_Partitioned_free( &recvReq );
  }

  double duration = MPI_Wtime() - start;
  if ( numThreads > 1 ) {
    duration -=  sleepPlus / 1000000000.0 * numIterations;
  } else {
    duration -=  sleep1 / 1000000000.0 * numIterations;
  }
  return duration;
}
