#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
//#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

bool initialized;
int globalSeed;
int cores;
int FP_OPS_PER_ITERATION;  // just used for outputting approximate MFLOPS

int problemSize; 
int T; 
int lowerBound;
int upperBound;

double** space[2];

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
                 if( verbose ) printf( "FAILED\n"); //! %f != %f at %d, %d\n", endSpace[x][y],space[ T & 1 ][x][y], x, y);
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

// naive parallel iteration test suite
double test_1(){
    int t, x, y, read = 0, write = 1;
    
    double start_time = omp_get_wtime();
    
    for( t = 1; t <= T; ++t ){
        {
        #pragma omp parallel for private( x, y ) schedule(dynamic)
        for( x = lowerBound; x <= upperBound; ++x ){
            for( y = lowerBound; y <= upperBound; ++y ){
                STENCIL( read, write, x, y);
            }        
        }
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

    init();
    initSpace();
    double time = test_1();
    if( printtime ){
        printf( "Time: %f\n", time );
    }
    
    if( verify ){
        verifyResult( true );
    }
    
}
