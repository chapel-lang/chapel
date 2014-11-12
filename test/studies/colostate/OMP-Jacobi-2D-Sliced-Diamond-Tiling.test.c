// COMPILE-TIME OPTIONS:
//    BOUNDING_BOX_FOR_PARALLEL_LOOPS  (also automatically on if PARALLEL) --- execute extra iterations of parallel loops to un-confuse OMP
//    PARALLEL                         use bounding box (above) and #pragma omp parallel on appropriate for loops
//    CHATTY                           print more info, e.g. echoing compile-time options and 		
//    tau                              set to an integer multiple of 3 that is >= 15 to enable more optimization but lose flexibility

#if ! defined PARALLEL
#define PARALLEL 0
# if ! defined BOUNDING_BOX_FOR_PARALLEL_LOOPS
# define BOUNDING_BOX_FOR_PARALLEL_LOOPS 0
# endif
#else
# if PARALLEL
# define BOUNDING_BOX_FOR_PARALLEL_LOOPS 1
# endif
#endif


#include <stdio.h>
#if PARALLEL || 1  /* need omp for the timing function; otherwise we could skip it */
#include <omp.h>
#endif
#include <time.h>
#include <stdlib.h>
//#include <unistd.h>
#include <getopt.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include "eassert.h"
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include "intops.h"

bool initialized = false;
int globalSeed = -1;
int cores = -1;
int FP_OPS_PER_ITERATION = 5;

int problemSize = -1, T = -1, lowerBound = -1, upperBound = -1;

double** space[2] = { NULL, NULL }; // space[t][x][y] for (t,x) in { {0,1} X {lowerBound, ... , upperBound} X {lowerBound, ..., upperBound} };


void init(){
    // if init has not already been called (preserve things like global seed.
    if( ! initialized ){
        // note the convention someVar = ( someVar == -1 )? defaultValue : someVar ;
        // this allows us to use the cmd line flags to set variables, AND have an init call.
        // all values are initialized with -1 in global space, so if someVar == -1, then it has
        // not been set, and and be given a default value.
        
        // seed for random number generator.
        // allows all initSpace calls to generate the same inital values
        globalSeed = (globalSeed== -1)? time(NULL) : globalSeed;
        
        // problemSpace parameters
        T = (T == -1)? 100 : T;
        problemSize = (problemSize == -1)? 100 : problemSize;
        lowerBound = 1;
        upperBound = lowerBound + problemSize - 1;
        
        
        cores = (cores == -1)? omp_get_num_procs() : cores ;
        
        omp_set_num_threads( cores );
        
        // set initialization flag
        initialized = true;
    }
}


// initialize space array
void initSpace(){
    int i; 
    
    // if space has been previously allocated, free up space.
    if( space[0] != NULL ){
      free( space[0] );
      space[0] = NULL;
    }
    
    if( space[1] != NULL ){
      free( space[1] );
      space[1] = NULL;
    }
    
    /*
    // allocate time steps 0 and 1
    space = (double***) malloc( 2 * sizeof(double**) );
    if( space == NULL ){
        printf( "Could not allocate time steps of space array\n" );
        exit(0);
    }
    */
    
    // allocate x axis
    space[0] = (double**) malloc( (problemSize + 2) * sizeof(double*));
    space[1] = (double**) malloc( (problemSize + 2) * sizeof(double*));
    if( space[0] == NULL || space[1] == NULL ){
        printf( "Could not allocate x axis of space array\n" );
        exit(0);
    }
    
    // allocate y axis
    for( i = 0; i < problemSize + 2; ++i ){
        space[0][i] = (double*) malloc( (problemSize + 2) * sizeof(double));
        space[1][i] = (double*) malloc( (problemSize + 2) * sizeof(double));
        if( space[0][i] == NULL || space[1][i] == NULL ){
            printf( "Could not allocate y axis of space array\n" );
            exit(0);
        }
    }
    
    // use global seed to seed the random number gen (will be constant)
    srand(globalSeed);
    // seed the space.
    int x, y;
    for( x = lowerBound; x <= upperBound; ++x ){
        for( y = lowerBound; y <= upperBound; ++y ){
            space[0][x][y] = rand() / (double)rand();
        }
    }
    
    // set halo values (sanity)
    for( i = 0; i < problemSize + 2; ++i){
        space[0][i][0] = 0;
        space[1][i][0] = 0;
        space[0][i][problemSize + 1] = 0;
        space[1][i][problemSize + 1] = 0;
        
        space[0][0][i] = 0;
        space[1][0][i] = 0;
        space[0][problemSize + 1][i] = 0;
        space[1][problemSize + 1][i] = 0;
    }
    
}

// stencil call.
void stencil( int read, int write, int x, int y ){
    // stencil operation
    space[write][x][y] = ( space[read][x-1][y] + space[read][x][y] + space[read][x+1][y] + space[read][x][y+1] + space[read][x][y-1] )/5;
}

// parse int abstraction from strtol
int parseInt( char* string ){
    return (int) strtol( string, NULL, 10 );
}


// returns true if valid result
bool verifyResult( bool verbose ){
    assert( space[0] != NULL && space[1] != NULL );
   
    double** endSpace;
    endSpace = (double**) malloc( (problemSize + 2) * sizeof(double*));
    if( endSpace == NULL ){
        printf( "Could not allocate x axis of verification array\n" );
        exit(0);
    }
    
   // allocate y axis
    for( int x = 0; x < problemSize + 2; ++x ){
        endSpace[x] = (double*) malloc( (problemSize + 2) * sizeof(double));
        if( endSpace[x] == NULL ){
            printf( "Could not allocate y axis of verification array\n" );
            exit(0);
        }
    }
   
    for( int x = 0; x <  problemSize + 2; ++x ){
        for( int y = lowerBound; y <= upperBound; ++y ){
            endSpace[x][y] = space[ T & 1 ][x][y];
        }
    }
    
    initSpace();
    
    int t, x, y, read = 0, write = 1;
    for( t = 1; t <= T; ++t ){
        for( x = lowerBound; x <= upperBound; ++x ){
            for( y = lowerBound; y <= upperBound; ++y ){
                stencil( read, write, x, y);
            }        
        }
        read = write;
        write = 1 - write;
    }
    
    bool failed = false;
    for( x = lowerBound; x <= upperBound; ++x ){
        for( y = lowerBound; y <= upperBound; ++y ){
            if( endSpace[x][y] != space[ T & 1 ][x][y] ){
                failed = true;
                 if( verbose ) printf( "FAILED! %f != %f at %d, %d\n", endSpace[x][y],space[ T & 1 ][x][y], x, y);
                break;
            }
        }
        if( failed ) break;
    }
    
    if( verbose && !failed ) printf( "SUCCESS\n" );
    
    for( int x = 0; x < problemSize + 2; ++x ){
        free( endSpace[x] );
    }
    
    free( endSpace );
    
    return !failed;

}


#define STENCIL(read,write,x,y) space[write][x][y] = ( space[read][x-1][y] + space[read][x][y] + space[read][x+1][y] + space[read][x][y+1] + space[read][x][y-1] )/5
#if ! defined tau
int tau = 192; // diamond tile size, can be any multiple of 3 >=15
#endif
int subset_s = 38; // number of time slices to use in each round of
                  // the tiling pattern

// Some error codes
#define BAD_COMPILE_TIME_PARAMETERS     1
#define BAD_RUN_TIME_PARAMETERS         2

double test_1(){
    double start_time = omp_get_wtime();
    int read=0, write = 1;

    // s is the number of non-pointy bit 2D slices of diamond tiling
    // that is available for the current tile size.
    int s = (tau/3) - 2;
    // subset_s is an input parameter indicating how many of those
    // slices we want to use in the repeated tiling pattern.
    // subset_s should be less than s and greater than or equal to 2.
    if (subset_s > s  || subset_s<2) {
        fprintf(stderr, "Error: need 2<=subset_s<=s\n");
        exit(-1);
    }
  
    // Set lower and upper bounds for spatial dimensions.
    // When did code gen have a non-inclusive upper bound.
    // Ian's upper bound is inclusive.
    int Li=1, Lj=1, Ui=upperBound+1, Uj=upperBound+1;
  
    // Loop over the tiling pattern.
    for (int toffset=0; toffset<T; toffset+=subset_s){
  
        // Loop over phases of tiles within repeated tile pattern.
        // This is like iterating over the A and B trapezoid tile types.
        for (int c0 = -2; c0 <= 0; c0 += 1){

            // Two loops over tiles within one phase.
            // All of the tiles within one phase can be done in parallel.

            // updates by Dave W, to the c1 and c2 loops, for OpenMP (from here to the end of the #if BOUNDING_BOX_FOR_PARALLEL_LOOPS
            //   hoist out min_c1 and max_c1, then use that to hoist a bounding box for c2
            //   initial version is just aiming for correct and parallel, without worrying about a loose boundingbox

            int c1_lb =
                max(
                    max(
                        floord(Lj + (tau/3) * c0 + (tau/3), tau), 
                        c0 + floord(-2 * T + Lj - 1, tau) + 1), 
                    floord(Lj + 1, tau)
                ); // end init block c1
            int c1_ub =
                min(
                    min(
                        floord(Uj + (tau/3) * c0 - ((tau/3)+2), tau) + 1, 
                        floord(T + Uj - 1, tau)), 
                    c0 + floord(Uj - 5, tau) + 2
                ); // end cond block c1

            // The two expressions below are the same as in the previous version, except that
            //  in the c2_lb_min_expr, I have replaced c1 with:
            //    c1_min_value where it appears with a positive coefficient, and
            //    c1_max_value where it appears with a negative coefficient.
            //  and in the c2_ub_max_expr, the opposite (i.e., c1 becomes c1_max_value where positive)
            // I will be embarrassed if I have done this wrong.
            ///  Note that I assume tau > 0
#define c2_lb_min_expr(c1_min_value, c1_max_value)              \
                    max( \
                        max( \
                            max( \
                                max( \
                                    max( \
                                        max( \
                                            c0 - 2 * c1_max_value + floord(-Ui + Lj + 1,tau),  \
                                            -c1_max_value + floord(-2 * Ui - Uj + tau * c0 + tau * c1_min_value - tau-3, tau*2)+1), \
                                        c1_min_value + floord(-Ui - 2 * Uj + 3, tau)), \
                                    floord(-Ui - Uj + 3, tau)),  \
                                c0 - c1_max_value + floord(-Ui - (tau/3) * c0 + ((tau/3)+1), tau)),  \
                            c0 - c1_max_value + floord(-T - Ui, tau) + 1),  \
                        -c1_max_value + floord(-Ui + 4, tau) - 1 \
                    ) /* end init block c2 */

#define c2_ub_max_expr(c1_min_value, c1_max_value)              \
                    min( \
                        min( \
                            min( \
                                min( \
                                    min( \
                                        min( \
                                            c0 - 2 * c1_min_value + floord(-Li + Uj - 2, tau) + 1,  \
                                            c0 - c1_min_value + floord(-Li - 2, tau) + 1),  \
                                        c0 - c1_min_value + floord(-Li - (tau/3) * c0 - ((tau/3)+1), tau) + 1), \
                                    floord(T - Li - Lj, tau)), \
                                -c1_min_value + floord(2 * T - Li, tau)),  \
                            c1_max_value + floord(-Li - 2 * Lj - 1, tau) + 1),  \
                        -c1_min_value + floord(-2 * Li - Lj + tau * c0 + tau * c1_max_value + (tau-1), tau*2) \
                    ) /* end cond block c2 */
#define c2_lb_expr(c1_value) c2_lb_min_expr(c1_value, c1_value)
#define c2_ub_expr(c1_value) c2_ub_max_expr(c1_value, c1_value)


#if BOUNDING_BOX_FOR_PARALLEL_LOOPS
            int c2_box_lb = c2_lb_min_expr(c1_lb, c1_ub);
            int c2_box_ub = c2_ub_max_expr(c1_lb, c1_ub);
#if PARALLEL
            // don't need to mention c1...c5 below, since they're scoped inside the for loops
#pragma omp parallel for shared(start_time, s, Li, Lj, Ui, Uj, toffset, c0, c1_lb, c1_ub, c2_box_lb, c2_box_ub, ) private(read, write) collapse(2)
#endif
            for (int c1 = c1_lb; c1 <= c1_ub; c1 += 1) {
                for (int c2 = c2_box_lb; c2 <= c2_box_ub; c2 += 1) if (c2 >= c2_lb_expr(c1) && c2 <= c2_ub_expr(c1)) {
#else
            for (int c1 = c1_lb; c1 <= c1_ub; c1 += 1) {
                for (int c2 = c2_lb_expr(c1); c2 <= c2_ub_expr(c1); c2 += 1) {
#endif
                  //fprintf(stdout, "(%d,%d,%d)\n", c0,c1,c2);
                    // Loop over subset_s time steps within tiling pattern
                    // and within tile c0,c1,c2.
                    // Every time the pattern is repeated, toffset will be
                    // subset_s bigger.
                    // The real t value is c3+toffset.  We are just using the
                    // tiling pattern from t=1 to t<=subset_s.
                    for (int c3 = 1; c3 <= min(T-toffset,subset_s); c3 += 1){
                        int t = c3+toffset;
                        // if t % 2  is 1, then read=0 and write=1
                        write = t & 1;
                        read = 1-write;

                        // x spatial dimension.
                        for (int c4 = 
                            max(
                                max(
                                    max(
                                        -tau * c1 - tau * c2 + 2 * c3 - (2*tau-2),
                                        -Uj - tau * c2 + c3 - (tau-2)),
                                   tau * c0 - tau * c1 - tau * c2 - c3),
                                 Li
                            ); // end init block c4
                        c4 <= 
                            min(
                                min(
                                    min(
                                        tau * c0 - tau * c1 - tau * c2 - c3 + (tau-1),
                                        -tau * c1 - tau * c2 + 2 * c3),
                                    -Lj - tau * c2 + c3),
                                Ui - 1
                            ); // end cond block c4
                        c4 += 1){

                            // y spatial dimension.
                            for (int c5 = 
                                max(
                                    max(
                                        tau * c1 - c3,
                                        Lj),
                                    -tau * c2 + c3 - c4 - (tau-1)
                                ); // end init block c5
                                c5 <= 
                                    min(
                                        min(
                                            Uj - 1,
                                            -tau * c2 + c3 - c4),
                                        tau * c1 - c3 + (tau-1)
                                ); // end cond block c5
                                c5 += 1){
                                //fprintf(stdout, "(%d,%d,%d,%d,%d,%d)\n", c0,c1,c2,c3,c4,c5);
                                stencil( read, write, c4, c5);
                            } // for c5
                        } // for c4
                    } // for c3
                } // for c2
            } // for c1
        } // for c0
    } // for toffset
    
    double end_time = omp_get_wtime();
    
    return (end_time - start_time);
}

int main( int argc, char* argv[] ){
    setbuf(stdout, NULL); // set buffer to null, so prints ALWAYS print (for debug purposes mainly)
    
    bool verify = false;
    bool printtime = true;
    // Command line parsing
    char c;
    while ((c = getopt (argc, argv, "nc:s:p:T:t:hv")) != -1){
        switch( c ) {
            case 'n':
                printtime=false;
                break;
                
            case 'c': // problem size
                cores = parseInt( optarg );
                if( cores <= 0 ){
                    fprintf(stderr, "cores must be greater than 0: %d\n", cores);
                    exit(BAD_RUN_TIME_PARAMETERS);
                }
                break;
                
            case 's': // subset
                //globalSeed = parseInt( optarg );
                subset_s = parseInt( optarg );
                break;
                
            case 'p': // problem size
                problemSize = parseInt( optarg );
                if( problemSize <= 0 ){
                    fprintf(stderr, "problemSize must be greater than 0: %d\n", problemSize);
                    exit(BAD_RUN_TIME_PARAMETERS);
                }
                break;
                
            case 'T': // T (time steps)
                T = parseInt( optarg );
                if( T <= 0 ){    
                    fprintf(stderr, "T must be greater than 0: %d\n", T);
                    exit(BAD_RUN_TIME_PARAMETERS);
                }
                break;
            
            case 't': // tau
#if defined tau
                fprintf(stderr, "don't use -t to set tau when you compiled with -Dtau=%d.\n", tau);
                if (parseInt(optarg) != tau)
                    exit(BAD_COMPILE_TIME_PARAMETERS);
#else
                tau = parseInt( optarg );
#endif
                break;
            
            case 'h': // help
                printf("usage: %s\n-n \t dont print time \n-p <problem size> \t problem size in elements \n-T <time steps>\t number of time steps\n-c <cores>\tnumber of threads\n-s <subset_s>\t tile parameter\n-t <tau>\t tile parameter\n-h\tthis dialogue\n-v\tverify output\n", argv[0]);
                exit(0);
            
            case 'v': // verify;
                verify = true;
                break;
            
            case '?':
                if (optopt == 'p')
                    fprintf (stderr, "Option -%c requires positive int argument: problem size.\n", optopt);
                else if (optopt == 'T')
                    fprintf (stderr, "Option -%c requires positive int argument: T.\n", optopt);
                else if (optopt == 's')
                    fprintf (stderr, "Option -%c requires int argument: subset_s.\n", optopt);
                else if (optopt == 'c')
                    fprintf (stderr, "Option -%c requires int argument: number of cores.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                exit(0);
                
          default:
             exit(0);
          }
    }

    if( !( tau % 3 == 0 && tau >= 15 ) ){
#if defined tau
        fprintf(stderr, "tau must be a multiple of 3, and >= 15, but the program was compiled with -Dtau=%d, and thus can't run :-(\n", tau);
        exit(BAD_COMPILE_TIME_PARAMETERS);
#else
        fprintf(stderr, "tau must be a multiple of 3, and >= 15, but it's %d; re-run with a different -t value\n", tau);
        exit(BAD_RUN_TIME_PARAMETERS);
#endif
    }


    init();
    initSpace();
    
    double time = test_1();
    
    if( printtime ) {
        printf( "Time: %f\n", time );
    }

    if( verify ){
        verifyResult( true );
    }
    
}

/*
 * setting up emacs to use tabs the way Michelle likes them, and set the following
 * Local Variables:
 *   c-basic-offset:   4
 *   indent-tabs-mode: nil
 * End:
 *  (select the whole buffer and use M-x untabify to convert)
 */
