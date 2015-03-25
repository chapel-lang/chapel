/* Jacobi-2D (space x time), Serial Implementation
 * Ian J. Bertolacci - CSU
 * 9.18.2014
 */


use Time;
use Random; 

config const printTime: bool = true; // print timer

type Cell = real(64);
config const globalSeed = SeedGenerator.currentTime;


config const problemSize = 100000; 
config const lowerBound = 1; // start of computation space
const upperBound = lowerBound + problemSize - 1; // end of computation space
config const T = 1000; // number of time steps      

// create a haloed domain
var totalSpaceRange = lowerBound - 1 .. upperBound + 1;
var computationTimeRange = 1..T;
var computationSpaceRange = lowerBound..upperBound;

// Storage array.
var space: [0..1, totalSpaceRange ] Cell;   

// procedure computing the 3-point stencil.
inline proc stencil( readBuffer: int, writeBuffer: int, x: int){   
  space[writeBuffer, x] = (space[readBuffer, x-1] + space[readBuffer, x] + space[readBuffer, x+1]) / 3;
}

// masking procedure taking values at time t and updating t+1
inline proc stencil( t: int, x: int ){
  stencil( (t - 1) & 1, t & 1, x );
}

// Give initial values to the storage array
// Parameter: seed, seed for random stream. Default to global seed
proc initializeSpace( ){
  var generator = new RandomStream( globalSeed, parSafe = false );

  for i in computationSpaceRange do
     space[0, i] = generator.getNext();

}

// return true if the current end state is the same as the 
// stencil applied to the original state, in serial iteration.
proc verifyResult( verbose: bool = true ): bool {
  var spaceEndState: [computationSpaceRange] Cell;

  for x in computationSpaceRange do
     spaceEndState[ x ] = space[ T & 1, x ];
     
  initializeSpace( );

  var read = 0;
  var write = 1;
  
  for t in computationTimeRange {
     for x in computationSpaceRange do
        stencil( t, x );
     //read <=> write;   
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
proc Test_1( ) {
   // timer object for recording execution time
   var timer: Timer;

   // Serial test
   // initialize space with values
   initializeSpace(); 

   // because the serial method does not give us read and write buffers
   // we must make our own
   var read = 0;
   var write = 1;
   
   timer.start();
   
   for t in computationTimeRange {
      for x in computationSpaceRange do
         stencil( read, write, x );
      
      read <=> write;
   }
   
   timer.stop();

   return timer.elapsed();
}

proc main(){
   
   var time = Test_1();

   if printTime then writeln( "Time: ", time );
   
   if verifyResult( false ) then writeln( "SUCCESS" );
   else writeln( "FAILURE" );

}
