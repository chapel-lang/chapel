/* Jacobi-2D (space x time), Inlined Diamond Slab Tiling Implementation
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
   

type Tile = (int, int, int, int, int, int); // tile type, describes tile shape: (x_0, dx_0, x_1, dx_1, t_0, t_1)
// the following are parameters for tiles
// they are given values when buildTileSize() runs
config const timeBand: int = 50; // time slice size
config const width_max: int = problemSize / here.numCores; // maximum width of a trapezoid
const width_min: int = (width_max + -1 * timeBand) - (0 + 1 * timeBand) +1; // minimum width of trapezoid

// fail if tile is an invalid size.
assert( width_min >= 1 && width_max >= width_min );

proc InlinedTrapezoidTilingProcedure(){

  var tiles_A_start = lowerBound - timeBand + 1; // starting point for doing '1' tiles loops
  var tiles_B_start = tiles_A_start + width_max; // starting point for doing '2' tiles loop
  var betweenTiles = width_min + width_max; // distance to next point in tile set

  var x0, t0, t1 : int; 
  var dx0: int = 1, dx1: int = -1;
  //var readBuffer: int, writeBuffer: int;

  // iterate over time band
  for t0 in 1..T by timeBand{   

     // clamp t1 to T if t1 > T
     t1 = min( t0 + timeBand - 1, T );
  
     // Do A-tiles

     // for all starting points of x0 do a tile
     forall x0 in tiles_A_start .. upperBound by betweenTiles {
     
        var x1 = x0 + width_max - 1;
  
        // this lacks the complexity of previous iterators
        // because we are ignoring it.
  
        // check if left edge tile
        if x0 <= lowerBound {
           //T(lowerBound, 0, x1, dx1, t0, t1)

           // for t in t0..t1
           var readBuffer = (t0 - 1) & 1; 
           var writeBuffer = 1 - readBuffer;
           for t in t0..t1 {

              // for x in x0+dx0*(t-t0) .. x1+dx1*(t-t0) && in computation space
              for x in lowerBound .. min( x1 + dx1 * (t - t0), upperBound ) {         
                 stencil (readBuffer, writeBuffer, x);
              }// for x
              readBuffer <=> writeBuffer;
           }// for t
        
        }// if below lower
     
        // check if right edge tile
        else if x1 >= upperBound {
           //T(x0, dx0, upperBound, 0, t0, t1);
        

           // for t in t0..t1
           var readBuffer = (t0 - 1) & 1;
           var writeBuffer = 1 - readBuffer;
           for t in t0..t1 {

              // for x in x0+dx0*(t-t0) .. x1+dx1*(t-t0) && in computation space
              for x in max( x0 + dx0 * (t - t0), lowerBound) .. upperBound {         
                 stencil (readBuffer, writeBuffer, x);
              } // for x
              readBuffer <=> writeBuffer;
           } // for t
        
        } // if outside upper
     
        // otherwise, regular ol' tile
        else {
           //T(x0, dx0, x1, dx1, t0, t1)
        
           // for t in t0..t1
           var readBuffer = (t0 - 1) & 1;
           var writeBuffer = 1 - readBuffer;
           for t in t0..t1 {

              // for x in x0+dx0*(t-t0) .. x1+dx1*(t-t0) && in computation space
              for x in max( x0 + dx0 * (t - t0), lowerBound) .. min( x1 + dx1 * (t - t0), upperBound ) {         
                 stencil (readBuffer, writeBuffer, x);
              }
              readBuffer <=> writeBuffer;
           }
        }// else
     } // for x0 (A)

     // swap dx0 and dx1.
     dx0 <=> dx1 ;

     // Do B-tiles

     // for all starting points of x0 do a tile
     forall x0 in tiles_B_start .. upperBound by betweenTiles {

        var x1 = x0 + width_min - 1;
  
        // check if right edge tile
        if x1 >= upperBound {
           //T( x0, dx0, upperBound, 0, t0, t1 )
        
           // for t in t0..t1
           var readBuffer = (t0 - 1) & 1;
           var writeBuffer = 1 - readBuffer;
           for t in t0..t1 {

              // for x in x0+dx0*(t-t0) .. x1+dx1*(t-t0) && in computation space
              for x in max( x0 + dx0 * (t - t0), lowerBound) .. upperBound  {         
                 stencil (readBuffer, writeBuffer, x);
              } // for x
              readBuffer <=> writeBuffer;
           } // for t
        } // if outside upper bound
     
        // otherwise, regular ol' tile            
        else {
           //T( x0, dx0, x1, dx1, t0, t1 )
        
           // for t in t0..t1
           var readBuffer = (t0 - 1) & 1;
           var writeBuffer = 1 - readBuffer;
           for t in t0..t1 {

              // for x in x0+dx0*(t-t0) .. x1+dx1*(t-t0) && in computation space
              for x in max( x0 + dx0 * (t - t0), lowerBound) .. min( x1 + dx1 * (t - t0), upperBound ) {         
                 stencil (readBuffer, writeBuffer, x);
              } // for x
              readBuffer <=> writeBuffer;
           } // for t
        } // else
  
     } // for x0 (B)

     // swap dx0 and dx1 back for next '1' tiles round
     dx0 <=> dx1; 

  } // for time-band
} // proc   


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
   var timer: Timer;
   // initialize space with values
   initializeSpace();
   
   timer.start();
   
   InlinedTrapezoidTilingProcedure();
   
   timer.stop();

   return timer.elapsed();
}

proc main(){
   
   var time = Test_1();
   if printTime then writeln( "Time: ", time );
   
   if verifyResult( false ) then writeln( "SUCCESS" );
   else writeln( "FAILURE" );
}
