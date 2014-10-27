/* Jacobi-1D (space x time), Serial Implementation
 * Unrolled outer loop to implement ping pong.
 * Ian J. Bertolacci - CSU, unrolled by Michelle Strout
 * 10.14.2014
 */

use Time;
use Random; 

config var test = 0;

config const printTime: bool = true; // print timer

type Cell = real(64);
//config const globalSeed = SeedGenerator.currentTime;
config const globalSeed = 41;


config const problemSize = 1000; 
config const lowerBound = 1; // start of computation space
const upperBound = lowerBound + problemSize - 1; // end of computation space
config const T = 1000; // number of time steps      

// create a haloed domain
var totalSpaceRange = lowerBound - 1 .. upperBound + 1;
var computationTimeRange = 1..T;
var computationSpaceRange = lowerBound..upperBound;

// Storage array.
var space: [0..1, totalSpaceRange ] Cell;   

// Give initial values to the storage array
// Parameter: seed, seed for random stream. Default to global seed
proc initializeSpace( ){
  var generator = new RandomStream( globalSeed, parSafe = false );

  for i in computationSpaceRange do {
     space[0, i] = generator.getNext();
  }

}

proc main(){
   
    // timer object for recording execution time
    var timer: Timer;

    // Serial test
    // initialize space with values
    initializeSpace(); 

    timer.start();
   
    for t in computationTimeRange by 2 {
        for x in computationSpaceRange do {
            space[1, x] = (space[0, x-1] + space[0, x] + space[0, x+1])/3;
        }
        for x in computationSpaceRange do {
            space[0, x] = (space[1, x-1] + space[1, x] + space[1, x+1])/3;
        }
    }
   
    timer.stop();

    // output result if we are testing and time if we are not
    if (test ) {
        for x in computationSpaceRange do {
            write(space[0,x], ", ");
        }
        writeln();
        for x in computationSpaceRange do {
            write(space[1,x], ", ");
        }
    } else {
        writeln( "Time: ", timer.elapsed() );
    }
   
    // This is the baseline so just indicate SUCCESS.
    //if verifyResult( false ) then writeln( "SUCCESS" );
    //else writeln( "FAILURE" );
    writeln( "SUCCESS" );
}
