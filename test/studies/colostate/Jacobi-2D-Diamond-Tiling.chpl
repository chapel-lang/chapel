/*
    Jacobi-3D (2d-space x time) Diamond Slab Tiling implementation
    adapted to chapel by Ian J. Bertolacci (Colorado State Univeristy), originally 
    written by Michelle M. Strout (CSU), modified by Cathie Olschanowsky (CSU) 
    and David G. Wonnacott (Haverford College) 
    9/18/2014
*/

use Time;
use Random;
use AdvancedIters;

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

// diamond tile size, can be any multiple of 3 >=15
config const tau: int  = 15; 
// number of time slices to use in each round of the tiling pattern
config const subset_s: int  = 3; 

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


iter SlicedDiamondIterator( ){
    var read: int = 0, write: int = 1;

    // s is the number of non-pointy bit 2D slices of diamond tiling
    // that is available for the current tile size.
    var s: int = (tau/3) - 2;
    // subset_s is an input parameter indicating how many of those
    // slices we want to use in the repeated tiling pattern.
    // subset_s should be less than s and greater than or equal to 2.
    if subset_s > s  || subset_s<2 {
        writeln( "Error: need 2<=subset_s<=s\n" );
        exit(-1);
    }

    // Set lower and upper bounds for spatial dimensions.
    // When did code gen have a non-inclusive upper bound.
    // Ian's upper bound is inclusive.
    var  Li: int = 1, Lj: int = 1, Ui: int = upperBound+1, Uj: int = upperBound+1;

    // Loop over the tiling pattern.
    for toffset in 0 .. T-1 by subset_s {

        // Loop over phases of tiles within repeated tile pattern.
        // This is like iterating over the A and B trapezoid tile types.
        for c0 in -2 .. floord( T, tau/3 ) {
            // Two loops over tiles within one phase.
            // All of the tiles within one phase can be done in parallel.
            for c1 in 
                max(
                    max(
                        floord(Lj + (tau/3) * c0 + (tau/3), tau), 
                        c0 + floord(-2 * T + Lj - 1, tau) + 1), 
                    floord(Lj + 1, tau)
                )
                .. 
                min(
                    min(
                        floord(Uj + (tau/3) * c0 - ((tau/3)+2), tau) + 1, 
                        floord(T + Uj - 1, tau)), 
                    c0 + floord(Uj - 5, tau) + 2
                )
                { // for c1
                
                for c2 in 
                    max(
                        max(
                            max(
                                max(
                                    max(
                                        max(
                                            c0 - 2 * c1 + floord(-Ui + Lj + 1,tau), 
                                            -c1 + floord(-2 * Ui - Uj + tau * c0 + tau * c1 - tau-3, tau*2)+1),
                                        c1 + floord(-Ui - 2 * Uj + 3, tau)),
                                    floord(-Ui - Uj + 3, tau)), 
                                c0 - c1 + floord(-Ui - (tau/3) * c0 + ((tau/3)+1), tau)), 
                            c0 - c1 + floord(-T - Ui, tau) + 1), 
                        -c1 + floord(-Ui + 4, tau) - 1
                    )
                    .. 
                    min(
                        min(
                            min(
                                min(
                                    min(
                                        min(
                                            c0 - 2 * c1 + floord(-Li + Uj - 2, tau) + 1, 
                                            c0 - c1 + floord(-Li - 2, tau) + 1), 
                                        c0 - c1 + floord(-Li - (tau/3) * c0 - ((tau/3)+1), tau) + 1),
                                    floord(T - Li - Lj, tau)),
                                -c1 + floord(2 * T - Li, tau)), 
                            c1 + floord(-Li - 2 * Lj - 1, tau) + 1), 
                        -c1 + floord(-2 * Li - Lj + tau * c0 + tau * c1 + (tau-1), tau*2)
                    )
                    { // for c2
                    
                    // Loop over subset_s time steps within tiling pattern
                    // and within tile c0,c1,c2.
                    // Every time the pattern is repeated, toffset will be
                    // subset_s bigger.
                    // The real t value is c3+toffset.  We are just using the
                    // tiling pattern from t=1 to t<=subset_s.
                    for c3 in 1 .. min( T-toffset, subset_s) {
                        var t: int = c3+toffset;
                        // if t % 2  is 1, then read=0 and write=1
                        write = t & 1;
                        read = 1-write;

                        // x spatial dimension.
                        for c4 in 
                            max(
                                max(
                                    max(
                                        -tau * c1 - tau * c2 + 2 * c3 - (2*tau-2),
                                        -Uj - tau * c2 + c3 - (tau-2)),
                                   tau * c0 - tau * c1 - tau * c2 - c3),
                                 Li
                            )
                            .. 
                            min(
                                min(
                                    min(
                                        tau * c0 - tau * c1 - tau * c2 - c3 + (tau-1),
                                        -tau * c1 - tau * c2 + 2 * c3),
                                    -Lj - tau * c2 + c3),
                                Ui - 1
                            )
                            {// for c4
                            // y spatial dimension.
                            for c5 in 
                                max(
                                    max(
                                        tau * c1 - c3,
                                        Lj),
                                    -tau * c2 + c3 - c4 - (tau-1)
                                ) // end init block c5
                                .. 
                                min(
                                    min(
                                        Uj - 1,
                                        -tau * c2 + c3 - c4),
                                    tau * c1 - c3 + (tau-1)
                                )
                                { // for c5
                                    yield ( read, write, c4, c5);
                            } // for c5
                        } // for c4
                    } // for c3
                } // for c2
            } // for c1
        } // for c0
    } // for toffset
}


iter SlicedDiamondIterator(  param tag: iterKind  )
where tag == iterKind.leader {
    //var read: int = 0, write: int = 1;
    var tau_OVER_3: int = (tau/3); // common subexpression for CSE

    // s is the number of non-pointy bit 2D slices of diamond tiling
    // that is available for the current tile size.
    var s: int = tau_OVER_3 - 2;

    // subset_s is an input parameter indicating how many of those
    // slices we want to use in the repeated tiling pattern.
    // subset_s should be less than s and greater than or equal to 2.
    if subset_s > s  || subset_s < 2 {
        writeln("Error: need 2<=subset_s<=s: ", subset_s, "\n");
        exit(-1);
    }

    // We want to position subset_s in the middle of the s range
    // so, take s and subtract subset_s divide that by 2 and we have
    // the beginning of the middle.
    // Due to the read and write flags, we need start_s
    // to start on an odd number.
    var start_s: int =  (s/2)-(subset_s/2);
    if start_s % 2 == 0 then start_s -=1;
    if start_s < 0 then start_s = 1;
    var stop_s:int = start_s + subset_s - 1;

    // Set lower and upper bounds for spatial dimensions.
    // When did code gen have a non-inclusive upper bound.
    // Ian's upper bound is inclusive.
    var Li: int = 1, 
        Lj: int = 1, 
        Ui: int = upperBound + 1, 
        Uj: int = upperBound + 1;

    // Loop over the tiling pattern.
    //for (int toffset=0; toffset<T; toffset+=subset_s){
    for toffset in 0 .. T by subset_s {

        // Loop over phases.  Tiles within each phase can be executed in parallel.
	    //for (int c0 = -2; c0 <= 0; c0 += 1){
	    // We have unrolled this loop below because then
	    // we can specialize the c1 and c2 bounds and make them bounding
	    // boxes for OpenMP.

	    // ================= Phase 1, c0=-2 =====================
	    var c0: int = -2;
	    var c1lb: int = floord(Lj + 1, tau);
	    var c1ub: int = floord(Uj -5, tau);
	    var c2lb: int = floord(-Ui - Uj + 3, tau);
	    var c2ub: int = -floord(Lj + 1, tau) + floord(-Li - 2, tau) - 1;
        //#if PARALLEL
        //#pragma omp parallel for shared(start_time, s, Li, Lj, Ui, Uj, toffset, c0 ) private(read, write) collapse(2) schedule(dynamic)
        //#endif
	    // for (int c1 = c1lb; c1 <= c1ub; c1 += 1){
	    //write( "c1lb ", c1lb, ", c1ub ", c1ub );
	    var c1range = c1lb..c1ub;
	    var csize = max( 1, ceil(c1range.length/(if dataParTasksPerLocale==0 then here.numCores else dataParTasksPerLocale)):int);
        forall c1 in dynamic(c1range, chunkSize=csize ){
        //forall c1 in guided(c1lb..c1ub) {
        //forall c1 in adaptive(c1lb..c1ub) {
        //forall c1 in c1lb..c1ub {  
		    //for (int c2 = c2lb; c2<=c2ub; c2++) 
            //forall c2 in c2lb..c2ub {
            for c2 in c2lb..c2ub {
                
			    // Every time the pattern is repeated, toffset will be
			    // subset_s bigger.
			    // The real t value is c3+toffset-start_s+1.  
			    // We are just using the tiling pattern from c3=start_s to 
			    // c3<=stop_s.
			    for c3 in start_s..min( T-toffset+start_s-1, stop_s) {
				     var t: int = c3 + toffset - start_s + 1;
				     // if t % 2  is 1, then read=0 and write=1
				     var write = t & 1;
				     var read = 1-write;

                    for c4 in maxs(-tau * c1 - tau * c2 + 2 * c3 - (2*tau-2),
                                   -Uj - tau * c2 + c3 - (tau-2),
                                   tau * c0 - tau * c1 - tau * c2 - c3,
                                   Li)
			                      .. 
				                  mins(tau * c0 - tau * c1 - tau * c2 - c3 + (tau-1),
				                       -tau * c1 - tau * c2 + 2 * c3,
				                       -Lj - tau * c2 + c3,
				                       Ui - 1)
			                      {
					    //for (int c5 = max(max(tau * c1 - c3, Lj), -tau * c2 + c3 - c4 - (tau-1)); c5 <= min(min(Uj - 1, -tau * c2 + c3 - c4), tau * c1 - c3 + (tau-1)); c5 += 1) 
                        for c5 in maxs(tau * c1 - c3,
                                       Lj, 
                                       -tau * c2 + c3 - c4 - (tau-1))
                                       ..
                                       mins(Uj - 1, 
                                            -tau * c2 + c3 - c4, 
                                            tau * c1 - c3 + (tau-1))
                                       {
						    yield ( read, write, c4, c5);
                        } // for c5
                    }// for c4
                }// for c3
            }// for c2
	    }// for c1

	    // ================= Phase 2, c0=-1 =====================
        c0 = -1;
	    c1lb = floord(Lj + 1, tau);
	    c1ub = floord(Uj + (tau_OVER_3)-2, tau);
	    c2lb = floord(-Ui - Uj + 3, tau);
	    c2ub = floord(-Li - Lj + (tau_OVER_3)-3, tau);
        //#if PARALLEL
        //#pragma omp parallel for shared(start_time, s, Li, Lj, Ui, Uj, toffset, c0 ) private(read, write) collapse(2) schedule(dynamic)
        //#endif
        //for (int c1 = c1lb; c1 <= c1ub; c1 += 1)
	    //c1range = c1lb..c1ub;
	    //csize = (c1range.length/(if dataParTasksPerLocale==0 then here.numCores else dataParTasksPerLocale)):int;
        //writeln( c1range, "|", csize, "|" );
        //forall c1 in dynamic(c1range, chunkSize=csize ){
        //forall c1 in c1lb..c1ub {
        c1range = c1lb..c1ub;
	    csize = max( 1, ceil(c1range.length/(if dataParTasksPerLocale==0 then here.numCores else dataParTasksPerLocale)):int);
        forall c1 in dynamic(c1range, chunkSize=csize ){
		    //for (int c2 = c2lb; c2<=c2ub; c2++) 
            //forall c2 in c2lb..c2ub {
            for c2 in c2lb..c2ub {

                // Every time the pattern is repeated, toffset will be
		        // subset_s bigger.
		        // The real t value is c3+toffset-start_s+1.  
		        // We are just using the tiling pattern from c3=start_s to 
		        // c3<=stop_s.
		        //for (int c3 = start_s; c3 <= min(T-toffset+start_s-1,stop_s); c3 += 1)
			    for c3 in start_s..min(T-toffset+start_s-1,stop_s) {
			        var t: int = c3 + toffset - start_s + 1;
				    // if t % 2  is 1, then read=0 and write=1
				    var write = t & 1;
				    var read = 1-write;
                    for c4 in maxs(-tau * c1 - tau * c2 + 2 * c3 - (2*tau-2),
                                            -Uj - tau * c2 + c3 - (tau-2),
                                            tau * c0 - tau * c1 - tau * c2 - c3,
                                            Li) 
                                       ..
                                       mins( tau * c0 - tau * c1 - tau * c2 - c3 + (tau-1),
                                           -tau * c1 - tau * c2 + 2 * c3,
                                           -Lj - tau * c2 + c3, Ui - 1)
                                       {
                        //for (int c5 = max(max(tau * c1 - c3, Lj), -tau * c2 + c3 - c4 - (tau-1)); c5 <= min(min(Uj - 1, -tau * c2 + c3 - c4), tau * c1 - c3 + (tau-1)); c5 += 1) 
					    for c5 in maxs(tau * c1 - c3, Lj, 
				                            -tau * c2 + c3 - c4 - (tau-1))
			                            ..
			                            mins(Uj - 1, -tau * c2 + c3 - c4,
			                                 tau * c1 - c3 + (tau-1))
		                                {

                            yield ( read, write, c4, c5);
                        }
                    }
                }
            }
        }
	    // ================= Phase 3, c0=0 =====================
        c0 = 0;
	    c1lb = floord(Lj + (tau_OVER_3)+4, tau);
	    c1ub = floord(Uj + (tau_OVER_3)-2, tau);
	    c2lb = -floord(Uj + (tau_OVER_3)-2, tau) + floord(-Ui-(tau_OVER_3-3),tau)+1;
	    c2ub = floord(-Li - Lj + (tau_OVER_3-3), tau);
        //#if PARALLEL
        //#pragma omp parallel for shared(start_time, s, Li, Lj, Ui, Uj, toffset, c0 ) private(read, write) collapse(2) schedule(dynamic)
        //#endif
        //for (int c1 = c1lb; c1 <= c1ub; c1 += 1)
        //forall c1 in c1lb..c1ub {
        c1range = c1lb..c1ub;
	    csize = max( 1, ceil(c1range.length/(if dataParTasksPerLocale==0 then here.numCores else dataParTasksPerLocale)):int);
        forall c1 in dynamic(c1range, chunkSize=csize ){
            var read: int = 0;
            var write: int = 1;
		    //for (int c2 = c2lb; c2<=c2ub; c2++)
            //forall c2 in c2lb..c2ub {
            for c2 in c2lb..c2ub {  
                // Every time the pattern is repeated, toffset will be
			    // subset_s bigger.
			    // The real t value is c3+toffset-start_s+1.  
			    // We are just using the tiling pattern from c3=start_s to 
			    // c3<=stop_s.
			    //for (int c3 = start_s; c3 <= min(T-toffset+start_s-1,stop_s); c3 += 1)
                for c3 in start_s..min(T-toffset+start_s-1,stop_s) {
				    var t: int = c3 + toffset - start_s + 1;
				    // if t % 2  is 1, then read=0 and write=1
                    var write = t & 1;
                    var read = 1-write;

				    //for (int c4 = max(max(max(-tau * c1 - tau * c2 + 2 * c3 - (2*tau-2), -Uj - tau * c2 + c3 - (tau-2)), tau * c0 - tau * c1 - tau * c2 - c3), Li); c4 <= min(min(min(tau * c0 - tau * c1 - tau * c2 - c3 + (tau-1), -tau * c1 - tau * c2 + 2 * c3), -Lj - tau * c2 + c3), Ui - 1); c4 += 1) 
                    for c4 in maxs(-tau * c1 - tau * c2 + 2 * c3 - (2*tau-2),
                                         -Uj - tau * c2 + c3 - (tau-2),
                                         tau * c0 - tau * c1 - tau * c2 - c3,
                                         Li)
                                    ..
                                    mins(tau * c0 - tau * c1 - tau * c2 - c3 + (tau-1),
                                         -tau * c1 - tau * c2 + 2 * c3,
                                         -Lj - tau * c2 + c3,
                                         Ui - 1)
                                    {  
					    //for (int c5 = max(max(tau * c1 - c3, Lj), -tau * c2 + c3 - c4 - (tau-1)); c5 <= min(min(Uj - 1, -tau * c2 + c3 - c4), tau * c1 - c3 + (tau-1)); c5 += 1) 
                        for c5 in maxs(tau * c1 - c3, Lj,
                                            -tau * c2 + c3 - c4 - (tau-1) )
                                       ..
                                       mins( Uj - 1,
                                             -tau * c2 + c3 - c4,
                                             tau * c1 - c3 + (tau-1) )
                                       {
						    yield ( read, write, c4, c5);
                        }
                    }
                }
            }
        }
    } // for toffset  
  
}

iter SlicedDiamondIterator( followThis, param tag: iterKind  )
where tag == iterKind.follower {
    yield followThis; 
  
}


proc floord( n: int , d: int ): int {
    var r: int = n/d - ( if n % d < 0 then 1 else 0);
    assert(d*r <= n && n < d*(r+1));
    assert(d*r + n%d + (if n % d < 0 then d else 0) == n);
    return r;
}

proc maxs( x ... ?k ){
    var maxVal = x(1);
    for param i in 2..k {
        if maxVal < x(i) then
            maxVal = x(i);
    }
    return maxVal;
}

proc mins( x ... ?k ){
    var minVal = x(1);
    for param i in 2..k{
        if minVal > x(i) then
            minVal = x(i);
    }
    return minVal;
}

proc Test_1(){
   var timer: Timer;
   
   initializeSpace();
   
   timer.start();
   
   forall (read, write, x, y) in SlicedDiamondIterator(){
      stencil( read, write, x, y );
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
