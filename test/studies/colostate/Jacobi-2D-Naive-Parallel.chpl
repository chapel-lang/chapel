/*
    Jacobi-3D (2d-space x time) Naive Parallel implementation
    Ian J. Bertolacci (Colorado State Univeristy)
    9/18/2014
*/

use Time;
use Random;

config const printTime: bool = true; // print timer

type Cell = real(64);

config const globalSeed = SeedGenerator.currentTime;

config const problemSize = 1000;
config const T = 100;
config const lowerBound = 1;
const upperBound = lowerBound + problemSize - 1;

const computationSpaceDomain: domain(2) = { lowerBound .. upperBound, lowerBound .. upperBound };
const computationTimeRange: range = 1 .. T;

var space: [ 0..1, lowerBound - 1 .. upperBound + 1, lowerBound - 1 .. upperBound + 1] Cell;

// space initialization function
proc initializeSpace( seed = globalSeed ){
   var generator = new RandomStream( seed, parSafe = false );

   for (x, y) in computationSpaceDomain {
	   space[0, x, y] = generator.getNext();
   } 

} 

// stencil method(s)
inline proc stencil( t: int, x: int, y: int ){
   stencil( (t -1) & 1, t & 1, x, y );
}


inline proc stencil( read: int, write: int, x: int, y: int ){
   space[write, x, y] = ( space[read, x,y] 
							   + space[read, x-1,y] + space[read, x+1,y] 
							   + space[read, x,y-1] + space[read, x,y+1] )/5;
}

proc verifyResult( verbose: bool = true ): bool {
  var spaceEndState: [computationSpaceDomain] Cell;

  for (x, y) in computationSpaceDomain do
     spaceEndState[ x, y ] = space[ T & 1, x, y ];
     
  initializeSpace( );

  var read = 0;
  var write = 1;
  
  for t in computationTimeRange {
     for (x, y) in computationSpaceDomain do
        stencil( t, x, y );
     //read <=> write;   
  }      
  
  var passed: bool = true;
  
  for (x, y) in computationSpaceDomain do
     if spaceEndState[x, y] != space[ T & 1, x, y ]  {
        if verbose {
            writeln( "FAILED! ", spaceEndState[x, y] , " != ", space[ T & 1, x, y ] , " at ", x, ", ", y );
            writeln( "diff: ", abs( spaceEndState[x, y] - space[ T & 1, x, y ] ) );
        }   
        passed = false;
        break;
     }
  
  if passed && verbose then
     writeln( "SUCCESS!" );
  
  return passed;
  
}

proc Test_1(){
    var timer: Timer;
	var read: int = 0, write: int = 1;
	
	initializeSpace();
	
	timer.start();
	
	for t in computationTimeRange {
		forall (x, y) in computationSpaceDomain {
			stencil( read, write, x, y);
		}
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
