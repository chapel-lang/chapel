/******************************************************************************
 * Jacobi1D benchmark
 * Basic parallelism over inner loop forall
 *
 * Usage:
 * make chapel
 * export OMP_NUM_THREADS=8
 * bin/Jacobi1D-NaiveParallel-Chapel \
 * `cat src/Jacobi1D-NaiveParallel-Chapel.perfexecopts`
 * For a run on 8 threads
 *
 * Ian J. Bertolacci - CSU
 ******************************************************************************/
use Time;
use Random;
use IO;

config const printTime: bool = true; // print timer
config const globalSeed = SeedGenerator.oddCurrentTime;
config const problemSize = 100000;
config const T = 1000; // number of time steps
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
  const upperBound = lowerBound + problemSize - 1; // end of computation space

  // create a haloed domain
  var totalSpaceRange = lowerBound - 1 .. upperBound + 1;
  var computationTimeRange = 1..T;
  var computationSpaceRange = lowerBound..upperBound;

  // Storage array.
  var space: [0..1, totalSpaceRange ] Cell;
  var timer: Timer;
  // initialize space with values
  var generator = new RandomStream( real, globalSeed, parSafe = false );

  forall i in computationSpaceRange do{
     space[0, i] = 0;
     space[1, i] = 0;
  }

  for i in computationSpaceRange do
     space[0, i] = generator.getNext();

  // because the serial method does not give us read and write buffers
  // we must make our own
  var read = 0;
  var write = 1;

  // 3 - jacobi 1D timed within an openmp loop
  timer.start();

  for t in computationTimeRange {
      forall x in computationSpaceRange do
          space[write, x] = (space[read, x-1] +
                             space[read, x] +
                             space[read, x+1]) / 3;
      read <=> write;
  }

  timer.stop();

  var time = timer.elapsed();

  // 4 - output and optional verification
  if printTime then stdout.write( "Time: ", time );

  if verify {
    if verifyResult(space,lowerBound,upperBound,false ) then writeln( "SUCCESS" );
    else writeln( "FAILURE" );
  }
}


// return true if the current end state is the same as the
// stencil applied to the original state, in serial iteration.
proc verifyResult(space: [] Cell, lowerBound: int, upperBound: int,
                   verbose: bool = true ): bool {

  var totalSpaceRange = lowerBound - 1 .. upperBound + 1;
  var computationTimeRange = 1..T;
  var computationSpaceRange = lowerBound..upperBound;

  var spaceEndState: [computationSpaceRange] Cell;

  for x in computationSpaceRange do
     spaceEndState[ x ] = space[ T & 1, x ];

  var generator = new RandomStream( real, globalSeed, parSafe = false );

  for i in computationSpaceRange do
     space[0, i] = generator.getNext();

  var read = 0;
  var write = 1;

  for t in computationTimeRange {
     for x in computationSpaceRange do
          space[write, x] = (space[read, x-1] +
                             space[read, x] +
                             space[read, x+1]) / 3;
     read <=> write;
  }

  var passed: bool = true;

  for x in computationSpaceRange do
     if spaceEndState[x] != space[ T & 1, x ]  {
        if verbose then
           writeln( "FAILED! ", spaceEndState[x] , " != ", space[ T & 1, x ] , " at ", x );
        passed = false;
        break;
     }

  if passed && verbose then
     writeln( "SUCCESS!" );

  return passed;
}


