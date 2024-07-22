/******************************************************************************
 * Jacobi2D benchmark
 * Basic parallelism over inner loop forall
 *
 * Usage:
 * make chapel
 * export OMP_NUM_THREADS=8
 * bin/Jacobi2D-NaiveParallel-Chapel \
 * `cat src/Jacobi2D-NaiveParallel-Chapel.perfexecopts`
 * For a run on 8 threads
 *
 * Ian J. Bertolacci - CSU
 ******************************************************************************/
use Time;
use Random;
use IO;

config const printTime: bool = true; // print timer
config const globalSeed = (new randomStream(int(32))).seed;
config const problemSize = 1000;
config const T = 100; // number of time steps
config const verify: bool;

type Cell = real(64);


// main
// Given that this is a very straight forward benchark the code is almost
// entirely kept within the main function.
// The steps taken in this code are the following:
// 2 - data allocation and initialization
// 3 - jacobi 1D timed within an openmp loop
// 4 - output and optional verification
//
proc main(){

  // 2 - data allocation and initialization
  const lowerBound = 1; // start of computation space
  const upperBound = problemSize; // end of computation space

  // create a haloed domain
  var totalSpaceRange: domain(2) = { 0..problemSize+1, 0..problemSize+1};
  var computationTimeRange = 1..T;
  var computationDomain: domain(2) = totalSpaceRange.expand(-1);

  // Storage array.
  var space: [0..1, totalSpaceRange.dim(0), totalSpaceRange.dim(1) ] Cell;
  var timer: stopwatch;
  // initialize space with values
  var generator = new randomStream(real, globalSeed);

  forall (x,y) in computationDomain with (ref space) do{
     space[0, x, y] = 0;
     space[1, x, y] = 0;
  }

  for (x, y) in computationDomain do
     space[0, x, y] = generator.next();

  // because the serial method does not give us read and write buffers
  // we must make our own
  var read = 0;
  var write = 1;

  // 3 - jacobi 1D timed within an openmp loop
  timer.start();

  const rows = computationDomain.dim(0);
  const cols = computationDomain.dim(1);
  for t in computationTimeRange {
      //forall (x,y) in dynamic(computationDomain,chunkSize=1) do
      //forall (x,y) in computationDomain do
      forall x in rows with (ref space) {
        for y in cols do
          space[write, x, y] = (space[read, x, y-1] + space[read, x-1, y] +
                                space[read, x, y+1] + space[read, x+1, y] +
                                space[read, x, y] ) / 5;
      }
      read <=> write;
  }

  timer.stop();

  var time = timer.elapsed();

  // 4 - output and optional verification
  if printTime then stdout.write( "Time: ", time );

  if verify {
    if verifyResult(space, computationDomain, verbose=false, T ) then writeln( "SUCCESS" );
    else writeln( "FAILURE" );
  }
}


// return true if the current end state is the same as the
// stencil applied to the original state, in serial iteration.
proc verifyResult( ref space: [] Cell, computationalDomain: domain(2),
                   verbose: bool = true, T: int ): bool {

  var computationTimeRange = 1..T;

  var spaceEndState: [computationalDomain] Cell;

  forall (x, y) in computationalDomain with (ref spaceEndState) do
     spaceEndState[ x, y ] = space[ T & 1, x, y ];

  var generator = new randomStream( real, globalSeed );

  for (x, y) in computationalDomain do
     space[0, x, y] = generator.next();

  var read = 0;
  var write = 1;

  for t in computationTimeRange {
     for (x,y) in computationalDomain do
          space[write, x, y] = (space[read, x, y-1] + space[read, x-1, y] +
                                space[read, x, y+1] + space[read, x+1, y] +
                                space[read, x, y] ) / 5;
     read <=> write;
  }

  var passed: bool = true;

  for (x, y) in computationalDomain do
     if spaceEndState[x,y] != space[ T & 1, x, y]  {
        if verbose then
           writeln( "FAILED! ", spaceEndState[x,y] , " != ", space[ T & 1, x, y ] , " at ", x, ", ", y );
        passed = false;
        break;
     }

  if passed && verbose then
     writeln( "SUCCESS!" );

  return passed;

}
