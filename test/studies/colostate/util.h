/******************************************************************************
 * util.h
 *
 * Is included in the .test.c drivers.
 * Search for "default" to find where the default values for command-line options
 * are set.
 *
 * Also includes the various max, min, and floord definitions we need for iscc-generated code.
 ******************************************************************************/
//==== Needed for generated and modified iscc code.
#define min(a,b)	((a)>(b)?(b):(a))
#define max(a,b)	((a)>(b)?(a):(b))

#if defined NDEBUG
#define eassert(X)	0
#else
static inline int eassert_function(int fact, int line, char *fact_text)
{
  if (!fact)
    {
      fprintf(stderr, "assertion failed, line %d: %s\n", line, fact_text);
      exit(1);
    }
  else
    return 0;
}
#define eassert(X)	eassert_function(X, __LINE__, "X")
#endif

// FIXME: Dave W, what is going on with ASSUME_POSITIVE_INTMOD?
#if ! defined ASSUME_POSITIVE_INTMOD
#define ASSUME_POSITIVE_INTMOD 0
#endif

#if ASSUME_POSITIVE_INTMOD
#define intDiv(x,y)	(eassert(((x)%(y)) >= 0), ((x)/(y)))
#define intMod(x,y)	(eassert(((x)%(y)) >= 0), ((x)%(y)))
#else
#define intDiv_(x,y)	((((x)%(y))>=0) ? ((x)/(y)) : (((x)/(y)) -1))
#define intMod_(x,y)	((((x)%(y))>=0) ? ((x)%(y)) : (((x)%(y)) +y))
#define checkIntDiv(x,y) (eassert((y) > 0 && intMod_((x),(y)) >= 0 && intMod_((x),(y)) <= (y) && x==((y)*intDiv_((x),(y)) + intMod_((x),(y)))))
#define intDiv(x,y)	(checkIntDiv((x),(y)), intDiv_((x),(y)))
#define intMod(x,y)	(checkIntDiv((x),(y)), intMod_((x),(y)))
#endif

#define ceild(n, d)	intDiv((n), (d)) + ((intMod((n),(d))>0)?1:0)
#define floord(n, d)	intDiv((n), (d))


int omp_thread_count() {
      int n = 0;
          #pragma omp parallel reduction(+:n)
          n += 1;
              return n;
}

//==== parse int abstraction from strtol
 int parseInt( char* string ){
   return (int) strtol( string, NULL, 10 );
  }

//==== verification
// All that verification does is to run the exact same 
// computation in a serial manner.
// Therefore, all we are checking is that our we did not break it when
// going parallel
bool verifyResultJacobi1D(double* result,int problemSize,int seed,int steps){

  // run serial Jacobi 1D
  int  lowerBound = 1;
  int  upperBound = lowerBound + problemSize - 1;
  int x;
  int t, read = 0, write = 1;
  double* space[2];
  bool failed = false;
  space[0] = (double*) malloc( (problemSize + 2) * sizeof(double));
  space[1] = (double*) malloc( (problemSize + 2) * sizeof(double));
  if( space[0] == NULL || space[1] == NULL ){
    printf( "Could not allocate space array\n" );
    exit(-1);
  }
  srand(seed);

  // seed the space. 
  for( x = lowerBound; x <= upperBound; ++x ){
    space[0][x] = rand() / (double)rand();
  }

  // set halo values (sanity)
  space[0][0] = 0;
  space[0][upperBound+1] = 0;
  space[1][0] = 0;
  space[1][upperBound+1] = 0;


  for( t = 1; t <= steps; ++t ){
    for( x = lowerBound; x <= upperBound; ++x ){
      space[write][x] = (space[read][x-1] +
                         space[read][x] +
                         space[read][x+1])/3;
    }
    read = write;
    write = 1 - write;
  }


  for( x = lowerBound; x <= upperBound; ++x ){
    if( space[read][x] != result[x] ){
      fprintf(stderr,"Position: %d, values: %f, %f\n",
                      x,space[read][x],result[x]);
      failed = true;
      break;
    }
  }
  return !failed;
}

/******************************************************************************
 * Command Line Parsing
 *
 * Each benchmark has its own command line parsing because the
 * options vary slightly depending on the tiling approach used.
 ******************************************************************************/
typedef struct {
  // common parameters
  int T; // time step
  int globalSeed;
  int cores;
  int problemSize;
  bool verify;
  bool printtime;
  //==== needed for tiling (ignored by some)
  int timeBand; // time band
  int width_max;

  // diamond tiling approaches derived from iscc
  // Height (i.e., number of slices) in diamond slab.
  // Equivalent to timeBand, but not using that because error checking
  // is related to tau and different.
  // See Jacobi2D-DiamondSlabISCCParam-OMP.test.c for error checking.
  int subset_s;
  // width between tiling hyperplanes, only use command-line param
  // if tau was not set at compile time
  // See Jacobi2D-DiamondSlabISCCParam-OMP.test.c after call to
  // parseCmdLineArgs to see an example of how this works.
  int tau_runtime;

  // this is being used for the naive space tiling
  int tile_len_x;
  int tile_len_y;

} Params;
int parseCmdLineArgs(Params *cmdLineArgs, int argc, char* argv[]){

  // set default values
  cmdLineArgs->T = 100;
  cmdLineArgs->globalSeed = time(NULL); 
  //cmdLineArgs->cores = omp_thread_count();
  cmdLineArgs->cores = omp_get_max_threads();
  cmdLineArgs->problemSize = 100; 
  cmdLineArgs->verify = false;
  cmdLineArgs->printtime = true;
  cmdLineArgs->timeBand = 10;
  cmdLineArgs->width_max = 54701;
  // diamond tile size, can be any multiple of 3 >=15
  cmdLineArgs->tau_runtime = 30;    
  // max val for this
  cmdLineArgs->subset_s = (cmdLineArgs->tau_runtime / 3) -2;    
  // this is a random number, so don't ask me why I chose it
  cmdLineArgs->tile_len_x = 128; 
  cmdLineArgs->tile_len_y = 128; 

  // process incoming
  char c;
  while ((c = getopt (argc, argv, "nc:s:p:T:x:y:a:w:t:hv")) != -1){
    switch( c ) {
      case 'n': // print time
        cmdLineArgs->printtime = false;
        break;
      case 'c': // cores
        cmdLineArgs->cores = parseInt( optarg );
        if(cmdLineArgs->cores <= 0){
          fprintf(stderr, "cores must be greater than 0: %d\n", 
                           cmdLineArgs->cores);
            exit( -1 );
          }
          break;
                
      case 'p': // problem size
        cmdLineArgs->problemSize = parseInt( optarg );
        if( cmdLineArgs->problemSize <= 0 ){
          fprintf(stderr, "problemSize must be greater than 0: %d\n",
                           cmdLineArgs->problemSize);
          exit( -1 );
        }
        break;
                
        case 'T': // T (time steps)
          cmdLineArgs->T = parseInt( optarg );
          if( cmdLineArgs->T <= 0 ){    
            fprintf(stderr, "T must be greater than 0: %d\n", cmdLineArgs->T);
            exit( -1 );
          }
          break;
         case 't': // timeBand
            cmdLineArgs->timeBand = parseInt( optarg );
            if( cmdLineArgs->timeBand <= 0 ){
                fprintf(stderr, "t must be greater than 0: %d\n", 
                                 cmdLineArgs->timeBand);
                exit( -1 );
            }
            break;

         case 'w': // width
            cmdLineArgs->width_max = parseInt( optarg );
            if( cmdLineArgs->width_max <= 0 ){
                fprintf(stderr, "w must be greater than 0: %d\n", 
                                cmdLineArgs->width_max);
                exit( -1 );
            }
            break; 

         case 'a': // tau_runtime
            cmdLineArgs->tau_runtime = parseInt( optarg );
            if( cmdLineArgs->tau_runtime <= 1 ){
                fprintf(stderr, 
                        "-a tau must be greater than 1:"
                        " current value is %d\n", 
                        cmdLineArgs->tau_runtime);
                exit( -1 );
            }
            break; 

         case 'x': // tile_len
            cmdLineArgs->tile_len_x = parseInt( optarg );
            if( cmdLineArgs->tile_len_x < 1){
              fprintf(stderr, "-x: must be >= 1\n");
              exit(-1);
            }
            break;
         case 'y': // tile_len
            cmdLineArgs->tile_len_y = parseInt( optarg );
            if( cmdLineArgs->tile_len_y < 1){
              fprintf(stderr, "-y: must be >= 1\n");
              exit(-1);
            }
            break;

         // Have to check in driver as well.
         // See Jacobi2D-DiamondSlabISCCParam-OMP.test.c for same check.
         case 's': // subset_s
            cmdLineArgs->subset_s = parseInt( optarg );
            // s is the number of non-pointy bit 2D slices of diamond tiling
            // that is available for the current tile size.
            int s = (cmdLineArgs->tau_runtime/3) - 2;
            // subset_s is an input parameter indicating how many of those
            // slices we want to use in the repeated tiling pattern.
            // subset_s should be less than s and greater than or equal to 2.
            if (cmdLineArgs->subset_s > s  || cmdLineArgs->subset_s<2) {
                fprintf(stderr, "-s: need 2<=subset_s<=s\n");
                exit(-1);
            }
            break; 


        case 'h': // help
          printf("usage: %s\n"
                  "-n \t dont print time \n"
                  "-p <problem size> \t problem size in elements \n"
                  "-T <time steps>\t number of time steps\n"
                  "-c <cores>\tnumber of threads\n"
                  "-w <max tile width>\tmax tile width for Jacobi1D-DiamondSlabByHand\n"
                  "-a <tau>\tdistance between tiling hyperplanes (all diamond(slab) but ByHand)\n"
                  "-s <subset_s>\tnumber of slices in slab (all diamondslab but ByHand)\n"
                  "-h\tusage help, this dialogue\n"
                  "-v\tverify output\n", argv[0]);
           exit(0);
            
        case 'v': // verify;
          cmdLineArgs->verify = true;
          break;
            
        case '?':
          if (optopt == 'p'){
            fprintf (stderr,
                   "Option -%c requires positive int argument: problem size.\n",
                   optopt);
          }else if (optopt == 'T'){
            fprintf (stderr,
                     "Option -%c requires positive int argument: T.\n",
                      optopt);
          }else if (optopt == 's'){
            fprintf (stderr,
                     "Option -%c requires int argument: subset_s.\n",
                      optopt);
          }else if (optopt == 'c'){
            fprintf (stderr,
                "Option -%c requires int argument: number of cores.\n",
                 optopt);
          }else if (isprint (optopt)){
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
          }else{
            fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
          }
          exit(-1);
               
        default:
          exit(0);
    }
  } 
  omp_set_num_threads( cmdLineArgs->cores );

  return 0;
}



// returns true if valid result
bool verifyResultJacobi2D( double** result,int problemSize,int seed,int steps){

  int i;
  int lowerBound = 1;
  int upperBound = lowerBound + problemSize - 1;

  // allocate and initialize the space
  double** space[2]; 
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
  srand(seed);
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

  
  int t,read = 0, write = 1;
  for( t = 1; t <= steps; ++t ){
    for( x = lowerBound; x <= upperBound; ++x ){
      for( y = lowerBound; y <= upperBound; ++y ){
        space[write][x][y] = ( space[read][x-1][y] +
                               space[read][x][y] +
                               space[read][x+1][y] +
                               space[read][x][y+1] +
                               space[read][x][y-1] )/5;
      }    
    }
    read = write;
    write = 1 - write;
  }
  
  bool failed = false;
  for( x = lowerBound; x <= upperBound; ++x ){
    for( y = lowerBound; y <= upperBound; ++y ){
      if( result[x][y] != space[ steps & 1 ][x][y] ){
        failed = true;
        break;
      }
    }
    if( failed ) break;
  }
  
  return !failed;

}


