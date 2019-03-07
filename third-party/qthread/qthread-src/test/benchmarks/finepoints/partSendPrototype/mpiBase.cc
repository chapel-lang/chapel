#include <qthread/qthread.h>
#include <mpi.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string>

double doTest1( int rank, int, char*, char*, int, size_t );
double doTest11( int rank, int, char*, char*, int, size_t );
double doTest2( int rank, int, char*, char*, int, size_t );
double doTest21( int rank, int, char*, char*, int, size_t, double, double );
double doTest3( int rank, int, char*, char*, int, size_t );
double doTest31( int rank, int, char*, char*, int, size_t );
double doTest31t( int rank, int, char*, char*, int, size_t );
double doTest32( int rank, int, char*, char*, int, size_t );
double doTest5( int rank, int, char*, char*, int, size_t, double, double );

struct Args {
  int threadModel;
  int test;
  int numIterations;
  int numThreads;
  int minNumThreads;
  int maxNumThreads;
  size_t maxBuffSize;
  size_t minBuffSize;
  double compTime;
  double noise;
};

int parseArgs( int argc, char* argv[], Args* args );
std::string getThreadModelStr(int model);

int main( int argc, char* argv[] )
{
  int provided;
  int rank, size;
  int rc;

  Args args;
  args.threadModel = MPI_THREAD_SERIALIZED;
  args.test = atoi(argv[1]) ? atoi(argv[1]) : 1;
  args.numThreads = 0;
  args.numIterations = 1;
  args.minNumThreads = 1;
  args.maxNumThreads = 1;
  args.minBuffSize = 32;
  args.maxBuffSize = 32;
  args.compTime = 0.050;
  args.noise = 0.002;

  rc = parseArgs( argc, argv, &args );
  if ( -1 == rc ) {
    return -1;
  }

  if ( args.numThreads != 0 ) {
    args.minNumThreads = args.maxNumThreads = args.numThreads;
  }

  assert(qthread_initialize() == 0);

  rc = MPI_Init_thread( &argc, & argv, args.threadModel, &provided );
  assert( rc  == MPI_SUCCESS );

  assert( provided == args.threadModel );

  rc = MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  assert( rc  == MPI_SUCCESS );

  if ( 0 == rank ) {
    printf("# %i shepherds...\n", qthread_num_shepherds());
    printf("# %i threads total\n", qthread_num_workers());
    printf("# MPI thread model %s\n",getThreadModelStr(args.threadModel).c_str() );
    printf("# test=%d iterations=%d minThreads=%d maxThreads=%d minBuf=%lu maxBuf=%lu\n",args.test,args.numIterations,
           args.minNumThreads,args.maxNumThreads,args.minBuffSize,args.maxBuffSize);
    printf("compTime=%f noise=%f\n",args.compTime,args.noise);
  }
  char hostname[100];
  gethostname(hostname,100);
  printf("# rank=%d hostname=%s\n",rank,hostname);

  rc = MPI_Comm_size( MPI_COMM_WORLD, &size );
  assert( rc  == MPI_SUCCESS );

  assert( size == 2 );

  char* sendBuf = (char*) malloc( args.maxBuffSize );
  char* recvBuf = (char*) malloc( args.maxBuffSize );

  for ( size_t bufSize = args.minBuffSize; bufSize <= args.maxBuffSize; bufSize *= 2 ) {
    for ( int numThreads = args.minNumThreads; numThreads <= args.maxNumThreads; numThreads*=2 ) {
      size_t threadPart = bufSize/numThreads;
      //printf("rank=%d bufSize=%d numThreads=%d\n",rank, bufSize, numThreads );
      double duration;
      double start = MPI_Wtime();
      MPI_Barrier(MPI_COMM_WORLD);
      switch( args.test ) {
      case 1:
        duration = doTest1( rank, args.numIterations, sendBuf, recvBuf, numThreads, threadPart );
        break;
      case 11:
        duration = doTest11( rank, args.numIterations, sendBuf, recvBuf, numThreads, threadPart );
        break;
      case 2:
        duration = doTest2( rank, args.numIterations, sendBuf, recvBuf, numThreads, threadPart );
        break;
      case 21:
        duration = doTest21( rank, args.numIterations, sendBuf, recvBuf, numThreads, threadPart, args.compTime, args.noise );
        break;
      case 3:
        duration = doTest3( rank, args.numIterations, sendBuf, recvBuf, numThreads, threadPart );
        break;
      case 31:
        duration = doTest31( rank, args.numIterations, sendBuf, recvBuf, numThreads, threadPart );
        break;
      case 311:
        duration = doTest31t( rank, args.numIterations, sendBuf, recvBuf, numThreads, threadPart );
        break;
      case 32:
        duration = doTest32( rank, args.numIterations, sendBuf, recvBuf, numThreads, threadPart );
        break;
      case 5:
        duration = doTest5( rank, args.numIterations, sendBuf, recvBuf, numThreads, threadPart, args.compTime, args.noise  );
        break;
      default:
        printf("don't know about test %d\n",args.test);
        exit(-1);
      }
      double stop = MPI_Wtime();
      if ( 0 == rank ) {
        double bandwidth = bufSize * args.numIterations / duration;
        bandwidth /= 1000000.0;

        double msgRate = args.numIterations / duration;
        //printf("bufSize %lu, numThreads %d, threadPart %lu, time %f, bandwidth %.3f MB/s, msgRate %.3f msg/sec, %lf\n",
        //	bufSize,numThreads,threadPart, duration, bandwidth, msgRate, (stop-start)/args.numIterations );
        printf("output %lu %d %lu %f %.3f %.3f %lf\n",
               bufSize,numThreads,threadPart, duration, bandwidth, msgRate, (stop-start)/args.numIterations );
      }
    }
  }

  rc = MPI_Finalize();
  assert( rc  == MPI_SUCCESS );

  printf("# %d: rank=%d exiting\n",__LINE__,rank);

  return 0;
}
int parseArgs( int argc, char* argv[], Args* args )
{
#define OPTIONAL 2
  static struct option long_options[] = {
    {"i", OPTIONAL, NULL, 0 },
    {"minT", OPTIONAL, NULL, 1 },
    {"maxT", OPTIONAL, NULL, 2 },
    {"minB", OPTIONAL, NULL, 3 },
    {"maxB", OPTIONAL, NULL, 4 },
    {"test", OPTIONAL, NULL, 5 },
    {"numT", OPTIONAL, NULL, 6 },
    {"tm", OPTIONAL, NULL, 7 },
    {"ct", OPTIONAL, NULL, 8 },
    {"n", OPTIONAL, NULL, 9 },
    {0,0,0,0}
  };

  while (1) {
    int option_index = 0;
    int c =  getopt_long( argc, argv, "", long_options, &option_index );
    if ( c == -1) break;
    switch( c ) {
    case 0:
      args->numIterations = atoi(optarg);
      break;
    case 1:
      args->minNumThreads = atoi(optarg);
      break;
    case 2:
      args->maxNumThreads = atoi(optarg);
      break;
    case 3:
      args->minBuffSize = atoi(optarg);
      break;
    case 4:
      args->maxBuffSize = atoi(optarg);
      break;
    case 5:
      args->test = atoi(optarg);
      break;
    case 6:
      args->numThreads = atoi(optarg);
      break;
    case 7:
      args->threadModel = MPI_THREAD_MULTIPLE;
      break;
    case 8:
      args->compTime = atof(optarg);
      break;
    case 9:
      args->noise = atof(optarg);
      break;
    default:
      return -1;
    }
  }
  return 0;
}
std::string getThreadModelStr(int model)
{
  switch(model) {
  case MPI_THREAD_MULTIPLE: return "Multiple";
  case MPI_THREAD_SERIALIZED: return "Serial";
  }
  assert(0);
}
