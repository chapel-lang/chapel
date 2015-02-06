//#include "jacobi1d.h"
//#include "trapezoidTiling.h"
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

//#include "trapezoidTiling.h"
//#include "jacobi1d.h"
#include <stdbool.h>
#include <assert.h>
//#include "jacobi1d.h"
#include <stdbool.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#define stencil(read,write,x) space[write][x] = (space[read][x-1] + space[read][x] + space[read][x+1])/3;

bool initedJacobi = false;
int globalSeed = -1;
int cores = -1;

int problemSize = -1, T = -1, lowerBound = -1, upperBound = -1;
double* space[2] = { NULL, NULL }; // space[t][x] for (t,x) in { {0,1} X {lowerBound, ... , upperBound} };


int min( int a, int b){
	return (a <= b)? a : b;
}

int max( int a, int b){
	return (a >= b)? a : b;
}

/*
void stencil( int read, int write, int x ){
	// stencil operation
	space[write][x] = (space[read][x-1] + space[read][x] + space[read][x+1])/3;
}
*/

void initJacobi(){
	// if init has not already been called (preserve things like global seed.
	if( ! initedJacobi ){
		// note the convention someVar = ( someVar == -1 )? defaultValue : someVar ;
		// this allows us to use the cmd line flags to set variables, AND have an init call.
		// all values are initialized with -1 in global space, so if someVar == -1, then it has
		// not been set, and and be given a default value.
		
		// seed for random number generator.
		// allows all initSpace calls to generate the same inital values
		globalSeed = (globalSeed== -1)? time(NULL) : globalSeed;
		
		// problemSpace parameters
		T = (T == -1)? 100 : T;
		problemSize = (problemSize == -1)? 1000000 : problemSize;
		lowerBound = 1;
		upperBound = lowerBound + problemSize - 1;
		
		cores = (cores == -1)? omp_get_num_procs() : cores ;
		omp_set_num_threads( cores );
				
		// set initialization flag
		initedJacobi = true;
	}
}

// initialize space array
void initSpace(){
	// if space has been previously allocated, free up space.
	if( space[0] != NULL ){
		free( space[0] );
	}
	
	if( space[1] != NULL ) {
		free( space[1] );
	}
	
	/*
	// allocate space
	space = (double**) malloc( 2 * sizeof(double*) );
	if( space == NULL ){
		printf( "Could not allocate space array\n" );
		exit(0);
	}
	*/
	
	// allocate time-steps 0 and 1
	space[0] = (double*) malloc( (problemSize + 2) * sizeof(double));
	space[1] = (double*) malloc( (problemSize + 2) * sizeof(double));
	if( space[0] == NULL || space[1] == NULL ){
		printf( "Could not allocate space array\n" );
		exit(0);
	}
	
	
	// use global seed to seed the random number gen (will be constant)
	srand(globalSeed);
	// seed the space.
	int x;
	for( x = lowerBound; x <= upperBound; ++x ){
		space[0][x] = rand() / (double)rand();
	}
	
	// set halo values (sanity)
	space[0][0] = 0;
	space[0][upperBound+1] = 0;
	space[1][0] = 0;
	space[1][upperBound+1] = 0;
	
	
	
}


// parse int abstraction from strtol
int parseInt( char* string ){
	return (int) strtol( string, NULL, 10 );
}


bool verifyResult( bool verbose ){
	assert( space[0] != NULL && space[1] != NULL );
	
	double* endSpace = (double*) malloc( (problemSize + 2) * sizeof(double) );
	
	for( int x = 0; x < problemSize + 2; ++x ){
		endSpace[x] = space[T & 1][x];
	}
	
	initSpace();
	int read = 0, write = 1;
	for( int t = 1; t <= T; ++t ){
		for( int x = lowerBound; x <= upperBound; ++x ){
			stencil(read, write, x);
		}
		read = write;
		write = 1 - write;
	}
	
	bool failed = false;
	
	for( int x = lowerBound; x <= upperBound; ++x ){
		if( endSpace[x] != space[T & 1][x] ){
			failed = true;
			if( verbose ) printf( "FAILED\n");// %f != %f at %d\n", endSpace[x], space[T & 1][x], x );
			break;
		}
	}
	
	if( verbose && !failed ) printf( "SUCCESS\n" );
	
	free( endSpace );
	
	return !failed;
}



bool initedTrapezoid = false;
int timeBand = -1, width_max = -1, width_min = -1;
int tiles_A_start = -1, tiles_B_start = -1, betweenTiles = -1;
int count_A_tiles = 0, count_B_tiles = 0;
int A_tiles_per_core = 0, B_tiles_per_core = 0;

int countTiles( char type ){
   int x0;
   int count = 0;
   if( type == 0 || type == 'a' || type == 'A' ){
         for( x0 = tiles_A_start; x0 <= upperBound; x0 += betweenTiles ){
            count += 1;
         }
   } else if( type == 1 || type == 'b' || type == 'B' ){
      for( x0 = tiles_B_start; x0 <= upperBound; x0 += betweenTiles ){
         count += 1;
      }
   } 
   
   
   return count;
}


void initTrapezoid(){
	// if init has not already been called (preserve things like global seed.
	if( initedJacobi && !initedTrapezoid  ){
		// note the convention someVar = ( someVar == -1 )? defaultValue : someVar ;
		// this allows us to use the cmd line flags to set variables, AND have an init call.
		// all values are initialized with -1 in global space, so if someVar == -1, then it has
		// not been set, and and be given a default value.
		
		// tile size parameters
		timeBand = (timeBand == -1)? 100 : timeBand;
		width_max = (width_max == -1)? 54701 : width_max ;
		width_min = (width_max + -1 * timeBand) - (0 + 1 * timeBand) +1;
		tiles_A_start = lowerBound - timeBand + 1; // starting point for doing 'A' tiles loops
		tiles_B_start = tiles_A_start + width_max; // starting point for doing 'B' tiles loop
		betweenTiles = width_min + width_max; // width between the first x0 point and next x0 point
		
		// asser that this is a valid tile
		assert( width_min >= 1 && width_max >= width_min );
		
		count_A_tiles = countTiles( 'a' );
		count_B_tiles = countTiles( 'b' );
		A_tiles_per_core = max( 1, count_A_tiles / cores );
		B_tiles_per_core = max( 1, count_B_tiles / cores );
		
		//printf("count A tiles: %d\n", count_A_tiles );
		//printf("count B tiles: %d\n", count_A_tiles );
		// set initialization flag
		initedTrapezoid = true;
	}
}


// Parallel Tiling
double test_1(){
    
    double start_time = omp_get_wtime();
    
    int write, read; // read and write buffers
	int t0, t1, x0, x1, dx0, dx1; // most values of the tile tuples
	int t, x; // indices into space (t,x)
	
	// for all t0 in t0..T by timeBand
	for( t0 = 1; t0 <= T; t0 += timeBand ) {
		// set and clamp t1 from t0
		t1 = min(t0 + timeBand - 1, T); 
		
		// Do A-tiles
		
		// set dx0 and dx1 to correct A-tile values
		dx0 = 1; 
		dx1 = -1;
		
		// iterate over all x0 points for A-tiles
		#pragma omp parallel for private(  x0, x1, write, read, t, x) schedule(dynamic, A_tiles_per_core)
		for( x0 = tiles_A_start; x0 <= upperBound; x0 += betweenTiles ){
			x1 = x0 + width_max - 1; // set x1 from x0
			
			// Set read and write buffer. 
			// this is equivilent to t0 % 2 but assumed faster
			read = (t0 - 1) & 1;
			write = 1 - read; 
			
			// if x0 is at or below lower bound (left edge tile)
			if( x0 <= lowerBound ) {
				//printf("%d, %d, %d, %d, %d, %d\n", lowerBound, 0, x1, dx1, t0, t1 );
				// for t in t0 ... t1
				
				for( t = t0; t<= t1; ++t ){
					//#pragma omp parallel for private( x ) schedule(static)
					// for x in lowerBound ... x1'ish
					int minVal = min(x1 + dx1 * (t - t0), upperBound );
					for( x = lowerBound; x <= minVal; ++x){
						stencil( read, write, x ); // stencil computation
					}// for x
					// flip write buffer
					read = write;
					write = 1 - write;
					
				}// for t
			}// if x0 <= lowerBound
			
			// if x1 is at or above upper bound (right edge tile)
			else if( x1 >= upperBound ){
				//printf("%d, %d, %d, %d, %d, %d\n", x0, dx0, upperBound, 0, t0, t1 );
				// for t in t0...t1
				for( t = t0; t<= t1; ++t ){
					//#pragma omp parallel for private( x ) schedule(static)	
					// for x in x0'ish ... upperbound
					for( x = max(x0 + dx0 * (t - t0), lowerBound); x <= upperBound; ++x){
						stencil( read, write, x ); // stencil computation
					}// for x
					// flip write buffer
					read = write;
					write = 1 - write;
				}// for t
			}// else if x1 >= upperBound
			
			// otherwise regular ol' tile
			else {
				//printf("%d, %d, %d, %d, %d, %d\n", x0, dx0, x1, dx1, t0, t1 );
				// for t in t0 ... t1
				for( t = t0; t<= t1; ++t ){	
					//#pragma omp parallel for private( x ) schedule(static)	
					// for x in x0'ish ... x1'ish	
					int minVal = min(x1 + dx1 * (t - t0), upperBound );
					for( x = max(x0 + dx0 * (t - t0), lowerBound); x <= minVal; ++x){
						stencil( read, write, x ); // stencil computation
					}// for x
					// flip write buffer
					read = write;
					write = 1 - write;
				}// for t
			}// else
		}// for A-tiles
		
		
		// Do B-tiles
		
		// set dx0 and dx1 to correct B-tile values
		dx0 = -1;
		dx1 = 1;
		
		// iterate over x0 points for B-tiles
		#pragma omp parallel for private(  x0, x1, write, read, t, x) schedule(dynamic,B_tiles_per_core)
		
		for( x0 = tiles_B_start; x0 <= upperBound; x0 += betweenTiles ){
			x1 = x0 + width_min - 1; // set x1 from x0
			
			// Set write buffer. 
			// this is equivilent to (t0 - 1 )% 2, but assumed faster
			read = (t0 - 1) & 1;
			write = 1 - read;
			
			// if x1 is at or above upper bound (right edge tile)
			if( x1 >= upperBound ){
				//printf("%d, %d, %d, %d, %d, %d\n", x0, dx0, upperBound, 0, t0, t1 );
				// for t in t0 ... t1
				for( t = t0; t <= t1; ++t ){
					//#pragma omp parallel for private( x ) schedule(static)
					// for x in x0'ish ... upper bound
					for( x = max( x0 + dx0 * (t - t0), lowerBound); x <= upperBound; ++x){
						stencil( read, write, x ); // stencil computation
					}// for x
					// flip write buffer
					read = write;
					write = 1 - write;
				}// for t
			}// if x1 >= upperBound
			
			// regular ol' tile
			else {
				//printf("%d, %d, %d, %d, %d, %d\n", x0, dx0, x1, dx1, t0, t1 );
				// for t in t0 ... t1
				for( t = t0; t<= t1; ++t ){	
					//#pragma omp parallel for private( x ) schedule(static)	
					// for x in x0'ish ... x1'ish	
					int minVal = min(x1 + dx1 * (t - t0), upperBound);
					for( x = max(x0 + dx0 * (t - t0), lowerBound); x <= minVal; ++x){
						stencil( read, write, x ); // stencil computation
					}// for x
					// flip write buffer
					read = write;
					write = 1 - write;
					
				} // for t
			}// else
		} // for B-tiles
	}// for t0
    
    double end_time = omp_get_wtime();
    return (end_time - start_time);
}

int main( int argc, char* argv[] ){
    setbuf(stdout, NULL); // set buffer to null, so prints ALWAYS print (for debug purposes mainly)
    
    bool verify = false;
    bool printtime = true;
    // Command line parsing
    char c;
    while ((c = getopt (argc, argv, "nc:s:p:T:t:w:hv")) != -1){
        switch( c ) {
            case 'n': // print time
                printtime = false;
                break;
            case 'c': // cores
                cores = parseInt( optarg );
                if( cores <= 0 ){
                    fprintf(stderr, "cores must be greater than 0: %d\n", cores);
                    exit( 0 );
                }
                break;
                
            case 'p': // problem size
                problemSize = parseInt( optarg );
                if( problemSize <= 0 ){
                    fprintf(stderr, "problemSize must be greater than 0: %d\n", problemSize);
                    exit( 0 );
                }
                break;
                
            case 'T': // T (time steps)
                T = parseInt( optarg );
                if( T <= 0 ){    
                    fprintf(stderr, "T must be greater than 0: %d\n", T);
                    exit( 0 );
                }
                break;
                
            case 't': // timeBand
                timeBand = parseInt( optarg );
                if( timeBand <= 0 ){
                    fprintf(stderr, "t must be greater than 0: %d\n", T);
                    exit( 0 );
                }
                break;
                
            case 'w': // width
                width_max = parseInt( optarg );
                if( width_max <= 0 ){
                    fprintf(stderr, "w must be greater than 0: %d\n", T);
                    exit( 0 );
                }
                break;
                
            case 'h': // help
               printf("usage: %s\n-n \t dont print time \n-p <problem size> \t problem size in elements \n-T <time steps>\t number of time steps\n-c <cores>\tnumber of threads\n-w <tile width>\t the width of the tile\n-t <tile height>\t the number of timesteps in a tile\n-h\tthis dialogue\n-v\tverify output\n", argv[0]);
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

    initJacobi();
    initTrapezoid();
    initSpace();
    double time = test_1();
    if( printtime ){
        printf( "Time: %f\n", time );
    }
    if( verify ){
        verifyResult( true );
    }
    
}
