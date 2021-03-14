/******************************************************************************
 * Jacobi1D benchmark
 * Basic parallelisation with OpenMP
 *
 * Usage:
 * make omp
 * export OMP_NUM_THREADS=8
 * bin/Jacobi1D-NaiveParallel-OMP `cat src/$V.perfexecopts`
 * For a run on 8 threads
 ******************************************************************************/
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

#include "util.h"


// main
// Given that this is a very straight forward benchark the code is almost
// entirely kept within the main function.
// The steps taken in this code are the following:
// 1 - command line parsing
// 2 - data allocation and initialization
// 3 - jacobi 1D timed within an openmp loop
// 4 - output and optional verification
//
int main( int argc, char* argv[] ){

  // rather than calling fflush    
  setbuf(stdout, NULL); 

  // 1 - command line parsing
  Params cmdLineArgs;
  parseCmdLineArgs(&cmdLineArgs,argc,argv);


  // 2 - data allocation and initialization
  int lowerBound = 1;
  int upperBound = lowerBound + cmdLineArgs.problemSize - 1; 
  double* space[2] = { NULL, NULL }; 

  // allocate time-steps 0 and 1
  space[0] = (double*) malloc( (cmdLineArgs.problemSize + 2) * sizeof(double));
  space[1] = (double*) malloc( (cmdLineArgs.problemSize + 2) * sizeof(double));
  if( space[0] == NULL || space[1] == NULL ){
    printf( "Could not allocate space array\n" );
    exit(0);
  }
  
  // use global seed to seed the random number gen (will be constant)
  srand(cmdLineArgs.globalSeed);
  // seed the space.
  int idx;
  // the randome number generator is not thread safe -- so first
  // set everything to 0 - respecting first touch
  #pragma omp parallel for private( idx ) schedule(static)
  for( idx = lowerBound-1; idx <= upperBound+1; ++idx ){
    space[0][idx] = 0;
  }
  for( idx = lowerBound; idx <= upperBound; ++idx ){
    space[0][idx] = rand() / (double)rand();
  }
  // set halo values (sanity)
  space[0][0] = 0;
  space[0][upperBound+1] = 0;
  space[1][0] = 0;
  space[1][upperBound+1] = 0;
  // end allocate and initialize space
  
  // 3 - jacobi 1D timed within an openmp loop
  // Begin timed test  
  int t,  read = 0, write = 1;
  double start_time = omp_get_wtime();
    
  for( t = 1; t <= cmdLineArgs.T; ++t ){
    #pragma omp parallel for private( idx ) schedule(static)
    for( idx = lowerBound; idx <= upperBound; ++idx ){
      space[write][idx] = (space[read][idx-1] +
                         space[read][idx] +
                         space[read][idx+1])/3;
    }
    read = write;
    write = 1 - write;
  }
  double end_time = omp_get_wtime();
  double time =  end_time - start_time;
  // End timed test

  // 4 - output and optional verification
  /*
  printf( "p: %d, T: %d, c:%d",cmdLineArgs.problemSize,cmdLineArgs.T,
                               cmdLineArgs.cores);
  */
  if( cmdLineArgs.printtime ){
    printf( "Time: %f", time );
  }

    
  if( cmdLineArgs.verify ){
    if(!verifyResultJacobi1D(space[cmdLineArgs.T & 1],cmdLineArgs.problemSize,
                             cmdLineArgs.globalSeed,cmdLineArgs.T )){
      fprintf(stderr,"FAILURE\n");
    }else{
      fprintf(stderr,"SUCCESS\n");
    } 
  }
  return 0; 
}
