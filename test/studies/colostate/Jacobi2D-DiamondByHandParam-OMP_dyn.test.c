/******************************************************************************
 * Jacobi2D benchmark
 * Diamond tiling parameterized by hand.
 *
 * Copied Jacobi2D-DiamondSlabISCCParam-OMP-test.c to get similar format
 * to other drivers and then put in parameterized loop bounds.
 *
 * Look for the notes titled "Parameterizing diamond tiling by hand" in
 * ProjectNotes/chapel-diamond-MMS-log.txt for the details of how this was done.
 *
 * Usage:
 *  make omp
 *  export OMP_NUM_THREADS=8
 *  bin/Jacobi2D-DiamondISCCParam-OMP \
 *     `cat src/Jacobi2D-DiamondByHandParam-OMP.perfexecopts`
 * For a run on 8 threads
 *
 * Use
 *   bin/Jacobi2D-DiamondByHandParam-OMP -h
 * to get a list of command-line arguments.  It is possible that not all work 
 * for this driver.
 *
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
  
  // T is used down it loop code a lot.
  int T = cmdLineArgs.T;

  // If tau was not defined at compile time then declare a tau variable
  // and grab the value set with command-line options.
  // If it was defined at compile time then all instances of "tau" will be replaced
  // with that number by the compiler.
  #if ! defined tau
  int tau = cmdLineArgs.tau_runtime;
  #endif

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
  // FIXME: we will have to specialize first touch for diamond tiles
  int x, y;
  #pragma omp parallel for private( x, y ) collapse(2) schedule(dynamic)
  for( x = lowerBound; x <= upperBound; ++x ){
    for( y = lowerBound; y <= upperBound; ++y ){
      space[0][x][y] = 0;
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
  int read=0,write=1;
  int num_tiles = 0;
  int tau_times_3 = tau*3;

  // Set lower and upper bounds for spatial dimensions.
  int Li=1, Lj=1, Ui=upperBound, Uj=upperBound;
  int thyme, k1, k2, t, j;

  // Loop over tile wavefronts.
  //for (thyme=-2; thyme<=floord(3*T,tau); thyme++) { // rev 298
  for (thyme=ceild(3,tau)-3; thyme<=floord(3*T,tau); thyme++){ // from paper

    // The next two loops iterate within a tile wavefront.
    //int k1_lb = floord(3*Lj+2+(thyme-2)*tau,tau_times_3); // rev 298
    //int k1_ub = floord(3*Uj+(thyme+2)*tau-2,tau_times_3);
    int k1_lb = ceild(3*Lj+2+(thyme-2)*tau,tau*3);       // paper
    int k1_ub = floord(3*Uj+(thyme+2)*tau,tau*3);

//    int k2_lb = floord((2*thyme-2)*tau-3*Ui+2,tau_times_3);
//    int k2_ub = floord((2+2*thyme)*tau-2-3*Li,tau_times_3);
#define k2_lb_exp(k1val) floord((2*thyme-2)*tau-3*Ui+2,tau*3)-(k1val)
#define k2_ub_exp(k1val) floord((2+2*thyme)*tau-3*Li-2,tau*3)-(k1val)
    // bounding box for k2
    int k2_lb = k2_lb_exp(k1_ub); // min possible value of expression
    int k2_ub = k2_ub_exp(k1_lb); // max possible value of expression
   
    //num_tiles+=(k2_ub-k2_lb+1)*(k1_ub-k1_lb+1);
    //printf("Number of tiles: %d\n",(k2_ub-k2_lb+1)*(k1_ub-k1_lb+1)); 
#pragma omp parallel for shared(start_time, Li, Lj, Ui, Uj ) private(read, write,k1,k2,t,i,j) schedule(dynamic) collapse(2)
    for (k1=k1_lb; k1<=k1_ub; k1++) {
      for (int x=k2_lb; x<=k2_ub; x++) {
        k2 = x;

        // Loop over time within a tile.
        //for (t=max(1,floord(thyme*tau,3)); // rev 298
        //     t<= min(T,floord((3+thyme)*tau-3,3)); t++) {
        for (t = max(1,floord(thyme*tau-1, 3) + 1);         // from paper
             t < min(T+1, tau + floord(thyme*tau, 3)); t+=1) {
          // if t % 2  is 1, then read=0 and write=1
          write = t & 1;
          read = 1-write;

          // Loops over spatial dimensions within tile.
          //for (i=max(Li,max((thyme-k1-k2)*tau-t, 2*t-(2+k1+k2)*tau+2));// rev 298
          //     i<=min(Ui,min((1+thyme-k1-k2)*tau-t-1, 2*t-(k1+k2)*tau)); i++) {
          //  for (j=max(Lj,max(tau*k1-t,t-i-(1+k2)*tau+1));
          //       j<=min(Uj,min((1+k1)*tau-t-1,t-i-k2*tau)); j++) {
          for (i = max(Li, max(-2*tau-k1*tau-k2*tau+2*t+2, // from paper
                               (thyme-k1-k2)*tau-t));
               i <= min(Ui, min(tau+(thyme-k1-k2)*tau-t-1, 
                                -k1*tau-k2*tau+2*t)); i+=1) {
            for (j = max(Li,max(k1*tau-t, -tau-k2*tau+t-i+1)); 
                 j <= min(Ui,min(tau+k1*tau-t-1, -k2*tau+t-i)); j+=1) {
              STENCIL( read, write, i, j);
            } // for j
          } // for i
        } // for t
      } // for k2
    } // for k1
  } // for thyme

  double end_time = omp_get_wtime();
  double time =  (end_time - start_time);

  // 4 - output and optional verification
  if( cmdLineArgs.printtime ){
    /*
    printf("Threads: %d, ",cmdLineArgs.cores);
    printf( "T: %d, ", T );
    printf( "N: %d, ", upperBound );
    printf( "tau: %d, ", tau );
    printf( "num_tiles: %d, ", num_tiles );
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

