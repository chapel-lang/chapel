/******************************************************************************
 * Jacobi1D benchmark
 * Tiled using diamond slabs coded by hand
 *
 * Usage:
 * make omp
 * export OMP_NUM_THREADS=8
 * bin/Jacobi1D-DiamondSlabByHand-OMP \
 * `cat src/Jacobi1D-DiamondSlabByHand-OMP.perfexecopts`
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

#define STENCIL(read,write,x)  space[write][x] = (space[read][x-1] +\
                               space[read][x] +\
                               space[read][x+1])/3;

int countTiles( int tiles_start, int upperBound, int stride ){
   int x0;
   int count = 0;
   for( x0 = tiles_start; x0 <= upperBound; x0 += stride ){
       count += 1;
   }
   return count;
}


// main
// The steps taken in this code are the following:
// 1 - command line parsing
// 2 - data allocation and initialization
// 3 - jacobi 1D timed within tiling loop
// 4 - output and optional verification
//
int main( int argc, char* argv[] ) {

  // rather than calling fflush
  setbuf(stdout, NULL); 

  // 1 - command line parsing
  Params cmdLineArgs;
  parseCmdLineArgs(&cmdLineArgs,argc,argv);

  // 2 - data allocation and initialization
  // variables required for Jacobi
  int lowerBound = 1;
  int upperBound = lowerBound + cmdLineArgs.problemSize - 1;

  // variables required for tiling
  int width_min = (cmdLineArgs.width_max + -1 * cmdLineArgs.timeBand) - 
                  (0 + 1 * cmdLineArgs.timeBand) +1;
  // starting point for doing 'A' tiles loops
  int tiles_A_start = lowerBound - cmdLineArgs.timeBand + 1; 
  // starting point for doing 'B' tiles loop
  int tiles_B_start = tiles_A_start + cmdLineArgs.width_max; 
  // width between the first x0 point and next x0 point
  int betweenTiles = width_min + cmdLineArgs.width_max;
  // assert that this is a valid tile
  assert( width_min >= 1 && cmdLineArgs.width_max >= width_min );

  int count_A_tiles = countTiles( tiles_A_start, upperBound, betweenTiles );
  int count_B_tiles = countTiles( tiles_B_start, upperBound, betweenTiles );
  int A_tiles_per_core = max( 1, count_A_tiles / cmdLineArgs.cores );
  int B_tiles_per_core = max( 1, count_B_tiles / cmdLineArgs.cores );


  // allocate time-steps 0 and 1
  double* space[2] = { NULL, NULL };
  space[0] = (double*) malloc( (cmdLineArgs.problemSize + 2) * sizeof(double));
  space[1] = (double*) malloc( (cmdLineArgs.problemSize + 2) * sizeof(double));
  if( space[0] == NULL || space[1] == NULL ){
    printf( "Could not allocate space array\n" );
    exit(0);
  }
 
  // perform first touch in the same manner that the tile will use the data
  int idx;
  #pragma omp parallel for private(idx) schedule(dynamic)
  for( idx = tiles_A_start; idx <= upperBound; idx += betweenTiles ){
    if(idx >= lowerBound){
      int i;
      for (i=idx;i<(idx+betweenTiles)&&i<upperBound;i++){
        space[0][i] = 0;
        space[1][i] = 0;
      }
    }
  }
  // use global seed to seed the random number gen (will be constant)
  srand(cmdLineArgs.globalSeed);
  // seed the space.
  for( idx = lowerBound; idx <= upperBound; ++idx ){
    space[0][idx] = rand() / (double)rand();
  }
  
  // set halo values (sanity)
  space[0][0] = 0;
  space[0][upperBound+1] = 0;
  space[1][0] = 0;
  space[1][upperBound+1] = 0;

  int read, write;
  int tau = cmdLineArgs.tau_runtime;
  int T = cmdLineArgs.T;
  int Ui = upperBound;
  int Li = 1;
  int thyme=-12345, k1=-12345, t=-12345, i=-12345;
  //fprintf(stderr,"tau=%d\n", tau);
  //fprintf(stderr,"%d, %d\n",floord(2, tau)-2, floord(T*2, tau));
  // 4 - run the actual test
  double start_time = omp_get_wtime();
  for ( thyme = floord(2, tau)-2;
        thyme <= floord(T*2, tau);
        thyme += 1){
    #pragma omp parallel for private(k1, t, write, read, i) schedule(dynamic)
    for ( k1 = (int)(Ui*2/((double) tau)-thyme+1 )/-2;
          k1 <= (int)( (Li*2/((double) tau))-thyme-1)/ -2 ;
          k1 += 1){
        //     printf("%d, %d, %d, %d\n", thyme, k1, t, i);
      // begin inner loops over points in tile
      for ( t = max(1, floord(thyme*tau - k1*tau + k1*tau + 1, 2)); 
            t < min(T+1, tau + floord(thyme*tau - k1*tau + k1*tau, 2)); 
            t += 1){
         //   printf("%d, %d, %d, %d\n", thyme, k1, t, i);
        write = t & 1;
        read = 1-write;
        //read = (t - 1) & 1;
        //write = 1 - read; 
        for ( i = max(Li, max(thyme*tau - k1*tau - t, -tau - k1*tau + t + 1)); 
              i <=min(Ui, min(tau + thyme*tau - k1*tau - t - 1, -k1*tau + t)); 
              i += 1){
//fprintf(stderr, "%02d, %02d, %d, %d\n", t,i, thyme, k1);
          //printf("%d, %d\n", t, i);
          STENCIL( read, write, i );
         // (t, i);
        } // i
      } // t
    } // k1    
  }// thyme
  
            //STENCIL( read, write, idx ); // stencil computation

    
  double end_time = omp_get_wtime();
  double time =  (end_time - start_time);

  // 4 - output and optional verification
  /*
  printf( "p: %d, T: %d, c: %d",cmdLineArgs.problemSize,cmdLineArgs.T,
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
  return 1;
    
}



