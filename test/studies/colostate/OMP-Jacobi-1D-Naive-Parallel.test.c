#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

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


// stencil call.
void stencil( int read, int write, int x ){
	// stencil operation
	space[write][x] = (space[read][x-1] + space[read][x] + space[read][x+1])/3;
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
			if( verbose ) printf( "FAILED\n"); //! %f != %f at %d\n", endSpace[x], space[T & 1][x], x );
			break;
		}
	}
	
	if( verbose && !failed ) printf( "SUCCESS\n" );
	
	free( endSpace );
	
	return !failed;
}



// naive parallel iteration test suite
double test_1(){
    int t, x, read = 0, write = 1;
    
    double start_time = omp_get_wtime();
    
    for( t = 1; t <= T; ++t ){
        #pragma omp parallel for private( x ) //schedule(dynamic)
        for( x = lowerBound; x <= upperBound; ++x ){
        	stencil( read, write, x );
        }
        read = write;
        write = 1 - write;
    }
    
    double end_time = omp_get_wtime();
    return (end_time - start_time);
}

int main( int argc, char* argv[] ){
    setbuf(stdout, NULL); // set buffer to null, so prints ALWAYS print (for debug purposes mainly)
    
    bool verify = false;
    bool printtime = true;
    // Command line parsing
    char c;
    while ((c = getopt (argc, argv, "nc:s:p:T:hv")) != -1){
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
            
            case 'h': // help
                printf("usage: %s\n-n \t dont print time \n-p <problem size> \t problem size in elements \n-T <time steps>\t number of time steps\n-c <cores>\tnumber of threads\n-h\tthis dialogue\n-v\tverify output\n", argv[0]);
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
    initSpace();
    double time = test_1();
    if( printtime ){
        printf( "Time: %f\n", time );
    }
    
    if( verify ){
        verifyResult( true );
    }
    
}
