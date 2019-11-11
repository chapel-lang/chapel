/******************************************************************************
 * Jacobi1D benchmark
 * DiamondSlab Tiling
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~ IN PROGRESS ~~~~~~~~~~~~~~~~~~~~~~~~~~
 * TODO:
 * <strike> 1) Figure out how to do Standalone parallel iterator</strike>
 * <strike> 1.1) Figure out if a standalone iterator will work with C code
 *    Given the format, it will not. need leader follower.
 *    Means need serial.
 * </strike>
 * <strike> 2) Convert aforementioned C loop-nest code to Chapel iterator code.
 * <strike>
 * <strike> 3) Create associated test files for this </strike>
 * 4) Wait for Standalone par. iter to take parameters, and convert to that.
 *
 * Usage:
 * make chapel
 * export OMP_NUM_THREADS=8
 * bin/Jacobi1D-DiamondSlabByHand-Chapel \
 * `cat src/Jacobi1D-DiamondSlabByHand-Chapel.perfexecopts`
 * For a run on 8 threads
 *
 * Ian J. Bertolacci - CSU
 ******************************************************************************/
use Time;
use Random;
use IO;
use DynamicIters;

config const printTime: bool = true; // print timer
config const globalSeed = SeedGenerator.oddCurrentTime;
config const problemSize = 100000;
config const T = 1000; // number of time steps
config const tau = 10;
config const verify: bool;

type Cell = real(64);
// tile type, describes tile shape: (x_0, dx_0, x_1, dx_1, t_0, t_1)
type Tile = (int, int, int, int, int, int);


// DiamondSlab Tile Iterator (serial)
// This will produce the indicies for the loop.
iter DiamondTileIterator( lowerBound: int, upperBound: int, T: int,
                          tau: int): 3*int {
  var Ui = upperBound, Li = lowerBound;
  for thyme in floord(2, tau)-2 .. floord(T*2, tau){
    for  k1 in ((Ui*2/(tau: real)-thyme+1 )/-2): int
            .. (( (Li*2/(tau: real))-thyme-1)/ -2): int {
      // begin inner loops over points in tile
      for t in max(1, floord(thyme*tau - k1*tau + k1*tau + 1, 2))
               .. min(T+1, tau + floord(thyme*tau - k1*tau + k1*tau, 2)) - 1 {
        var write = t & 1;
        var read = 1-write;
        for i in max(Li, max(thyme*tau - k1*tau - t, -tau - k1*tau + t + 1))
               ..min(Ui, min(tau + thyme*tau - k1*tau - t - 1, -k1*tau + t)) {
          yield( read, write, i );
         // (t, i);
        } // i
      } // t
    } // k1
  }// thyme
}// DiamondTileIterator


// DiamondSlab Tile Iterator (Parallel)
// This will produce the indicies for the loop.
iter DiamondTileIterator( lowerBound: int, upperBound: int, T: int,
                              tau: int,
                              param tag: iterKind): 3*int
                              where tag == iterKind.standalone {
    var Ui = upperBound, Li = lowerBound;
  for thyme in floord(2, tau)-2 .. floord(T*2, tau){
    forall  k1 in dynamic( ((Ui*2/(tau: real)-thyme+1 )/-2): int
            .. (( (Li*2/(tau: real))-thyme-1)/ -2): int, chunkSize=1) {
      // begin inner loops over points in tile
      for t in max(1, floord(thyme*tau - k1*tau + k1*tau + 1, 2))
               .. min(T+1, tau + floord(thyme*tau - k1*tau + k1*tau, 2)) - 1 {
        var write = t & 1;
        var read = 1-write;
        for i in max(Li, max(thyme*tau - k1*tau - t, -tau - k1*tau + t + 1))
               ..min(Ui, min(tau + thyme*tau - k1*tau - t - 1, -k1*tau + t)) {
          yield( read, write, i );
         // (t, i);
        } // i
      } // t
    } // k1
  }// thyme

}// DiamondTileIterator

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

  // 3 - jacobi 1D timed within an openmp loop
  timer.start();

  forall (read, write, x) in DiamondTileIterator( lowerBound, upperBound, T,
                                                   tau ){
    space[write, x] = (space[read, x-1] +
                       space[read, x] +
                       space[read, x+1]) / 3;
  }

  timer.stop();

  var time = timer.elapsed();;

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

  var read  = 0;
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
        writeln( "FAILED! ", spaceEndState[x] , " != ", space[ x, T & 1 ] , " at ", x );
      passed = false;
      break;
    }

  if passed && verbose then
    writeln( "SUCCESS!" );

  return passed;
}

proc floord( x: int , y: int ): int {
  assert( y > 0 && ( if x % y >= 0 then x % y else (x%y) +y) >= 0 && (if x%y>=0 then x%y else (x%y) +y) <= y && x==(y*( if x%y>=0 then x/y else ((x/y) -1)) + ( if x%y>=0 then x%y else (x%y) +y)) );
  return (if x%y>=0 then x/y else (x/y) -1);
}
