// verification
// All that verification does is to run the exact same 
// computation in a serial manner.
// Therefore, all we are checking is that our we did not break it when
// going parallel
bool verifyResult( double* result, int problemSize, int seed, int steps ){

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
    exit(0);
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

