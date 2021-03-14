/******************************************************************************
 * Jacobi2D benchmark
 * Basic parallelisation with OpenMP
 *
 * Usage:
 * make omp
 * export OMP_NUM_THREADS=8
 * bin/Jacobi2D-NaiveParallel-OMP \
 *     `cat src/Jacobi2D-NaiveParallel-OMP.perfexecopts`
 * For a run on 8 threads
 ******************************************************************************/
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>


#define STENCIL(read,write,x,y) space[write][x][y] = \
                                                     ( space[read][x-1][y] +\
                                                       space[read][x][y] +\
                                                       space[read][x+1][y] +\
                                                       space[read][x][y+1] +\
                                                       space[read][x][y-1] )/5;
#include "util.h"


// main
// Stages
// 1 - command line parsing
// 2 - data allocation and initialization
// 3 - jacobi 1D timed within an openmp loop
// 4 - output and optional verification
int main( int argc, char* argv[] ){

  // rather than calling fflush    
  setbuf(stdout, NULL);

  // 1 - command line parsing
  Params cmdLineArgs;
  parseCmdLineArgs(&cmdLineArgs,argc,argv);

  // 1a - figure out how many complete tiles
  // and what this size of any incomplete tiles
  // are gonna be
  int tileCountX = cmdLineArgs.problemSize/cmdLineArgs.tile_len_x;
  int tileCountY = cmdLineArgs.problemSize/cmdLineArgs.tile_len_y;
  if(cmdLineArgs.problemSize % cmdLineArgs.tile_len_x != 0){
    tileCountX += 1;
  }
  if(cmdLineArgs.problemSize % cmdLineArgs.tile_len_y != 0){
    tileCountY += 1;
  }

  // 2 - data allocation and initialization
  int lowerBound = 1;
  int  upperBound = lowerBound + cmdLineArgs.problemSize - 1;
  double** space[2];
  int i; 
  
  // allocate x axis
  space[0] = (double**)malloc((cmdLineArgs.problemSize + 2) * sizeof(double*));
  space[1] = (double**)malloc((cmdLineArgs.problemSize + 2) * sizeof(double*));
  if( space[0] == NULL || space[1] == NULL ){
    printf( "Could not allocate x axis of space array\n" );
    exit(0);
  }
  
  // allocate y axis
  for( i = 0; i < cmdLineArgs.problemSize + 2; ++i ){
    space[0][i]=(double*)malloc((cmdLineArgs.problemSize + 2) * sizeof(double));
    space[1][i]=(double*)malloc((cmdLineArgs.problemSize + 2) * sizeof(double));
    if( space[0][i] == NULL || space[1][i] == NULL ){
      printf( "Could not allocate y axis of space array\n" );
      exit(0);
    }
  }
  
  // use global seed to seed the random number gen (will be constant)
  srand(cmdLineArgs.globalSeed);
  // first touch for openmp
  int x, y;
  int itx, ity;
  #pragma omp parallel for private( x, y ) schedule(dynamic) 
  for(itx=0; itx< tileCountX; itx++){
    for(ity=0; ity < tileCountY; ity++ ){
      int xlb = lowerBound+itx*cmdLineArgs.tile_len_x; 
      int xub = min(upperBound,lowerBound+itx*cmdLineArgs.tile_len_x+
                    cmdLineArgs.tile_len_x);
      for( x = xlb; x <= xub; ++x ){
        int ylb = lowerBound+ity*cmdLineArgs.tile_len_y; 
        int yub = min(lowerBound+ity*cmdLineArgs.tile_len_y+ 
                       cmdLineArgs.tile_len_y,upperBound);
        for( y = ylb; y <= yub;++y ){
          space[0][x][y] = 0;
          space[1][x][y] = 0;
        }
      }
    }
  }
  // seed the space.
  for( x = lowerBound; x <= upperBound; ++x ){
    for( y = lowerBound; y <= upperBound; ++y ){
      space[0][x][y] = rand() / (double)rand();
    }
  }
  
  // set halo values (sanity)
  for( i = 0; i < cmdLineArgs.problemSize + 2; ++i){
    space[0][i][0] = 0;
    space[1][i][0] = 0;
    space[0][i][cmdLineArgs.problemSize + 1] = 0;
    space[1][i][cmdLineArgs.problemSize + 1] = 0;
    
    space[0][0][i] = 0;
    space[1][0][i] = 0;
    space[0][cmdLineArgs.problemSize + 1][i] = 0;
    space[1][cmdLineArgs.problemSize + 1][i] = 0;
  }
  
  
  // 3 - jacobi 2D timed within an openmp loop
  double start_time = omp_get_wtime();
  int t,read=0,write=1;
  int lbplusx = lowerBound+cmdLineArgs.tile_len_x;
  int xtmp = cmdLineArgs.tile_len_x;
  int lbplusy = lowerBound+cmdLineArgs.tile_len_y;
  int ytmp =  cmdLineArgs.tile_len_y;
  for( t = 1; t <= cmdLineArgs.T; ++t ){ 
    #pragma omp parallel for private( x, y ) schedule(dynamic) 
    for(itx=0; itx< tileCountX; itx++){
      for(ity=0; ity < tileCountY; ity++ ){
      int xlb = lowerBound+itx*xtmp; 
      int xub = min(upperBound,lbplusx+itx*xtmp);
      for( x = xlb; x <= xub; ++x ){
        int ylb = lowerBound+ity*ytmp; 
        int yub = min(lbplusy+ity*ytmp, upperBound);
        for( y = ylb; y <= yub;++y ){
            STENCIL( read, write, x, y);
          }
        }
      }    
    }
    read = write;
    write = 1 - write;
  }
  
  double end_time = omp_get_wtime();
  double time =  (end_time - start_time);

  // 4 - output and optional verification
  if( cmdLineArgs.printtime ){
       /*
        printf( "Threads: %d, P: %d, Tile: %d,%d, ",cmdLineArgs.cores,
                                       cmdLineArgs.problemSize,
                                       cmdLineArgs.tile_len_x,
                                       cmdLineArgs.tile_len_y);
                                       */
    printf( "Time: %f", time );
  }
  if( cmdLineArgs.verify ){
    if(!verifyResultJacobi2D(space[cmdLineArgs.T & 1],cmdLineArgs.problemSize,
                             cmdLineArgs.globalSeed,cmdLineArgs.T )){
      fprintf(stderr,"FAILURE\n");
    }else{
      fprintf(stderr,"SUCCESS\n");
    }
  }
  
}
