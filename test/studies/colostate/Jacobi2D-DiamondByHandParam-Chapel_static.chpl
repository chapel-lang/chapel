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
config const globalSeed = SeedGenerator.oddCurrentTime;
config const problemSize = 1000;
config const T = 100; // number of time steps
config const tau: int = 30;
config const verify: bool;

type Cell = real(64);

// DiamondTileIterator (serial)
iter DiamondTileIterator(lowerBound: int, upperBound: int, T: int, tau: int): 4*int {

  var num_tiles: int = 0;
  var tau_times_3: int = tau*3;

  // Set lower and upper bounds for spatial dimensions.
  var Li: int = lowerBound, Lj: int = lowerBound,
      Ui: int = upperBound, Uj: int = upperBound;
  //var thyme: int, k1: int, k2: int, t: int, j: int;

  // Loop over tile wavefronts.
  //for (thyme=-2; thyme<=floord(3*T,tau); thyme++) {
  for thyme in -2 .. floord(3*T,tau) {

    // The next two loops iterate within a tile wavefront.
    var k1_lb: int = floord(3*Lj+2+(thyme-2)*tau,tau_times_3);
    var k1_ub: int = floord(3*Uj+(thyme+2)*tau-2,tau_times_3);

    // These bounds have been unskewed by adding in k1.
    // That also turns them into a bounding box.
    var k2_lb: int = floord((2*thyme-2)*tau-3*Ui+2,tau_times_3);
    var k2_ub: int = floord((2+2*thyme)*tau-2-3*Li,tau_times_3);

//#pragma omp parallel for shared(start_time, Li, Lj, Ui, Uj ) private(read, write,k1,k2,t,i,j)
    //for (k1=k1_lb; k1<=k1_ub; k1++) {
    for k1 in k1_lb .. k1_ub {
      var read: int = 0, write: int = 1;
      //for (int x=k2_lb; x<=k2_ub; x++) {
      for x in k2_lb .. k2_ub {
        num_tiles += 1;
        var k2 = x-k1; // skew back
        // Don't have to check bounds based on k1 because the skew
        // was the only dependence of k2 bounds on k1.

        // Loop over time within a tile.
        /*
        for (t=max(1,floord(thyme*tau-1,3));
             t<= min(T,floord((3+thyme)*tau-3,3)); t++) { */
        for t in max(1,floord(thyme*tau-1,3)) .. min(T,floord((3+thyme)*tau-3,3)){
          // if t % 2  is 1, then read=0 and write=1
          write = t & 1;
          read = 1-write;

          // Loops over spatial dimensions within tile.
          /*for (i=max(Li,max((thyme-k1-k2)*tau-t, 2*t-(2+k1+k2)*tau+2));
               i<=min(Ui,min((1+thyme-k1-k2)*tau-t-1, 2*t-(k1+k2)*tau)); i++) {*/
           for i in max(Li,max((thyme-k1-k2)*tau-t, 2*t-(2+k1+k2)*tau+2))
                 .. min(Ui,min((1+thyme-k1-k2)*tau-t-1, 2*t-(k1+k2)*tau)) {
            /*for (j=max(Lj,max(tau*k1-t,t-i-(1+k2)*tau+1));
                 j<=min(Uj,min((1+k1)*tau-t-1,t-i-k2*tau)); j++) {*/
              for j in max(Lj,max(tau*k1-t,t-i-(1+k2)*tau+1))
                    .. min(Uj,min((1+k1)*tau-t-1,t-i-k2*tau)){
               yield (read, write, i, j);
            } // for j
          } // for i
        } // for t
      } // for k2
    } // for k1
  } // for thyme
} // DiamondTileIterator (serial)


// DiamondTileIterator (parallel)
iter DiamondTileIterator( lowerBound: int, upperBound: int, T: int,
                          tau: int,
                          param tag: iterKind): 4*int
                          where tag == iterKind.standalone {
  var tau_times_3: int = tau*3;

  // Set lower and upper bounds for spatial dimensions.
  var Li: int = lowerBound, Lj: int = lowerBound,
      Ui: int = upperBound, Uj: int = upperBound;
  //var thyme: int, k1: int, k2: int, t: int, j: int;

  // Loop over tile wavefronts.
  //for (thyme=-2; thyme<=floord(3*T,tau); thyme++) {
  for thyme in -2 .. floord(3*T,tau) {

    // The next two loops iterate within a tile wavefront.
    var k1_lb: int = floord(3*Lj+2+(thyme-2)*tau,tau_times_3);
    var k1_ub: int = floord(3*Uj+(thyme+2)*tau-2,tau_times_3);

    // These bounds have been unskewed by adding in k1.
    // That also turns them into a bounding box.
    var k2_lb: int = floord((2*thyme-2)*tau-3*Ui+2,tau_times_3);
    var k2_ub: int = floord((2+2*thyme)*tau-2-3*Li,tau_times_3);

//#pragma omp parallel for shared(start_time, Li, Lj, Ui, Uj ) private(read, write,k1,k2,t,i,j)
    //for (k1=k1_lb; k1<=k1_ub; k1++) {
    forall k1 in k1_lb .. k1_ub {
      var read: int = 0, write: int = 1;
      //for (int x=k2_lb; x<=k2_ub; x++) {
      for x in k2_lb .. k2_ub {
        var k2 = x-k1; // skew back
        // Don't have to check bounds based on k1 because the skew
        // was the only dependence of k2 bounds on k1.

        // Loop over time within a tile.
        /*
        for (t=max(1,floord(thyme*tau,3));
             t<= min(T,floord((3+thyme)*tau-3,3)); t++) { */
        for t in max(1,floord(thyme*tau,3)) .. min(T,floord((3+thyme)*tau-3,3)){
          // if t % 2  is 1, then read=0 and write=1
          write = t & 1;
          read = 1-write;

          // Loops over spatial dimensions within tile.
          /*for (i=max(Li,max((thyme-k1-k2)*tau-t, 2*t-(2+k1+k2)*tau+2));
               i<=min(Ui,min((1+thyme-k1-k2)*tau-t-1, 2*t-(k1+k2)*tau)); i++) {*/
           for i in max(Li,max((thyme-k1-k2)*tau-t, 2*t-(2+k1+k2)*tau+2))
                 .. min(Ui,min((1+thyme-k1-k2)*tau-t-1, 2*t-(k1+k2)*tau)) {
            /*for (j=max(Lj,max(tau*k1-t,t-i-(1+k2)*tau+1));
                 j<=min(Uj,min((1+k1)*tau-t-1,t-i-k2*tau)); j++) {*/
              for j in max(Lj,max(tau*k1-t,t-i-(1+k2)*tau+1))
                    .. min(Uj,min((1+k1)*tau-t-1,t-i-k2*tau)){
               yield (read, write, i, j);
            } // for j
          } // for i
        } // for t
      } // for k2
    } // for k1
  } // for thyme
}// DiamondTileIterator (parallel)


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
  const upperBound = problemSize ; // end of computation space

  // create a haloed domain
  var totalSpaceRange: domain(2) = { 0..problemSize+1, 0..problemSize+1};
  var computationTimeRange = 1..T;
  var computationDomain: domain(2) = totalSpaceRange.expand(-1);

  // Storage array.
  var space: [0..1, totalSpaceRange.dim(0), totalSpaceRange.dim(1) ] Cell;
  var timer: Timer;

  // initialize space with values
  var generator = new RandomStream( real, globalSeed, parSafe = false );

  forall (x,y) in computationDomain do{
     space[0, x, y] = 0;
     space[1, x, y] = 0;
  }

  for (x, y) in computationDomain do
     space[0, x, y] = generator.getNext();

  //var read: int = 0, write: int = 1;

  // 3 - jacobi 1D timed within an openmp loop
  timer.start();

  //forall (x,y) in computationDomain do
  forall (read, write, x ,y) in DiamondTileIterator(lowerBound, upperBound, T, tau){
  //forall (x,y) in computationDomain {
      space[write, x, y] = (space[read, x, y-1] +
                            space[read, x-1, y] +
                            space[read, x, y+1] +
                            space[read, x+1, y] +
                            space[read, x, y] ) / 5;
  }

  //read <=> write;

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
proc verifyResult( space: [] Cell, computationalDomain: domain(2),
                   verbose: bool = true, T: int ): bool {

  var computationTimeRange = 1..T;

  var spaceEndState: [computationalDomain] Cell;

  forall (x, y) in computationalDomain do
     spaceEndState[ x, y ] = space[ T & 1, x, y ];

  var generator = new RandomStream( real, globalSeed, parSafe = false );

  for (x, y) in computationalDomain do
     space[0, x, y] = generator.getNext();

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

proc floord( x: int , y: int ): int {
  assert( y > 0 && ( if x % y >= 0 then x % y else (x%y) +y) >= 0 && (if x%y>=0 then x%y else (x%y) +y) <= y && x==(y*( if x%y>=0 then x/y else ((x/y) -1)) + ( if x%y>=0 then x%y else (x%y) +y)) );
  return (if x%y>=0 then x/y else (x/y) -1);
}

